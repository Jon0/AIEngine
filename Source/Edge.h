#pragma once

#include <unordered_map>

#include "ResourceSet.h"

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

	virtual ResourceSetDelta get_pre_effect() = 0;
	virtual ResourceSetDelta get_post_effect() = 0;

};

}
