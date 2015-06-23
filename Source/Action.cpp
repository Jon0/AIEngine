#include "Action.h"
#include "Graph.h"
#include "Resource.h"

namespace graph {

Action::Action(std::string name, double time, const ResourceSetDelta &pre, const ResourceSetDelta &post)
	:
	action_name(name),
	action_time(time),
	pre_effect(pre),
	post_effect(post) {
}

Action::~Action() {}

std::string Action::get_name() const {
	return action_name;
}

bool Action::controlled() {
	return true;
}

double Action::get_time() {
	return 1.0;
}

ResourceSetDelta Action::get_pre_effect() {
	return pre_effect;
}

ResourceSetDelta Action::get_post_effect() {
	return post_effect;
}

void Action::apply_start_effect(ResourceSet &r) {
	for (auto &e : pre_effect.get_deltas()) {
		r.adjust_amount(e.first, e.second);
	}
}

void Action::apply_end_effect(ResourceSet &r) {
	for (auto &e : post_effect.get_deltas()) {
		r.adjust_amount(e.first, e.second);
	}
}

bool Action::can_do_event(Graph *g, ResourceSet amount) {

	// check all required resources are available
	for (auto &a : amount.get_amounts()) {

		// if the amount is not enough
		if (!g->get_resource(a.first)->allow_negative() &&
			a.second < -pre_effect.get_delta(a.first)) {
			return false;
		}
	}
	return true;
}

void Action::do_event(Graph *g) {
	if (act_func && act_func()) {

		// modify the resource amounts
		for (auto &e : pre_effect.get_deltas()) {
			g->get_resource(e.first)->adjust(e.second);
		}
	}
}

void Action::set_event(std::function<bool()> e) {
	act_func = e;
}

std::string to_string(Action &a) {
	std::string result = "[";
	result += a.get_name() + ", ";
	result += std::to_string(a.get_time()) + ", ";
	result += to_string(a.get_pre_effect()) + " >> ";
	result += to_string(a.get_post_effect()) + "]";
	return result;
}

}


