#pragma once

#include "eaalu_state.h"

class eaALUString : public eaALUState {
	public:
		eaValue* add(eaValue* argl, eaValue* argr);
		ezObject* compare(eaValue* argl, eaValue* argr);
		bool is_equal(eaValue* argl, eaValue* argr);
};
