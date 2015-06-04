#include <iostream>

#include <ActionOrder.h>
#include <Graph.h>

using namespace graph;

int main() {
	Graph *g = Graph::create({"A", "B", "C", "D"});

	g->set_win_func([g](ResourceSet r) {
		return r.get_amount(g->get_resource("A"));
	});

	// initial amounts
	g->get_resource("B")->adjust(3.0);

	// some actions
	g->add_action("1");
	Action *a = g->get_action("1");
	a->add_effect(g->get_resource("B"), -1.0, true);
	a->add_effect(g->get_resource("A"), 2.0);

	//g->add_action("2");
	//Action *b = g->get_action("2");
	//b->add_effect(g->get_resource("B"), 3.0);

	std::cout << to_string(*g) << std::endl;

	auto best = g->get_best_action();
	std::cout << to_string(best) << std::endl;
	std::cout << to_string(best.get_effect(g->get_amounts(), 55.0)) << "\n";

	return 0;
}