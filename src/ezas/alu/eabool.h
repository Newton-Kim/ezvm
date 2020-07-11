#pragma once

#include "eaalu_state.h"

class eaALUBool : public eaALUState {
	public:
		ezObject* compare(eaValue* argl, eaValue* argr);
		bool is_equal(eaValue* argl, eaValue* argr);
		eaValue* bitwise_not(eaValue* arg);
};

