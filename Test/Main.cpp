#include <iostream>

#include <ActionOrder.h>
#include <ActionProcess.h>
#include <Graph.h>

using namespace graph;

int main() {
	std::cout << "starting test\n";

	GraphProducer producer;
	producer.add_resource(Resource("Mineral"));
	producer.add_resource(Resource("Worker"));

	// some actions
	ResourceSetDelta pre({{"Worker", -1.0}});
	ResourceSetDelta post({{"Worker", 1.0}, {"Mineral", 10.0}});
	producer.add_action(Action("Mine", 1.0, pre, post));

	ResourceSetDelta pre1({{"Mineral", -50.0}});
	ResourceSetDelta post1({{"Worker", 1.0}});
	producer.add_action(Action("Train", 1.0, pre1, post1));

	// construct graph
	Graph g(producer);
	g.set_win_func([&g](const ResourceSet &r) {
		return r.get_amount(g.get_resource("Worker"));
	});

	// initial amounts
	g.get_resource("Worker")->adjust(6.0);
	std::cout << to_string(g) << std::endl;

	ActionProcessV1 p;
	ActionOrder best = p.get_best_action(&g);

	std::cout << "best action:\n";
	std::cout << best.effect_str(&g) << std::endl;
	//std::cout << to_string(best.get_effect(g.get_amounts(), 55.0)) << "\n";

	return 0;
}