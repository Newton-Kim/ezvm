#pragma once
#include "ezvm/ezalu.h"
#include "ezvm/ezinstruction.h"

class ezInstrSet : public ezInstruction {
private:
  ezALU *m_alu;
  ezAddress m_container;
  ezAddress m_member;
  ezAddress m_val;

public:
  ezInstrSet(ezALU *alu, const ezAddress &container, const ezAddress &member,
             const ezAddress val);
  void process(ezStackFrame &stk);
  void dump(ezFile &sink);
};

class ezInstrSetByArray : public ezInstruction {
private:
  ezALU *m_alu;
  ezAddress m_container;
  vector<ezAddress> m_member;
  ezAddress m_val;

public:
  ezInstrSetByArray(ezALU *alu, const ezAddress &container,
                    vector<ezAddress> &member, const ezAddress val);
  void process(ezStackFrame &stk);
  void dump(ezFile &sink);
};
