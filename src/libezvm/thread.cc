/* Copyright (C) 2015 Newton Kim
*
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditiong:
*
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*
*/
#include "ezvm/ezlog.h"
#include "ezvm/ezthread.h"
#include "ezvm/ezinstruction.h"
#include <stdexcept>

typedef void (*RUNFUNC)(ezThread&, uint8_t, uint8_t, uint8_t);

static void run_mv(ezThread& thd, uint8_t arg1, uint8_t arg2, uint8_t arg3) {
  thd.mv(arg1, arg2);
}

static void run_ld(ezThread& thd, uint8_t arg1, uint8_t arg2, uint8_t arg3) {
  thd.ld();
}

static void run_call(ezThread& thd, uint8_t arg1, uint8_t arg2, uint8_t arg3) {
  thd.call(arg1, arg2);
}

static void run_cmp(ezThread& thd, uint8_t arg1, uint8_t arg2, uint8_t arg3) {
  thd.cmp(arg1, arg2);
}

static void run_div(ezThread& thd, uint8_t arg1, uint8_t arg2, uint8_t arg3) {
  thd.div(arg1, arg2);
}

static void run_add(ezThread& thd, uint8_t arg1, uint8_t arg2, uint8_t arg3) {
  thd.add(arg1, arg2);
}

static void run_and(ezThread& thd, uint8_t arg1, uint8_t arg2, uint8_t arg3) {
  thd.bitwise_and(arg1, arg2);
}

static void run_mod(ezThread& thd, uint8_t arg1, uint8_t arg2, uint8_t arg3) {
  thd.mod(arg1, arg2);
}

static void run_mul(ezThread& thd, uint8_t arg1, uint8_t arg2, uint8_t arg3) {
  thd.mul(arg1, arg2);
}

static void run_neg(ezThread& thd, uint8_t arg1, uint8_t arg2, uint8_t arg3) {
  thd.neg(arg1, arg2);
}

static void run_not(ezThread& thd, uint8_t arg1, uint8_t arg2, uint8_t arg3) {
  thd.bitwise_not(arg1, arg2);
}

static void run_or(ezThread& thd, uint8_t arg1, uint8_t arg2, uint8_t arg3) {
  thd.bitwise_or(arg1, arg2);
}

static void run_sub(ezThread& thd, uint8_t arg1, uint8_t arg2, uint8_t arg3) {
  thd.sub(arg1, arg2);
}

static void run_xor(ezThread& thd, uint8_t arg1, uint8_t arg2, uint8_t arg3) {
  thd.bitwise_xor(arg1, arg2);
}

static void run_beq(ezThread& thd, uint8_t arg1, uint8_t arg2, uint8_t arg3) {
  thd.beq(arg1);
}

static void run_bge(ezThread& thd, uint8_t arg1, uint8_t arg2, uint8_t arg3) {
  thd.bge(arg1);
}

static void run_blt(ezThread& thd, uint8_t arg1, uint8_t arg2, uint8_t arg3) {
  thd.blt(arg1);
}

static void run_bne(ezThread& thd, uint8_t arg1, uint8_t arg2, uint8_t arg3) {
  thd.bne(arg1);
}

static void run_bra(ezThread& thd, uint8_t arg1, uint8_t arg2, uint8_t arg3) {
  thd.bra(arg1);
}

static RUNFUNC s_run[] = {run_add,  run_and, run_beq, run_bge, run_blt, run_bne, run_bra,
                   run_call, run_cmp, run_div, run_ld, run_mod, run_mul, run_mv,
                   run_neg,  run_not, run_or,  run_sub, run_xor};

ezThread::ezThread(ezAddress entry, vector<vector<ezValue*>*>& globals,
                   vector<ezValue*>& constants)
    : m_entry(entry), m_constants(constants), m_globals(globals) {
  ezValue* v = addr2val(entry);
  switch (v->type) {
    case EZ_VALUE_TYPE_CAROUSEL: {
      ezCarousel* crsl = (ezCarousel*)v;
      ezStackFrame* sf = new ezStackFrame(crsl);
      for (size_t i = 0; i < crsl->nmems; i++)
        sf->local.push_back(ezNull::instance());
      m_stack.push(sf);
    } break;
    case EZ_VALUE_TYPE_NATIVE_CAROUSEL:
      ((ezNativeCarousel*)v)->run(m_args, m_rets);
      break;
    default:
      throw("invalid value type");
      break;
  }
}

