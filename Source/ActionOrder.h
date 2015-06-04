#pragma once

#include <unordered_map>
#include <vector>

#include "ResourceSet.h"

namespace graph {

class Action;
class Graph;
class Resource;

bool compare(std::pair<double, Action *> a, std::pair<double, Action *> b);

/**
 * An order actions
 */
class ActionOrder
{
public:
	ActionOrder(Graph *gr);
	~ActionOrder();

	/**
	 * time when the last action gets completed
	 */
	double length();

	double value();

	void set_value(double v);

	void add_action(double time, Action *a);

	std::pair<double, Action *> first_action();

	std::vector<std::pair<double, Action *>> get_actions();

	/**
	 * effect if applied to current resources
	 */
	ResourceSet get_effect();

	/**
	 * effect if applied to given resource set
	 */
	ResourceSet get_effect(ResourceSet a);

	/**
	 * effect if applied to given resource set up to a set time
	 */
	ResourceSet get_effect(ResourceSet a, double time_duration);

private:
	Graph *g;
	double total_time, graph_value;

	// time and action pairs, ordered by times
	std::vector<std::pair<double, Action *>> actions;

};

std::string to_string(ActionOrder &o);

}

