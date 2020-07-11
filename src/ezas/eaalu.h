#pragma once

#include "ezvm/ezvm.h"
#include "eaval.h"
#include "eaalu_state.h"

class eaALU : public ezALU {
	private:
		eaALUState* m_state[EZ_VALUE_TYPE_MAX];
		inline int max_id(eaValue* argl, eaValue* argr) {
			return (argl->id > argr->id) ? argl->id : argr->id;
		}
	public:
		eaALU();
		~eaALU();
		ezValue* add(ezValue* argl, ezValue* argr);
		ezValue* subtract(ezValue* argl, ezValue* argr);
		ezValue* multiply(ezValue* argl, ezValue* argr);
		ezValue* divide(ezValue* argl, ezValue* argr);
		ezValue* modulate(ezValue* argl, ezValue* argr);
		ezValue* power(ezValue* argl, ezValue* argr);
		ezValue* bitwise_and(ezValue* argl, ezValue* argr);
		ezValue* bitwise_or(ezValue* argl, ezValue* argr);
		ezValue* bitwise_xor(ezValue* argl, ezValue* argr);
		ezValue* lsl(ezValue* argl, ezValue* argr);
		ezValue* lsr(ezValue* argl, ezValue* argr);
		ezObject* compare(ezValue* argl, ezValue* argr);
		bool is_equal(ezValue* argl, ezValue* argr);
		ezValue* negate(ezValue* arg);
		ezValue* bitwise_not(ezValue* arg);
};