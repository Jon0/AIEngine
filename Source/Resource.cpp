#include "Action.h"
#include "Flow.h"
#include "Graph.h"
#include "Resource.h"

namespace graph {

Resource::Resource(std::string name, bool allow_negative)
	:
	resource_name(name),
	allow_neg(allow_negative),
	amount(0.0),
	updates(0) {
}

Resource::~Resource() {}

bool Resource::has_location() {
	return false;
}

std::string Resource::get_name() const {
	return resource_name;
}

bool Resource::allow_negative() const {
	return allow_neg;
}

void Resource::update() {

	// expected resource changes are given via adjust
	// this update must account for other incomes

	if (update_func) {
		double new_amount = update_func();

		double p = static_cast<double>(updates) / static_cast<double>(updates + 1);

		// average the income rate
		amount_d = p * amount_d + (1 - p) * (new_amount - amount);

		// set current amount
		amount = new_amount;

		updates++;

		// divide the income over incoming flows
		for (auto &in : flows_in) {
			in->set_rate(amount_d / flows_in.size());
		}
	}
}

void Resource::adjust(double a) {
	amount += a;
}

double Resource::get_amount() const {
	return amount;
}

std::vector<Action *> Resource::get_effects() {
	return effect_action;
}

void Resource::set_update_function(std::function<double()> f) {
	update_func = f;
}

void Resource::produces(Graph *graph, Resource *other, double rate_in, double rate_out) {
	Flow *flow = graph->add_flow(Flow(this, other, rate_in, rate_out));
	flows_out.emplace_back(flow);
	other->add_flow_in(flow);
}

void Resource::effect_of(Action *a) {
	effect_action.push_back(a);
}

std::string Resource::debug() {
	return std::to_string(amount) + ", " + std::to_string(amount_d);
}

void Resource::add_flow_in(Flow *f) {
	flows_in.push_back(f);
}

std::string to_string(const Resource &r) {
	return r.get_name() + ": " + std::to_string(r.get_amount());
}

}
