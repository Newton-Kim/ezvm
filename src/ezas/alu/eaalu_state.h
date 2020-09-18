#pragma once

#include "eaval.h"

class eaALUState {
public:
  virtual eaValue *add(eaValue *argl, eaValue *argr);
  virtual eaValue *subtract(eaValue *argl, eaValue *argr);
  virtual eaValue *multiply(eaValue *argl, eaValue *argr);
  virtual eaValue *divide(eaValue *argl, eaValue *argr);
  virtual eaValue *modulate(eaValue *argl, eaValue *argr);
  virtual eaValue *power(eaValue *argl, eaValue *argr);
  virtual eaValue *bitwise_and(eaValue *argl, eaValue *argr);
  virtual eaValue *bitwise_or(eaValue *argl, eaValue *argr);
  virtual eaValue *bitwise_xor(eaValue *argl, eaValue *argr);
  virtual eaValue *lsl(eaValue *argl, eaValue *argr);
  virtual eaValue *lsr(eaValue *argl, eaValue *argr);
  virtual ezObject *compare(eaValue *argl, eaValue *argr);
  virtual ezObject *condition(eaValue *arg);
  virtual bool is_equal(eaValue *argl, eaValue *argr);
};
