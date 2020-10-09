#include "eaalu_state.h"

eaValue *eaALUState::add(eaValue *argl, eaValue *argr) {
  throw runtime_error("addition is not defined");
}

eaValue *eaALUState::subtract(eaValue *argl, eaValue *argr) {
  throw runtime_error("subtraction is not defined");
}

eaValue *eaALUState::multiply(eaValue *argl, eaValue *argr) {
  throw runtime_error("multiplication is not defined");
}

eaValue *eaALUState::divide(eaValue *argl, eaValue *argr) {
  throw runtime_error("devision is not defined");
}

eaValue *eaALUState::modulate(eaValue *argl, eaValue *argr) {
  throw runtime_error("modulation is not defined");
}

eaValue *eaALUState::power(eaValue *argl, eaValue *argr) {
  throw runtime_error("power operation is not defined");
}

eaValue *eaALUState::lsl(eaValue *argl, eaValue *argr) {
  throw runtime_error("lsl is not defined");
}

eaValue *eaALUState::lsr(eaValue *argl, eaValue *argr) {
  throw runtime_error("lsr is not defined");
}

ezObject *eaALUState::compare(eaValue *argl, eaValue *argr) {
  throw runtime_error("comparson is not defined");
}

ezObject *eaALUState::condition(eaValue *arg) {
  throw runtime_error("condition is not defined");
}

bool eaALUState::is_equal(eaValue *argl, eaValue *argr) {
  throw runtime_error("testing equality is not defined");
}
