#include "Action.h"
#include "Resource.h"

namespace graph {

Action::Action(std::string name) {}

Action::~Action() {}

bool Action::controlled() {
	return true;
}

double Action::get_time() {
	return 1.0;
}

std::unordered_map<Resource *, double> Action::get_effects() {
	return resource;
}

void Action::apply_start_effect(ResourceSet &r) {
	for (auto &e : resource) {
		r.adjust_amount(e.first, e.second);
	}
}

void Action::apply_end_effect(ResourceSet &r) {
	// TODO some effects occur later
}

void Action::add_effect(Resource *r, double amount, bool must_be_positive) {
	resource.emplace(std::make_pair(r, amount));
	require_positive.emplace(std::make_pair(r, must_be_positive));

	// link to the resource
	r->effect_of(this);
}

bool Action::can_do_event(ResourceSet amount) {

	// check all required resources are available
	for (auto &a : amount.get_amounts()) {
		if (require_positive.count(a.first) > 0 &&
			require_positive[a.first] &&
			a.second < resource[a.first]) {
			return false;
		}
	}
	return true;
}

void Action::do_event() {
	if (act_func && act_func()) {

		// modify the resource amounts
		for (auto &r : resource) {
			r.first->adjust(r.second);
		}
	}
}

void Action::set_event(std::function<bool()> e) {
	act_func = e;
}

std::string to_string(Action &a) {
	std::string result = "[";
	auto effects = a.get_effects();
	int i = 0, effects_size = effects.size();
	for (auto &e : effects) {
		result += e.first->get_name() + ": " + std::to_string(e.second);
		if (i < effects_size - 1) {
			result += ", ";
		}
		i++;
	}
	result += "]";
	return result;
}

}


