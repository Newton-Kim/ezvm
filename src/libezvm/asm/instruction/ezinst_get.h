#pragma once
#include "ezvm/ezalu.h"
#include "ezvm/ezinstruction.h"

class ezInstrGet : public ezInstruction {
private:
  ezAddress m_dest;
  ezAddress m_container;
  ezAddress m_member;

public:
  ezInstrGet(const ezAddress dest, const ezAddress &container,
             const ezAddress &member);
  void process(ezStackFrame &stk);
  void dump(ezFile &sink);
};

class ezInstrGetByArray : public ezInstruction {
private:
  ezAddress m_dest;
  ezAddress m_container;
  vector<ezAddress> m_member;

public:
  ezInstrGetByArray(const ezAddress dest, const ezAddress &container,
                    vector<ezAddress> &member);
  void process(ezStackFrame &stk);
  void dump(ezFile &sink);
};
