#include "ezvm/ezinstruction.h"

ezInstruction::ezInstruction():auto_cmd(false) {}

ezInstruction::ezInstruction(function<void(ezStackFrame* stk, ezInstruction& arg)> func):auto_cmd(true), m_func(func) {}
