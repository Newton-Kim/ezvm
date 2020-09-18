#include "eafloat.h"
#include "ezvm/ezvm.h"

eaValue *eaALUFloat::add(eaValue *argl, eaValue *argr) {
  return new ezFloat(argl->to_float() + argr->to_float());
}

eaValue *eaALUFloat::subtract(eaValue *argl, eaValue *argr) {
  return new ezFloat(argl->to_float() - argr->to_float());
}

eaValue *eaALUFloat::multiply(eaValue *argl, eaValue *argr) {
  return new ezFloat(argl->to_float() * argr->to_float());
}

eaValue *eaALUFloat::divide(eaValue *argl, eaValue *argr) {
  return new ezFloat(argl->to_float() / argr->to_float());
}
