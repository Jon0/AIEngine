#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Action.h"
#include "Resource.h"
#include "ResourceSet.h"

namespace graph {

class ActionOrder;
class Edge;
class Flow;

class Graph {
public:
	~Graph();

	/**
	 * returns a new instance
	 */
	static Graph *create(std::vector<std::string> resource_names);

	void update();

	// actions
	void add_action(std::string name);
	Action *get_action(std::string name);

	std::vector<Action *> get_action_list() const;

	// resources
	void add_resource(std::string name);
	Resource *get_resource(std::string name);

	/**
	 * list of resources ordered as constructed
	 */
	std::vector<Resource *> get_resource_list() const;
	ResourceSet get_amounts() const;

	/**
	 * function to evaluate win probability
	 */
	void set_win_func(std::function<double(ResourceSet)> f);

	double evaluate_win_func();
	double evaluate_win_func(ResourceSet);

	/**
	 * calculate the best sequence of actions
	 * TODO: make const
	 */
	ActionOrder get_best_action();

	std::vector<std::string> debug();

private:
	Graph();

	void update_actions();
	void register_flow(Flow *f);

	// lists ordered by construct order
	std::vector<Action *> order_act;
	std::vector<Resource *> order_res;
	std::vector<Edge *> edge;
	std::vector<Flow *> flow;

	std::unordered_map<std::string, std::unique_ptr<Action>> action;
	std::unordered_map<std::string, std::unique_ptr<Resource>> resource;

	std::function<double(ResourceSet)> win_func;

	friend class Resource;

};

std::string to_string(Graph &g);

}
