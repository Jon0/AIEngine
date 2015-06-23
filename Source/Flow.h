#pragma once

#include "Edge.h"

namespace graph {

class Graph;
class Resource;

class Flow : public Edge {
public:
	Flow(Resource *t, Resource *f, double, double);
	~Flow();

	bool controlled() override;

	double get_time() override;

	ResourceSetDelta get_pre_effect() override;
	ResourceSetDelta get_post_effect() override;

	Resource *get_to();
	Resource *get_from();

	double get_in_flow();
	double get_out_flow();

	/**
	 * expected rate
	 */
	void set_rate(double d);

private:
	Resource *to;
	Resource *from;
	double rate;

	std::unordered_map<Resource *, double> effect;
};

}


