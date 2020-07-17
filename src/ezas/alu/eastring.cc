#include "eastring.h"
#include "ezvm/ezvm.h"

eaValue *eaALUString::add(eaValue *argl, eaValue *argr) {
  return new ezString(argl->to_string() + argr->to_string());
}

ezObject *eaALUString::compare(eaValue *argl, eaValue *argr) {
  return new ezCondition(argl->id == EZ_VALUE_TYPE_STRING &&
                             argr->id == EZ_VALUE_TYPE_STRING &&
                             argl->to_string() == argr->to_string(),
                         false, false, false);
}

ezObject *eaALUString::condition(eaValue *arg) {
  return new ezCondition(arg->to_string().empty(), false, false, false);
}

bool eaALUString::is_equal(eaValue *argl, eaValue *argr) {
  if (EZ_VALUE_TYPE_STRING != argl->id || EZ_VALUE_TYPE_STRING != argr->id)
    return false;
  if (argl->to_string() != argr->to_string())
    return false;
  return true;
}
