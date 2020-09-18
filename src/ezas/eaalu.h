#pragma once

#include "alu/eaalu_state.h"
#include "eaval.h"
#include "ezvm/ezvm.h"

class eaALU : public ezALU {
private:
  eaALUState *m_state[EZ_VALUE_TYPE_MAX];
  inline int max_id(eaValue *argl, eaValue *argr) {
    return (argl->id > argr->id) ? argl->id : argr->id;
  }

public:
  eaALU();
  ~eaALU();
  ezValue *add(ezValue *argl, ezValue *argr);
  ezValue *subtract(ezValue *argl, ezValue *argr);
  ezValue *multiply(ezValue *argl, ezValue *argr);
  ezValue *divide(ezValue *argl, ezValue *argr);
  ezValue *modulate(ezValue *argl, ezValue *argr);
  ezValue *bitwise_and(ezValue *argl, ezValue *argr);
  ezValue *bitwise_or(ezValue *argl, ezValue *argr);
  ezValue *bitwise_xor(ezValue *argl, ezValue *argr);
};
