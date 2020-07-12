#include "eabool.h"
#include "ezvm/ezvm.h"

ezObject* eaALUBool::compare(eaValue* argl, eaValue* argr) {
  return new ezCondition(!(argl->to_bool() ^ argr->to_bool()), false, false, false);
}

ezObject* eaALUBool::condition(eaValue* arg) {
  return new ezCondition(!arg->to_bool(), false, false, false);
}

bool eaALUBool::is_equal(eaValue* argl, eaValue* argr) {
  if (EZ_VALUE_TYPE_BOOL != argl->id || EZ_VALUE_TYPE_BOOL != argr->id)
    return false;
  if (argl->to_bool() != argr->to_bool())
    return false;
  return true;
}

eaValue* eaALUBool::bitwise_not(eaValue* arg) {
	return new ezBool(!arg->to_bool());
}


