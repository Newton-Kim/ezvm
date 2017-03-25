#include "ezvm/ezinstruction.h"

ezInstruction::ezInstruction() {}

ezInstruction::ezInstruction(function<void(ezStackFrame* stk, ezInstruction& arg)> func):cmd(EZ_OP_AUTO), m_func(func) {}