ezThread::~ezThread() {
  while (!m_stack.empty()) {
    ezStackFrame* sf = m_stack.top();
    if (sf) delete sf;
    m_stack.pop();
  }
}

ezStepState ezThread::step(void) {
  // It needs 3 steps to finalize a thread
  if (m_stack.empty()) return EZ_STEP_DONE;
  ezStackFrame* sf = m_stack.top();
  ezLog& log = ezLog::instance();
  if (sf->pc >= sf->carousel->instruction.size()) {
    log.verbose("stack %p has poped out", sf);
    m_stack.pop();
    size_t rets = sf->returns.size();
    size_t dests = sf->return_dest.size();
    size_t cnt = (rets > dests) ? dests : rets;
    for (size_t i = 0; i < cnt; i++)
      val2addr(sf->return_dest[i], sf->returns[i]);
    if (dests > rets) {
      for (size_t i = cnt; i < dests; i++)
        val2addr(sf->return_dest[i], ezNull::instance());
    }
    delete sf;
    return EZ_STEP_CONTINUE;
  }
  log.verbose("stack %p has turn", sf);
  ezInstDecoder decoder;
  ezOpCode op;
  uint8_t arg1, arg2, arg3;
  decoder.opcode(sf->carousel->instruction[sf->pc++], op, arg1, arg2, arg3);
  s_run[op](*this, arg1, arg2, arg3);
  return EZ_STEP_CONTINUE;
}

void ezThread::mv(uint8_t ndests, uint8_t nsrcs) {
  // TODO it can be done via a macro
  if (m_stack.empty()) throw runtime_error("stack underrun");
  ezStackFrame* sf = m_stack.top();
  size_t cnt = (ndests > nsrcs) ? nsrcs : ndests;
  ezInstDecoder decoder;
  ezAddress src_addr;
  ezAddress dest_addr;
  ezValue* v = NULL;
  size_t i = 0;
  vector<ezValue*> q;
  for (i = 0; i < cnt; i++) {
    decoder.argument(sf->carousel->instruction[sf->pc + i + ndests], src_addr);
    v = addr2val(src_addr);
    q.push_back(v);
  }
  for (i = 0; i < cnt; i++) {
    decoder.argument(sf->carousel->instruction[sf->pc + i], dest_addr);
    v = addr2val(dest_addr);
    v = q[i];
    val2addr(dest_addr, v);
  }
  if (ndests > nsrcs) {
    for (i = cnt; i < ndests; i++) {
      decoder.argument(sf->carousel->instruction[sf->pc + cnt + i], dest_addr);
      val2addr(dest_addr, ezNull::instance());
    }
  }
  sf->pc += (ndests + nsrcs);
}

void ezThread::ld(void) {
  // TODO it can be done via a macro
  if (m_stack.empty()) throw runtime_error("stack underrun");
  ezStackFrame* sf = m_stack.top();
  sf->pc += 3;
}

void ezThread::call(uint8_t nargs, uint8_t nrets) {
  // TODO it can be done via a macro
  if (m_stack.empty()) throw runtime_error("stack underrun");
  ezStackFrame* sf = m_stack.top();
  ezInstDecoder decoder;
  ezAddress addr;
  decoder.argument(sf->carousel->instruction[sf->pc++], addr);
  ezValue* func = addr2val(addr);
  switch (func->type) {
    case EZ_VALUE_TYPE_NATIVE_CAROUSEL:
      call((ezNativeCarousel*)func, nargs, nrets);
      break;
    case EZ_VALUE_TYPE_CAROUSEL:
      call((ezCarousel*)func, nargs, nrets);
      break;
    default:
      throw runtime_error("function is not executable");
      break;
  }
}

