#include <algorithm>

#include "ActionProcess.h"
#include "Graph.h"

namespace graph {

ActionOrder ActionProcessV1::get_best_action(Graph *g) const {

	// current amounts
	// an optimised version could use a sorted list
	// of actions and only branch off the top portion

	ResourceSet current_amounts = g->get_amounts();
	double current_score = g->evaluate_win_func(current_amounts);

	std::vector<ActionOrder> orders;

	// add blank order
	orders.push_back(ActionOrder(g));

	//
	int max_iterations = 50;
	for (int t = 0; t < max_iterations; ++t) {
		for (int i = 0; i < 10; ++i) {

		// make new orders based of the existing list
		std::vector<ActionOrder> new_orders;
		for (auto &order : orders) {
			ResourceSet order_amounts = order.get_effect(current_amounts, t);


			// TODO many actions can occur in one frame

			// get available actions
			// at time within the orders context
			std::vector<Action *> available;
			for (auto &a : g->get_action_list()) {
				if (a->can_do_event(g, order_amounts)) {
					available.push_back(a);
				}
			}

			for (auto &a : available) {
				// add to the order
				// and add a new branch
				ActionOrder new_order = order;
				new_order.add_action(t, a);

				// TODO: divide by time taken
				double score = g->evaluate_win_func(order.get_effect(current_amounts)) - current_score;
				new_order.set_value(score);
				new_orders.push_back(new_order);
			}

		}
		for (auto &order : new_orders) {
			orders.push_back(order);
		}

		//sort and remove
		std::sort(orders.begin(), orders.end(), [](ActionOrder a, ActionOrder b){
			return a.value() > b.value();
		});
		if (orders.size() > 100) {
			orders.resize(100, ActionOrder(g));
		}
		}
	}

	// find the most valuable
	ActionOrder *best = &orders.front();
	double best_score = 0;
	for (auto &order : orders) {

		// TODO: divide by time taken
		double score = g->evaluate_win_func(order.get_effect(current_amounts)) - current_score;

		if (score > best_score) {
			best = &order;
			best_score = score;
		}
	}

	best->set_value(best_score);
	return *best;
}

}
