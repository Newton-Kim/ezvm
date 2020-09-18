#pragma once

#include "eaalu_state.h"

class eaALUString : public eaALUState {
public:
  eaValue *add(eaValue *argl, eaValue *argr);
};
