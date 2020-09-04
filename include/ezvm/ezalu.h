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
  virtual ezValue *get(ezValue *ctn, ezValue *mbr);
  virtual ezValue *get(ezValue *ctn, vector<ezValue *> mbr);
  virtual void set(ezValue *ctn, ezValue *mbr, ezValue *val);
  virtual void set(ezValue *ctn, vector<ezValue *> mbr, ezValue *val);
};
