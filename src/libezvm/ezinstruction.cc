#include "ezvm/ezinstruction.h"

ezInstruction::ezInstruction(
    function<ezStepState(ezStackFrame &stk, ezInstruction &arg)> func,
    function<void(ezFile &sink, ezDump &dump, ezInstruction &arg)> dump)
    : m_func(func), m_dump(dump) {}
