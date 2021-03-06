#include <algorithm>
#include <deque>

#include "Action.h"
#include "ActionOrder.h"
#include "Graph.h"

namespace graph {

bool compare(std::pair<double, Action *> a, std::pair<double, Action *> b) {
	return a.first < b.first;
}

ActionOrder::ActionOrder(Graph *gr)
	:
	g(gr),
	total_time(0.0),
	graph_value(0.0) {
}

ActionOrder::~ActionOrder() {}

double ActionOrder::length() const {
	return total_time;
}

double ActionOrder::value() const {
	return graph_value;
}

void ActionOrder::set_value(double v) {
	graph_value = v;
}

void ActionOrder::add_action(double time, Action *a) {
	actions.push_back(std::make_pair(time, a));

	// maintain ordering
	std::sort(std::begin(actions), std::end(actions), compare);

	// make required adjustments to order length
	total_time = std::max(total_time, time + a->get_time());
}

std::pair<double, Action *> ActionOrder::first_action() {
	if (actions.empty()) {
		return std::make_pair(0, nullptr);
	}
	return actions[0];
}


std::vector<std::pair<double, Action *>> ActionOrder::get_actions() const {
	return actions;
}

ResourceSet ActionOrder::get_effect() {
	return get_effect(g->get_amounts());
}

ResourceSet ActionOrder::get_effect(ResourceSet a) {
	return get_effect(a, length());
}

ResourceSet ActionOrder::get_effect(ResourceSet set, double time_duration) {
	ResourceSet result = set;

	// actions to be started
	std::deque<std::pair<double, Action *>> to_start(actions.begin(), actions.end());

	// actions to be completed
	// time, action
	// sorted by time
	std::deque<std::pair<double, Action *>> to_complete;

	for (double t = 0.0; t <= time_duration; t += 1.0) {

		// apply edges
		// add effects
		// TODO: may be many actions on time t
		while (!to_start.empty() && t >= to_start.front().first) {

			// apply start effects instantly
			Action *act = to_start.front().second;
			act->apply_start_effect(result);
			to_complete.push_back(std::make_pair(t + act->get_time(), act));
			to_start.pop_front();
		}

		// complete effects
		while (!to_complete.empty() && t >= to_complete.front().first) {
			Action *act = to_complete.front().second;
			act->apply_end_effect(result);
			to_complete.pop_front();
		}
	}
	return result;
}

std::string ActionOrder::effect_str(Graph *g) {
	std::string result = "{\n";
	for (auto &a : get_actions()) {
		result += "\t" + std::to_string(a.first) + " "
				+ to_string(get_effect(g->get_amounts(), a.first)) + " : "
				+ to_string(*a.second) + "\n";
	}
	result += "\t" + std::to_string(length()) + " "
			+ to_string(get_effect(g->get_amounts())) + " : complete\n";
	result += "}\n";

	return result;
}

std::string to_string(const ActionOrder &o) {
	std::string result = "{\n";
	for (auto &a : o.get_actions()) {
		result += "\t" + std::to_string(a.first) + " : " + to_string(*a.second) + "\n";
	}
	result += "\t" + std::to_string(o.length()) + " : complete\n";
	result += "} [" + std::to_string(o.value()) + "]\n";
	return result;
}

}
