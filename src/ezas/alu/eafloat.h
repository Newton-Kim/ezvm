#pragma once

#include "eaalu_state.h"

class eaALUFloat : public eaALUState {
	public:
		eaValue* add(eaValue* argl, eaValue* argr);
		eaValue* subtract(eaValue* argl, eaValue* argr);
		eaValue* multiply(eaValue* argl, eaValue* argr);
		eaValue* divide(eaValue* argl, eaValue* argr);
		eaValue* power(eaValue* argl, eaValue* argr);
		ezObject* compare(eaValue* argl, eaValue* argr);
		bool is_equal(eaValue* argl, eaValue* argr);
		eaValue* negate(eaValue* arg);
};

