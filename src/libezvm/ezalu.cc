#include "ezvm/ezalu.h"

ezValue *ezALU::add(ezValue *argl, ezValue *argr) {
  throw runtime_error("addition is not defined");
}

ezValue *ezALU::subtract(ezValue *argl, ezValue *argr) {
  throw runtime_error("subtraction is not defined");
}

ezValue *ezALU::multiply(ezValue *argl, ezValue *argr) {
  throw runtime_error("multiplication is not defined");
}

ezValue *ezALU::divide(ezValue *argl, ezValue *argr) {
  throw runtime_error("devision is not defined");
}

ezValue *ezALU::modulate(ezValue *argl, ezValue *argr) {
  throw runtime_error("modulation is not defined");
}

ezValue *ezALU::power(ezValue *argl, ezValue *argr) {
  throw runtime_error("power operation is not defined");
}

ezValue *ezALU::bitwise_and(ezValue *argl, ezValue *argr) {
  throw runtime_error("bitwise and is not defined");
}

ezValue *ezALU::bitwise_or(ezValue *argl, ezValue *argr) {
  throw runtime_error("bitwise or is not defined");
}

ezValue *ezALU::bitwise_xor(ezValue *argl, ezValue *argr) {
  throw runtime_error("bitwise xor is not defined");
}

ezValue *ezALU::lsl(ezValue *argl, ezValue *argr) {
  throw runtime_error("lsl is not defined");
}

ezValue *ezALU::lsr(ezValue *argl, ezValue *argr) {
  throw runtime_error("lsr is not defined");
}

ezObject *ezALU::compare(ezValue *argl, ezValue *argr) {
  throw runtime_error("comparson is not defined");
}

ezObject *ezALU::condition(ezValue *arg) {
  throw runtime_error("condition is not defined");
}

bool ezALU::is_equal(ezValue *argl, ezValue *argr) {
  throw runtime_error("testing equality is not defined");
}

ezValue *ezALU::negate(ezValue *arg) {
  throw runtime_error("negation is not defined");
}

ezValue *ezALU::bitwise_not(ezValue *arg) {
  throw runtime_error("bitwise not is not defined");
}

ezValue *ezALU::get(ezValue *ctn, ezValue *mbr) {
  throw runtime_error("get is not defined");
}

ezValue *ezALU::get(ezValue *ctn, vector<ezValue *> mbr) {
  throw runtime_error("get is not defined");
}

void ezALU::set(ezValue *ctn, ezValue *mbr, ezValue *val) {
  throw runtime_error("set is not defined");
}

void ezALU::set(ezValue *ctn, vector<ezValue *> mbr, ezValue *val) {
  throw runtime_error("set is not defined");
}
