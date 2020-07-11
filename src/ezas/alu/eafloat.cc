#include "eafloat.h"
#include "ezvm/ezvm.h"

eaValue* eaALUFloat::add(eaValue* argl, eaValue* argr) {
  return new ezFloat(argl->to_float() + argr->to_float());
}

eaValue* eaALUFloat::subtract(eaValue* argl, eaValue* argr) {
  return new ezFloat(argl->to_float() - argr->to_float());
}

eaValue* eaALUFloat::multiply(eaValue* argl, eaValue* argr) {
  return new ezFloat(argl->to_float() * argr->to_float());
}

eaValue* eaALUFloat::divide(eaValue* argl, eaValue* argr) {
  return new ezFloat(argl->to_float() / argr->to_float());
}

eaValue* eaALUFloat::power(eaValue* argl, eaValue* argr) {
  return new ezFloat(pow(argl->to_float(), argr->to_float()));
}

ezObject* eaALUFloat::compare(eaValue* argl, eaValue* argr) {
  double diff = argl->to_float() - argr->to_float();
  return new ezCondition(argl->id == EZ_VALUE_TYPE_FLOAT &&
		         argr->id == EZ_VALUE_TYPE_FLOAT &&
		         0 == diff,
                         (diff < 0) ? true : false, false, false);

}

bool eaALUFloat::is_equal(eaValue* argl, eaValue* argr) {
  if (EZ_VALUE_TYPE_FLOAT != argl->id || EZ_VALUE_TYPE_FLOAT != argr->id)
    return false;
  if (argl->to_float() != argr->to_float())
    return false;
  return true;
}

eaValue* eaALUFloat::negate(eaValue* arg) {
  return new ezFloat(-arg->to_float());
}


