#include "eastring.h"
#include "ezvm/ezvm.h"

eaValue *eaALUString::add(eaValue *argl, eaValue *argr) {
  return new ezString(argl->to_string() + argr->to_string());
}
