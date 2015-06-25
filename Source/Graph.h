#pragma once

#include <list>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Action.h"
#include "Location.h"
#include "Resource.h"
#include "ResourceSet.h"

namespace graph {

class ActionOrder;
class Edge;
class Flow;

/**
 * TODO type players
 */
class GraphProducer {
public:
	GraphProducer();

	void add_action(const Action &);
	void add_location(const Location &);
	void add_resource(const Resource &);

	std::vector<Action> get_actions() const;
	std::vector<Location> get_locations() const;
	std::vector<Resource> get_resources() const;

private:
	std::vector<int> players;
	std::vector<Action> actions;
	std::vector<Location> locations;
	std::vector<Resource> resources;

};

class Graph {
public:
	Graph(const GraphProducer &);
	~Graph();

	void update();

	// actions
	Action *get_action(std::string name);

	std::vector<Action *> get_action_list();

	// resources
	Resource *get_resource(std::string name);

	/**
	 * list of resources ordered as constructed
	 */
	std::vector<Resource *> get_resource_list();
	ResourceSet get_amounts() const;

	/**
	 * function to evaluate win probability
	 */
	void set_win_func(std::function<double(const ResourceSet &)> f);

	double evaluate_win_func() const;
	double evaluate_win_func(const ResourceSet &) const;

private:
	void add_action(const Action &);
	Flow *add_flow(const Flow &);
	void add_resource(const Resource &);

	double default_win_func(const ResourceSet &) const;

	// lists ordered by construct order
	// pointers to items must remain so containers
	// which reallocate will not work
	std::list<Action> order_act;
	std::list<Flow> order_flow;
	std::list<Resource> order_res;

	// combined set of actions and flows
	std::vector<Edge *> order_edge;

	std::unordered_map<std::string, Action *> action;
	std::unordered_map<std::string, Resource *> resource;

	std::function<double(const ResourceSet &)> win_func;

	friend class Resource;

};

std::string to_string(Graph &g);

} // namespace graph
