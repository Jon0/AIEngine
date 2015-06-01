#pragma once

#include <unordered_map>

namespace graph {

class Resource;

/**
 * General edge type, implemented as Action and Flow
 */
class Edge {
public:
	virtual ~Edge();

	/**
	 * actions are controlled
	 */
	virtual bool controlled() = 0;

	virtual double get_time() = 0;

	virtual std::unordered_map<Resource *, double> get_effects() = 0;

};

}
