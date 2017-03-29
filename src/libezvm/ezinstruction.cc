#include "ezvm/ezinstruction.h"

ezInstruction::ezInstruction() : auto_cmd(false) {}

ezInstruction::ezInstruction(
    function<ezStepState(ezStackFrame &stk, ezInstruction &arg)> func,
    function<void(ezFile &sink, ezDump &dump, ezInstruction &arg)> dump)
    : auto_cmd(true), m_func(func), m_dump(dump) {}
