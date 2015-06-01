#include "Action.h"
#include "ActionOrder.h"
#include "Flow.h"
#include "Graph.h"
#include "Resource.h"

namespace graph {

Graph *Graph::create(std::vector<std::string> resource_names) {
	Graph *instance = new Graph();
	for (auto &n : resource_names) {
		instance->add_resource(n);
	}

	return instance;
}


Graph::Graph() {}

Graph::~Graph() {}

void Graph::update() {

	// check for resource changes
	for (auto &r : resource) {
		r.second->update();
	}

	// TODO: best action
}

void Graph::add_action(std::string name) {
	action.emplace(std::make_pair(name, std::make_unique<Action>(name)));
	order_act.push_back(action[name].get());
	edge.push_back(action[name].get());
}

Action *Graph::get_action(std::string name) {
	return action[name].get();
}

std::vector<Action *> Graph::get_action_list() const {
	return order_act;
}

void Graph::add_resource(std::string name) {
	resource.emplace(std::make_pair(name, std::make_unique<Resource>(this, name)));
	order_res.push_back(resource[name].get());
}

Resource *Graph::get_resource(std::string name) {
	return resource[name].get();
}

std::vector<Resource *> Graph::get_resource_list() const {
	return order_res;
}

ResourceSet Graph::get_amounts() const {
	std::unordered_map<Resource *, double> result;
	for (auto &r : resource) {
		result.emplace(std::make_pair(r.second.get(), r.second->get_amount()));
	}
	return ResourceSet(result);
}

void Graph::set_win_func(std::function<double(ResourceSet)> f) {
	win_func = f;
}

double Graph::evaluate_win_func() {
	return evaluate_win_func(get_amounts());
}

double Graph::evaluate_win_func(ResourceSet r) {
	if (!win_func) {
		return 0.0;
	}
	return win_func(r);
}

void Graph::register_flow(Flow *f) {
	edge.push_back(f);
	flow.push_back(f);
}

void Graph::update_actions() {

	// execute first action
	std::pair<double, Action *> a = get_best_action().first_action();
	if (a.second) {
		a.second->do_event();
	}
}

ActionOrder Graph::get_best_action() {
	if (!win_func) {
		return ActionOrder(this);
	}

	// current amounts
	ResourceSet current_amounts = get_amounts();

	std::vector<ActionOrder> orders;

	// add blank order
	orders.push_back(ActionOrder(this));

	//
	int max_iterations = 3;
	for (int time = 0; time < max_iterations; ++time) {
		for (auto &order : orders) {
			ResourceSet order_amounts = order.get_effect(current_amounts, time);

			// get available actions
			// at time within the orders context
			std::vector<Action *> available;
			for (auto &a : action) {
				//if (a.second->can_do_event(order_amounts)) {
				//	available.push_back(a.second.get());
				//}
				available.push_back(a.second.get());
			}

			for (auto &a : available) {
				// add to the order
				// and add a new branch
				ActionOrder new_order = order;
				new_order.add_action(time, a);
				orders.push_back(new_order);
			}

		}
	}

	// find the most valuable
	ActionOrder *best = &orders.front();
	double best_score = 0;
	for (auto &order : orders) {
		double score = win_func(order.get_effect(current_amounts)); // / order.length();
		if (score > best_score) {
			best = &order;
			best_score = score;
		}
	}

	return *best;
}

std::vector<std::string> Graph::debug() {
	std::vector<std::string> result;
	for (auto &a : resource) {
		double amount = a.second->get_amount();
		result.push_back(a.first + " = " + std::to_string(amount));
	}
	return result;
}

std::string to_string(Graph &g) {
	std::string result = "";

	result += "resources:\n";
	for (auto &r : g.get_resource_list()) {
		result += to_string(*r) + " ";
	}

	result += "\n\nactions:\n";
	for (auto &a : g.get_action_list()) {
		result += to_string(*a) + "\n";
	}

	result += "\nwin:\n";
	result += std::to_string(g.evaluate_win_func()) + "\n";

	return result;
}

}
