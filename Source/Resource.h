#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace graph {

class Action;
class Flow;
class Graph;

class Resource {
public:
	Resource(std::string name, bool allow_negative=false);
	~Resource();

	virtual bool has_location();

	std::string get_name() const;

	bool allow_negative() const;

	/**
	 * use the update function
	 */
	void update();

	/**
	 * adds an amount to the resource
	 */
	void adjust(double a);

	/**
	 * return the stockpiled amount
	 */
	double get_amount() const;

	/**
	 * return actions which affect this resource
	 */
	std::vector<Action *> get_effects();

	/**
	 * pass a function returning the current
	 * amount of this resource allowing the resource
	 * to be automatically adjusted on updates
	 */
	void set_update_function(std::function<double()> f);

	/**
	 * creates a flow from this resource to another
	 */
	void produces(Graph *graph, Resource *other, double rate_in, double rate_out);

	/**
	 * adds an action to this resources list of action
	 * which affect it
	 */
	void effect_of(Action *a);

	std::string debug();

private:
	const std::string resource_name;
	const bool allow_neg;
	double amount, amount_d, value;
	int updates;

	void add_flow_in(Flow *);

	std::function<double()> update_func;

	std::vector<Flow *> flows_out;
	std::vector<Flow *> flows_in;
	std::vector<Action *> effect_action;

};

std::string to_string(const Resource &r);

}
