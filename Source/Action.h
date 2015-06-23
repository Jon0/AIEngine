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
	Action(std::string name, double time, const ResourceSetDelta &pre, const ResourceSetDelta &post);
	~Action();

	std::string get_name() const;

	bool controlled() override;

	double get_time() override;

	ResourceSetDelta get_pre_effect() override;
	ResourceSetDelta get_post_effect() override;

	void apply_start_effect(ResourceSet &r);
	void apply_end_effect(ResourceSet &r);

	/**
	 * can the action be used with these resources
	 */
	bool can_do_event(Graph *g, ResourceSet amount);

	/**
	 * modifies graph resource amounts
	 * if action is successful
	 */
	void do_event(Graph *g);
	void set_event(std::function<bool()> e);

private:
	// type key
	const std::string action_name;

	// type attributes
	const double action_time;
	const ResourceSetDelta pre_effect, post_effect;

	// apply action
	std::function<bool()> act_func;

};

std::string to_string(Action &a);

}



