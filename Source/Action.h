#pragma once

#include <functional>
#include <string>
#include <unordered_map>

#include "Edge.h"
#include "ResourceSet.h"

namespace graph {

class Resource;

class Action : public Edge {
public:
	Action(std::string name);
	~Action();

	bool controlled() override;

	double get_time() override;

	std::unordered_map<Resource *, double> get_effects() override;

	void apply_start_effect(ResourceSet &r);
	void apply_end_effect(ResourceSet &r);

	/**
	 * the resource effect expected from using this action
	 * some actions can only occur with a positive resource amount
	 */
	void add_effect(Resource *r, double amount, bool must_be_positive=false);

	/**
	 * can the action be used with these resources
	 */
	bool can_do_event(ResourceSet amount);

	void do_event();
	void set_event(std::function<bool()> e);

private:
	std::unordered_map<Resource *, double> resource;
	std::unordered_map<Resource *, bool> require_positive;
	std::function<bool()> act_func;

};

std::string to_string(Action &a);

}



