#include "eaint.h"
#include "ezvm/ezvm.h"

eaValue *eaALUInteger::add(eaValue *argl, eaValue *argr) {
  return new ezInteger(argl->to_int() + argr->to_int());
}

eaValue *eaALUInteger::subtract(eaValue *argl, eaValue *argr) {
  return new ezInteger(argl->to_int() - argr->to_int());
}

eaValue *eaALUInteger::multiply(eaValue *argl, eaValue *argr) {
  return new ezInteger(argl->to_int() * argr->to_int());
}

eaValue *eaALUInteger::divide(eaValue *argl, eaValue *argr) {
  return new ezInteger(argl->to_int() / argr->to_int());
}

eaValue *eaALUInteger::modulate(eaValue *argl, eaValue *argr) {
  return new ezInteger(argl->to_int() % argr->to_int());
}

eaValue *eaALUInteger::power(eaValue *argl, eaValue *argr) {
  return new ezInteger(pow(argl->to_int(), argr->to_int()));
}

eaValue *eaALUInteger::bitwise_and(eaValue *argl, eaValue *argr) {
  return new ezInteger(argl->to_int() & argr->to_int());
}

eaValue *eaALUInteger::bitwise_or(eaValue *argl, eaValue *argr) {
  return new ezInteger(argl->to_int() | argr->to_int());
}

eaValue *eaALUInteger::bitwise_xor(eaValue *argl, eaValue *argr) {
  return new ezInteger(argl->to_int() ^ argr->to_int());
}

eaValue *eaALUInteger::lsl(eaValue *argl, eaValue *argr) {
  return new ezInteger(argl->to_int() << argr->to_int());
}

eaValue *eaALUInteger::lsr(eaValue *argl, eaValue *argr) {
  return new ezInteger(argl->to_int() >> argr->to_int());
}

ezObject *eaALUInteger::compare(eaValue *argl, eaValue *argr) {
  int diff = argl->to_int() - argr->to_int();
  return new ezCondition(argl->id == EZ_VALUE_TYPE_INTEGER &&
                             argr->id == EZ_VALUE_TYPE_INTEGER && 0 == diff,
                         (diff < 0) ? true : false, false, false);
}

ezObject *eaALUInteger::condition(eaValue *arg) {
  return new ezCondition(0 == arg->to_int(), (arg->to_int() < 0) ? true : false,
                         false, false);
}

bool eaALUInteger::is_equal(eaValue *argl, eaValue *argr) {
  if (EZ_VALUE_TYPE_INTEGER != argl->id || EZ_VALUE_TYPE_INTEGER != argr->id)
    return false;
  if (argl->to_int() != argr->to_int())
    return false;
  return true;
}

eaValue *eaALUInteger::negate(eaValue *arg) {
  return new ezInteger(-arg->to_int());
}

eaValue *eaALUInteger::bitwise_not(eaValue *arg) {
  return new ezInteger(~arg->to_int());
}
