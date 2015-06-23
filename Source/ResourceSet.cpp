#include "Graph.h"
#include "ResourceSet.h"

namespace graph {

ResourceSet::ResourceSet(Graph *g, std::vector<double> a) {
	std::vector<Resource *> r = g->get_resource_list();
	if (r.size() == a.size()) {
		for (unsigned int i = 0; i < r.size(); ++i) {
			amounts.emplace(std::make_pair(r[i]->get_name(), a[i]));
		}
	}
}

ResourceSet::ResourceSet(std::unordered_map<std::string, double> a)
	:
	amounts(a) {
}

ResourceSet::~ResourceSet() {}

double ResourceSet::get_amount(Resource *r) const {
	std::string key = r->get_name();
	if (amounts.count(key) > 0) {
		return amounts.at(key);
	}
	return 0.0;
}

std::unordered_map<std::string, double> ResourceSet::get_amounts() const {
	return amounts;
}

std::unordered_map<Resource *, double> ResourceSet::get_amounts(Graph *g) const {
	std::unordered_map<Resource *, double> result;
	for (auto &a : amounts) {
		result.emplace(std::make_pair(g->get_resource(a.first), a.second));
	}
	return result;
}

void ResourceSet::adjust_amount(std::string key, double a) {
	if (amounts.count(key) > 0) {
		amounts[key] += a;
	}
}

void ResourceSet::adjust_amount(Resource *r, double a) {
	adjust_amount(r->get_name(), a);
}

ResourceSet ResourceSet::operator+(const ResourceSet &other) {
	std::unordered_map<std::string, double> result = other.get_amounts();
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

ResourceSetDelta::ResourceSetDelta() {}

ResourceSetDelta::ResourceSetDelta(std::unordered_map<std::string, double> d)
	:
	deltas(d) {
}

double ResourceSetDelta::get_delta(std::string key) const {
	if (deltas.count(key) > 0) {
		return deltas.at(key);
	}
	return 0.0;
}

double ResourceSetDelta::get_delta(Resource *key) const {
	return get_delta(key->get_name());
}

std::unordered_map<std::string, double> ResourceSetDelta::get_deltas() const {
	return deltas;
}

std::unordered_map<Resource *, double> ResourceSetDelta::get_deltas(Graph *g) const {
	std::unordered_map<Resource *, double> result;
	for (auto &d : deltas) {
		result.emplace(std::make_pair(g->get_resource(d.first), d.second));
	}
	return result;
}

std::string to_string(graph::ResourceSet rs) {
	std::string result = "[";
	for (auto &a : rs.get_amounts()) {
		result += a.first + " : " + std::to_string(a.second) + " ";
	}
	result += "]";
	return result;
}

std::string to_string(graph::ResourceSetDelta rsd) {
	std::string result = "[";
	for (auto &a : rsd.get_deltas()) {
		result += a.first + " : " + std::to_string(a.second) + " ";
	}
	result += "]";
	return result;
}

}
