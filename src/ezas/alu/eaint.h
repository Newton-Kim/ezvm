#pragma once

#include "eaalu_state.h"

class eaALUInteger : public eaALUState {
public:
  eaValue *add(eaValue *argl, eaValue *argr);
  eaValue *subtract(eaValue *argl, eaValue *argr);
  eaValue *multiply(eaValue *argl, eaValue *argr);
  eaValue *divide(eaValue *argl, eaValue *argr);
  eaValue *modulate(eaValue *argl, eaValue *argr);
};
