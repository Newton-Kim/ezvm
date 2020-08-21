#include "ezvm/ezalu.h"
#include "ezvm/ezfunc.h"
#include "ezvm/ezinstruction.h"

#include <functional>

using std::function;

class ezInstrConditionalBra : public ezInstruction {
private:
  function<bool (ezCondition *)> m_func;
  string m_name;
  ezAddress m_cond;
  size_t m_index;

public:
  ezInstrConditionalBra(
      const ezAddress &cond, const size_t index,
      const string name, function<bool (ezCondition *)> func);
  void process(ezStackFrame &stk);
  void dump(ezFile &sink);
};

