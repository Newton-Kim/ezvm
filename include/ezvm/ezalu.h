#pragma once
#include "ezval.h"

class ezALU {
public:
  virtual ezValue *add(ezValue *argl, ezValue *argr);
  virtual ezValue *subtract(ezValue *argl, ezValue *argr);
  virtual ezValue *multiply(ezValue *argl, ezValue *argr);
  virtual ezValue *divide(ezValue *argl, ezValue *argr);
  virtual ezValue *modulate(ezValue *argl, ezValue *argr);
  virtual ezValue *power(ezValue *argl, ezValue *argr);
  virtual ezValue *bitwise_and(ezValue *argl, ezValue *argr);
  virtual ezValue *bitwise_or(ezValue *argl, ezValue *argr);
  virtual ezValue *bitwise_xor(ezValue *argl, ezValue *argr);
  virtual ezValue *lsl(ezValue *argl, ezValue *argr);
  virtual ezValue *lsr(ezValue *argl, ezValue *argr);
  virtual ezObject *compare(ezValue *argl, ezValue *argr);
  virtual ezObject *condition(ezValue *arg);
  virtual bool is_equal(ezValue *argl, ezValue *argr);
  virtual ezValue *negate(ezValue *arg);
  virtual ezValue *bitwise_not(ezValue *arg);
};

class ezALUImplementor {
public:
  static ezALUImplementor *instance(void);
  virtual void set_alu(ezALU *alu) = 0;
  virtual ezALU *get_alu(void) = 0;
};
