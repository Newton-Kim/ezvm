#include "eaalu_state.h"

eaValue* eaALUState::add(eaValue* argl, eaValue* argr) {
	throw runtime_error("addition is not defined");
}

eaValue* eaALUState::subtract(eaValue* argl, eaValue* argr) {
	throw runtime_error("subtraction is not defined");
}

eaValue* eaALUState::multiply(eaValue* argl, eaValue* argr) {
	throw runtime_error("multiplication is not defined");
}

eaValue* eaALUState::divide(eaValue* argl, eaValue* argr) {
	throw runtime_error("devision is not defined");
}

eaValue* eaALUState::modulate(eaValue* argl, eaValue* argr) {
	throw runtime_error("modulation is not defined");
}

eaValue* eaALUState::power(eaValue* argl, eaValue* argr) {
	throw runtime_error("power operation is not defined");
}

eaValue* eaALUState::bitwise_and(eaValue* argl, eaValue* argr) {
	throw runtime_error("bitwise and is not defined");
}

eaValue* eaALUState::bitwise_or(eaValue* argl, eaValue* argr) {
	throw runtime_error("bitwise or is not defined");
}

eaValue* eaALUState::bitwise_xor(eaValue* argl, eaValue* argr) {
	throw runtime_error("bitwise xor is not defined");
}

eaValue* eaALUState::lsl(eaValue* argl, eaValue* argr) {
	throw runtime_error("lsl is not defined");
}

eaValue* eaALUState::lsr(eaValue* argl, eaValue* argr) {
	throw runtime_error("lsr is not defined");
}

ezObject* eaALUState::compare(eaValue* argl, eaValue* argr) {
	throw runtime_error("comparson is not defined");
}

ezObject* eaALUState::condition(eaValue* arg) {
	throw runtime_error("condition is not defined");
}

bool eaALUState::is_equal(eaValue* argl, eaValue* argr) {
	throw runtime_error("testing equality is not defined");
}

eaValue* eaALUState::negate(eaValue* arg) {
	throw runtime_error("negation is not defined");
}

eaValue* eaALUState::bitwise_not(eaValue* arg) {
	throw runtime_error("bitwise not is not defined");
}

