#pragma once

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
	ResourceSet(std::unordered_map<Resource *, double> a);
	~ResourceSet();

	double get_amount(Resource *) const;

	std::unordered_map<Resource *, double> get_amounts() const;

	/**
	 * modifies the amount of a resource
	 */
	void adjust_amount(Resource *r, double a);

	ResourceSet operator+(const ResourceSet &other);

private:
	std::unordered_map<Resource *, double> amounts;

};

std::string to_string(graph::ResourceSet rs);

}


