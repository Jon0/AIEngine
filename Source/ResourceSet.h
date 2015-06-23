#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace graph {

class Graph;
class Resource;

/**
 * a vector of resource and amount pairs
 */
class ResourceSet {
public:
	ResourceSet(Graph *g, std::vector<double> a);
	ResourceSet(std::unordered_map<std::string, double> a);
	~ResourceSet();

	double get_amount(Resource *) const;

	std::unordered_map<std::string, double> get_amounts() const;
	std::unordered_map<Resource *, double> get_amounts(Graph *g) const;

	/**
	 * modifies the amount of a resource
	 */
	void adjust_amount(std::string key, double a);
	void adjust_amount(Resource *r, double a);

	ResourceSet operator+(const ResourceSet &other);

private:
	std::unordered_map<std::string, double> amounts;

};

class ResourceSetDelta {
public:
	ResourceSetDelta();
	ResourceSetDelta(std::unordered_map<std::string, double> d);

	double get_delta(std::string key) const;
	double get_delta(Resource *key) const;

	/**
	 * delta of each resource
	 */
	std::unordered_map<std::string, double> get_deltas() const;
	std::unordered_map<Resource *, double> get_deltas(Graph *g) const;

private:
	const std::unordered_map<std::string, double> deltas;
};

std::string to_string(graph::ResourceSet rs);
std::string to_string(graph::ResourceSetDelta rsd);

}


