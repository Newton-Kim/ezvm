#pragma once
#include "ezval.h"

class ezALU {
public:
  virtual ezValue *add(ezValue *argl, ezValue *argr);
  virtual ezValue *subtract(ezValue *argl, ezValue *argr);
  virtual ezValue *multiply(ezValue *argl, ezValue *argr);
  virtual ezValue *divide(ezValue *argl, ezValue *argr);
  virtual ezValue *modulate(ezValue *argl, ezValue *argr);
};