void ezThread::call(ezNativeCarousel* func, uint8_t nargs, uint8_t nrets) {
  ezStackFrame* sf = m_stack.top();
  ezInstDecoder decoder;
  vector<ezValue*> args;
  ezAddress addr;
  for (size_t i = 0; i < nargs; i++, sf->pc++) {
    decoder.argument(sf->carousel->instruction[sf->pc], addr);
    ezValue* v = addr2val(addr);
    args.push_back(v);
  }
  vector<ezAddress> ret_dest;
  for (size_t i = 0; i < nrets; i++, sf->pc++) {
    decoder.argument(sf->carousel->instruction[sf->pc], addr);
    ret_dest.push_back(addr);
  }
  vector<ezValue*> rets;
  func->run(args, rets);
  if (rets.size()) val2addr(ret_dest, rets);
}

void ezThread::call(ezCarousel* func, uint8_t nargs, uint8_t nrets) {
  ezStackFrame* nsf = new ezStackFrame(func);
  ezStackFrame* sf = m_stack.top();
  ezInstDecoder decoder;
  vector<ezValue*> args;
  ezAddress addr;
  size_t min_args = (func->nargs > nargs) ? nargs : func->nargs;
  for (size_t i = 0; i < min_args; i++, sf->pc++) {
    decoder.argument(sf->carousel->instruction[sf->pc], addr);
    ezValue* v = addr2val(addr);
    v->reference();
    nsf->local.push_back(v);
  }
  if (func->nargs > nargs) {
    for (size_t i = min_args; i < func->nargs; i++, sf->pc++) {
      nsf->local.push_back(ezNull::instance());
    }
  }
  for (size_t i = 0; i < func->nmems; i++)
    nsf->local.push_back(ezNull::instance());
  vector<ezAddress> ret_dest;
  for (size_t i = 0; i < nrets; i++, sf->pc++) {
    decoder.argument(sf->carousel->instruction[sf->pc], addr);
    nsf->return_dest.push_back(addr);
  }
  m_stack.push(nsf);
}

void ezThread::cmp(uint8_t ndests, uint8_t nsrcs) {
  ezStackFrame* sf = m_stack.top();
  ezInstDecoder decoder;
  ezAddress addr, cond;
  ezValue* rst = NULL;
  switch (ndests) {
    case 1:
      decoder.argument(sf->carousel->instruction[sf->pc++], cond);
      break;
    default:
      throw runtime_error("the destination of CMP must be 1");
      break;
  }
  switch (nsrcs) {
    case 1:
      break;
    case 2: {
      ezValue* vr = NULL, *vl = NULL;
      decoder.argument(sf->carousel->instruction[sf->pc++], addr);
      vl = addr2val(addr);
      decoder.argument(sf->carousel->instruction[sf->pc++], addr);
      vr = addr2val(addr);
      rst = m_alu.cmp(vl, vr);
    } break;
      throw runtime_error("the number of ADD operands must be 2 or more");
      break;
  }
  val2addr(cond, rst);
}

void ezThread::binary_operation(uint8_t ndests, uint8_t nsrcs, function<ezValue*(ezValue*,ezValue*)> binary_func, function<ezValue*(vector<ezValue*>&)> multi_func) {
  ezStackFrame* sf = m_stack.top();
  ezInstDecoder decoder;
  ezAddress dest, addr, cond;
  decoder.argument(sf->carousel->instruction[sf->pc++], dest);
  ezValue* rst = NULL;
  switch (ndests) {
    case 2:
      decoder.argument(sf->carousel->instruction[sf->pc++], cond);
    case 1:
      break;
    default:
      throw runtime_error("the destination of ADD must be 1 or 2");
      break;
  }
  switch (nsrcs) {
    case 1:
      throw runtime_error("the number of ADD operands must be 2 or more");
      break;
    case 2: {
      ezValue* vr = NULL, *vl = NULL;
      decoder.argument(sf->carousel->instruction[sf->pc++], addr);
      vl = addr2val(addr);
      decoder.argument(sf->carousel->instruction[sf->pc++], addr);
      vr = addr2val(addr);
      rst = binary_func(vl, vr);
    } break;
    default: {
      ezValue* v = NULL;
      vector<ezValue*> args;
      for (size_t i = 0; i < nsrcs; i++) {
        decoder.argument(sf->carousel->instruction[sf->pc++], addr);
        v = addr2val(addr);
        args.push_back(v);
      }
      rst = multi_func(args);
    } break;
  }
  val2addr(dest, rst);
  if (ndests == 2) val2addr(cond, rst->condition());
}

