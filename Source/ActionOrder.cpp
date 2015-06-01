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
	total_time(0.0) {
}

ActionOrder::~ActionOrder() {}

double ActionOrder::length() {
	return total_time;
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


std::vector<std::pair<double, Action *>> ActionOrder::get_actions() {
	return actions;
}

ResourceSet ActionOrder::get_effect() {
	get_effect(g->get_amounts(), length());
}

ResourceSet ActionOrder::get_effect(ResourceSet a) {
	return get_effect(a, length());
}

ResourceSet ActionOrder::get_effect(ResourceSet set, double time_duration) {
	ResourceSet result = set;

	// actions to be started
	std::deque<std::pair<double, Action *>> to_start;
	for (auto &a : actions) {
		to_start.push_back(a);
	}

	// actions to be completed
	// time, resource, amount
	// sorted by time
	std::vector<std::tuple<double, Resource *, double>> to_complete;

	for (double t = 0.0; t < time_duration; t += 1.0) {

		// apply edges

		// add effects
		// TODO: may be many actions on time t
		if (!to_start.empty() && t >= to_start.front().first) {

			// apply all effects instantly for now
			Action *act = to_start.front().second;
			act->apply_start_effect(result);
			act->apply_end_effect(result);
			to_start.pop_front();
		}
	}

	// TODO find the outcome
	return result;
}

std::string to_string(ActionOrder &o) {
	std::string result = "{\n";
	for (auto &a : o.get_actions()) {
		result += "\t" + std::to_string(a.first) + " : " + to_string(*a.second) + "\n";
	}
	result += "} [" + std::to_string(o.length()) + "]\n";
	return result;
}

}
