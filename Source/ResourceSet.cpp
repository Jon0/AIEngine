#include "Graph.h"
#include "ResourceSet.h"

namespace graph {

ResourceSet::ResourceSet(Graph *g, std::vector<double> a) {
	std::vector<Resource *> r = g->get_resource_list();
	if (r.size() == a.size()) {
		for (unsigned int i = 0; i < r.size(); ++i) {
			amounts.emplace(std::make_pair(r[i], a[i]));
		}
	}
}

ResourceSet::ResourceSet(std::unordered_map<Resource *, double> a)
	:
	amounts(a) {
}

ResourceSet::~ResourceSet() {}

double ResourceSet::get_amount(Resource *r) const {
	if (amounts.count(r) > 0) {
		return amounts.at(r);
	}
	return 0.0;
}

std::unordered_map<Resource *, double> ResourceSet::get_amounts() const {
	return amounts;
}


void ResourceSet::adjust_amount(Resource *r, double a) {
	if (amounts.count(r) > 0) {
		amounts[r] += a;
	}
}

ResourceSet ResourceSet::operator+(const ResourceSet &other) {
	std::unordered_map<Resource *, double> result = other.get_amounts();
	for (auto &a : amounts) {
		if (result.count(a.first) == 0) {
			result.emplace(a);
		}
		else {
			result[a.first] += a.second;
		}
	}
	return ResourceSet(result);
}

std::string to_string(graph::ResourceSet rs) {
	return "todo resource set";
}

}