void ezThread::add(uint8_t ndests, uint8_t nsrcs) {
  binary_operation(ndests, nsrcs, [&](ezValue* vl, ezValue* vr) {return m_alu.add(vl, vr);}, [&](vector<ezValue*>& args) {return m_alu.add(args);});
}

void ezThread::bitwise_and(uint8_t ndests, uint8_t nsrcs) {
  binary_operation(ndests, nsrcs, [&](ezValue* vl, ezValue* vr) {return m_alu.bitwise_and(vl, vr);}, [&](vector<ezValue*>& args) {return m_alu.bitwise_and(args);});
}

void ezThread::unary_operation(uint8_t ndests, uint8_t nsrcs, function<ezValue*(ezValue*)> unary_func) {
  ezStackFrame* sf = m_stack.top();
  ezInstDecoder decoder;
  ezAddress dest, addr, cond;
  decoder.argument(sf->carousel->instruction[sf->pc++], dest);
  ezValue* v = NULL, *rst = NULL;
  switch (ndests) {
    case 2:
      decoder.argument(sf->carousel->instruction[sf->pc++], cond);
    case 1:
      break;
    default:
      throw runtime_error("the destination of the operation must be 1 or 2");
      break;
  }
  if (nsrcs != 1) throw runtime_error("the operands of the operation must be 1");
  decoder.argument(sf->carousel->instruction[sf->pc++], addr);
  v = addr2val(addr);
  rst = unary_func(v);
  val2addr(dest, rst);
  if (ndests == 2) val2addr(cond, rst->condition());
}

void ezThread::neg(uint8_t ndests, uint8_t nsrcs) {
  unary_operation(ndests, nsrcs, [&](ezValue* v) {return m_alu.neg(v);});
}

void ezThread::bitwise_not(uint8_t ndests, uint8_t nsrcs) {
  unary_operation(ndests, nsrcs, [&](ezValue* v) {return m_alu.bitwise_not(v);});
}

void ezThread::bitwise_or(uint8_t ndests, uint8_t nsrcs) {
  binary_operation(ndests, nsrcs, [&](ezValue* vl, ezValue* vr) {return m_alu.bitwise_or(vl, vr);}, [&](vector<ezValue*>& args) {return m_alu.bitwise_or(args);});
}

void ezThread::bitwise_xor(uint8_t ndests, uint8_t nsrcs) {
  binary_operation(ndests, nsrcs, [&](ezValue* vl, ezValue* vr) {return m_alu.bitwise_xor(vl, vr);}, [&](vector<ezValue*>& args) {return m_alu.bitwise_xor(args);});
}

void ezThread::div(uint8_t ndests, uint8_t nsrcs) {
  binary_operation(ndests, nsrcs, [&](ezValue* vl, ezValue* vr) {return m_alu.div(vl, vr);}, [&](vector<ezValue*>& args) {return m_alu.div(args);});
}

void ezThread::mod(uint8_t ndests, uint8_t nsrcs) {
  binary_operation(ndests, nsrcs, [&](ezValue* vl, ezValue* vr) {return m_alu.mod(vl, vr);}, [&](vector<ezValue*>& args) {return m_alu.mod(args);});
}

void ezThread::mul(uint8_t ndests, uint8_t nsrcs) {
  binary_operation(ndests, nsrcs, [&](ezValue* vl, ezValue* vr) {return m_alu.mul(vl, vr);}, [&](vector<ezValue*>& args) {return m_alu.mul(args);});
}

void ezThread::sub(uint8_t ndests, uint8_t nsrcs) {
  binary_operation(ndests, nsrcs, [&](ezValue* vl, ezValue* vr) {return m_alu.sub(vl, vr);}, [&](vector<ezValue*>& args) {return m_alu.sub(args);});
}

