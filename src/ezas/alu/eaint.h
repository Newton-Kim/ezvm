#pragma once

#include "eaalu_state.h"

class eaALUInteger : public eaALUState {
	public:
		eaValue* add(eaValue* argl, eaValue* argr);
		eaValue* subtract(eaValue* argl, eaValue* argr);
		eaValue* multiply(eaValue* argl, eaValue* argr);
		eaValue* divide(eaValue* argl, eaValue* argr);
		eaValue* modulate(eaValue* argl, eaValue* argr);
		eaValue* power(eaValue* argl, eaValue* argr);
		eaValue* bitwise_and(eaValue* argl, eaValue* argr);
		eaValue* bitwise_or(eaValue* argl, eaValue* argr);
		eaValue* bitwise_xor(eaValue* argl, eaValue* argr);
		eaValue* lsl(eaValue* argl, eaValue* argr);
		eaValue* lsr(eaValue* argl, eaValue* argr);
		ezObject* compare(eaValue* argl, eaValue* argr);
		ezObject* condition(eaValue* arg);
		bool is_equal(eaValue* argl, eaValue* argr);
		eaValue* negate(eaValue* arg);
		eaValue* bitwise_not(eaValue* arg);
};


