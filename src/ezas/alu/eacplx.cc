#include "eacplx.h"
#include "ezvm/ezvm.h"

eaValue *eaALUComplex::add(eaValue *argl, eaValue *argr) {
  return new ezComplex(argl->to_complex() + argr->to_complex());
}

eaValue *eaALUComplex::subtract(eaValue *argl, eaValue *argr) {
  return new ezComplex(argl->to_complex() - argr->to_complex());
}

eaValue *eaALUComplex::multiply(eaValue *argl, eaValue *argr) {
  return new ezComplex(argl->to_complex() * argr->to_complex());
}

eaValue *eaALUComplex::divide(eaValue *argl, eaValue *argr) {
  return new ezComplex(argl->to_complex() / argr->to_complex());
}
