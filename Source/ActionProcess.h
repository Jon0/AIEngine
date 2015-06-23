#pragma once

#include "ActionOrder.h"

namespace graph {

/**
 * implemented by various algorithms
 */
class ActionProcess {
public:

	/**
	 * calculate the best sequence of actions
	 */
	virtual ActionOrder get_best_action(Graph *g) const = 0;
};

class ActionProcessV1 : public ActionProcess {
public:
	ActionOrder get_best_action(Graph *g) const override;
};

}
