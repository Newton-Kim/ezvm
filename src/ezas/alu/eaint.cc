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

