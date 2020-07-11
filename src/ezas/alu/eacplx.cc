#include "eacplx.h"
#include "ezvm/ezvm.h"

eaValue* eaALUComplex::add(eaValue* argl, eaValue* argr) {
  return new ezComplex(argl->to_complex() + argr->to_complex());
}

eaValue* eaALUComplex::subtract(eaValue* argl, eaValue* argr) {
  return new ezComplex(argl->to_complex() - argr->to_complex());
}

eaValue* eaALUComplex::multiply(eaValue* argl, eaValue* argr) {
  return new ezComplex(argl->to_complex() * argr->to_complex());
}

eaValue* eaALUComplex::divide(eaValue* argl, eaValue* argr) {
  return new ezComplex(argl->to_complex() / argr->to_complex());
}

eaValue* eaALUComplex::power(eaValue* argl, eaValue* argr) {
  return new ezComplex(pow(argl->to_complex(), argr->to_complex()));
}

ezObject* eaALUComplex::compare(eaValue* argl, eaValue* argr) {
  return new ezCondition(argl->id == EZ_VALUE_TYPE_COMPLEX &&
			argr->id == EZ_VALUE_TYPE_COMPLEX &&
                             argl->to_complex() == argr->to_complex(),
                         false, false, false);
}

bool eaALUComplex::is_equal(eaValue* argl, eaValue* argr) {
  if (EZ_VALUE_TYPE_COMPLEX != argl->id || EZ_VALUE_TYPE_COMPLEX != argr->id)
    return false;
  if (argl->to_complex() != argr->to_complex())
    return false;
  return true;
}

eaValue* eaALUComplex::negate(eaValue* arg) {
	return new ezComplex(-arg->to_complex());
}


