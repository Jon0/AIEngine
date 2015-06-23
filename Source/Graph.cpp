#include "Action.h"
#include "ActionOrder.h"
#include "Flow.h"
#include "Graph.h"
#include "Resource.h"

namespace graph {

GraphProducer::GraphProducer() {}

void GraphProducer::add_action(const Action &a) {
	actions.push_back(a);
}

void GraphProducer::add_location(const Location &l) {
	locations.push_back(l);
}

void GraphProducer::add_resource(const Resource &r) {
	resources.push_back(r);
}

std::vector<Action> GraphProducer::get_actions() const {
	return actions;
}

std::vector<Location> GraphProducer::get_locations() const {
	return locations;
}

std::vector<Resource> GraphProducer::get_resources() const {
	return resources;
}

Graph::Graph(const GraphProducer &gp) {

	for (auto &a : gp.get_actions()) {
		add_action(a);
	}
	for (auto &n : gp.get_resources()) {
		add_resource(n);
	}

	// TODO resource.add_effect for actions
}

Graph::~Graph() {}

void Graph::update() {

	// check for resource changes
	for (auto &r : resource) {
		r.second->update();
	}
}

void Graph::add_action(const Action &a) {
	order_act.push_back(a);
	Action *a_ptr = &order_act.back();
	order_edge.push_back(a_ptr);
	action.emplace(std::make_pair(a.get_name(), a_ptr));
}

Flow *Graph::add_flow(const Flow &f) {
	order_flow.push_back(f);
	Flow *new_flow = &order_flow.back();
	order_edge.push_back(new_flow);
	return new_flow;
}

void Graph::add_resource(const Resource &r) {
	order_res.push_back(r);
	Resource *r_ptr = &order_res.back();
	resource.emplace(std::make_pair(r.get_name(), r_ptr));
}

Action *Graph::get_action(std::string name) {
	return action[name];
}

std::vector<Action *> Graph::get_action_list() {
	std::vector<Action *> act;
	for (auto &a : order_act) {
		act.push_back(&a);
	}
	return act;
}

Resource *Graph::get_resource(std::string name) {
	if (resource.count(name) > 0) {
		return resource[name];
	}
	return new Resource("resource not found");
}

std::vector<Resource *> Graph::get_resource_list() {
	std::vector<Resource *> res;
	for (auto &r : order_res) {
		res.push_back(&r);
	}
	return res;
}

ResourceSet Graph::get_amounts() const {
	std::unordered_map<std::string, double> result;
	for (auto &r : resource) {
		result.emplace(std::make_pair(r.first, r.second->get_amount()));
	}
	return ResourceSet(result);
}

void Graph::set_win_func(std::function<double(const ResourceSet &)> f) {
	win_func = f;
}

double Graph::evaluate_win_func() {
	return evaluate_win_func(get_amounts());
}

double Graph::evaluate_win_func(const ResourceSet &r) {
	if (!win_func) {
		return 0.0;
	}
	return win_func(r);
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