void ezThread::conditional_bra(uint8_t index, function<bool(ezCondition*)>func) {
  ezStackFrame* sf = m_stack.top();
  ezInstDecoder decoder;
  ezAddress addr;
  decoder.argument(sf->carousel->instruction[sf->pc++], addr);
  ezValue* cond = addr2val(addr);
  if (cond->type != EZ_VALUE_TYPE_CONDITION)
    throw runtime_error("The operation doesn't see condition");
  if (func((ezCondition*)cond)) bra(index);
}

void ezThread::beq(uint8_t index) {
  conditional_bra(index, [](ezCondition* cond) {return cond->zero;});
}

void ezThread::bge(uint8_t index) {
  conditional_bra(index, [](ezCondition* cond) {return (cond->zero || !cond->negative);});
}

void ezThread::blt(uint8_t index) {
  conditional_bra(index, [](ezCondition* cond) {return cond->negative;});
}

void ezThread::bne(uint8_t index) {
  conditional_bra(index, [](ezCondition* cond) {return !cond->zero;});
}

void ezThread::bra(uint8_t index) {
  ezStackFrame* sf = m_stack.top();
  if (sf->carousel->jmptbl.size() <= index)
    throw runtime_error("tried to jump to an invalid label");
  sf->pc = sf->carousel->jmptbl[index];
}

ezValue* ezThread::addr2val(ezAddress addr) {
  ezValue* v = NULL;
  if (addr.segment == EZ_ASM_SEGMENT_CONSTANT) {
    if (addr.offset >= m_constants.size())
      throw runtime_error("constant memory access violation");
    v = m_constants[addr.offset];
  } else if (addr.segment == EZ_ASM_SEGMENT_LOCAL) {
    ezStackFrame* sf = m_stack.top();
    if (addr.offset >= sf->local.size())
      throw runtime_error("local memory access violation");
    v = sf->local[addr.offset];
  } else if (addr.segment == EZ_ASM_SEGMENT_PARENT) {
    throw runtime_error("parent segment has not been implemented");
  } else if (addr.segment >= EZ_ASM_SEGMENT_GLOBAL) {
    if (addr.segment >= m_globals.size())
      throw runtime_error("invalid segment");
    if (addr.offset >= m_globals[addr.segment]->size())
      throw runtime_error("global memory access violation");
    v = (*m_globals[addr.segment])[addr.offset];
  } else {
    throw runtime_error("out of segment boundary");
  }
  return v;
}

void ezThread::val2addr(vector<ezAddress>& addr, vector<ezValue*>& vals) {
  size_t addr_sz = addr.size(), vals_sz = vals.size();
  size_t gcv = (addr_sz > vals_sz) ? vals_sz : addr_sz;
  for (size_t i = 0; i < gcv; i++) {
    ezValue* v = vals[i];
    val2addr(addr[i], v);
  }
  if (addr_sz > vals_sz) {
    for (size_t i = gcv; i < addr_sz; i++)
      val2addr(addr[i], ezNull::instance());
  }
}

void ezThread::val2addr(ezAddress addr, ezValue* v) {
  if (addr.segment == EZ_ASM_SEGMENT_CONSTANT) {
    throw runtime_error("cannot write to constant");
  } else if (addr.segment == EZ_ASM_SEGMENT_LOCAL) {
    ezStackFrame* sf = m_stack.top();
    if (addr.offset >= sf->local.size())
      throw runtime_error("local memory access violation");
    v->reference();
    sf->local[addr.offset]->release();
    sf->local[addr.offset] = v;
  } else if (addr.segment == EZ_ASM_SEGMENT_PARENT) {
    throw runtime_error("parent segment has not been implemented");
  } else if (addr.segment >= EZ_ASM_SEGMENT_GLOBAL) {
    if (addr.segment >= m_globals.size())
      throw runtime_error("invalid segment");
    if (addr.offset >= m_globals[addr.segment]->size())
      throw runtime_error("global memory access violation");
    v->reference();
    (*m_globals[addr.segment])[addr.offset]->release();
    (*m_globals[addr.segment])[addr.offset] = v;
  } else {
    throw runtime_error("out of segment boundary");
  }
}
