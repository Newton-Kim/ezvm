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
#include "ezvm/ezinstruction.h"
#include "ezvm/ezlog.h"
#include "ezvm/ezstack.h"
#include <iostream>
#include <stdexcept>

ezStackFrame::ezStackFrame(ezCarousel *crsl,
                           ezTable<string, ezValue *> &globals,
                           vector<ezValue *> &constants, ezALU &alu, ezGC &gc)
    : m_pc(0), m_local(m_carousel->local_memory()),
      m_scope(m_carousel->scope_memory()), m_carousel(crsl),
      m_constants(constants), m_globals(globals), m_alu(alu), m_gc(gc),
      m_callee(NULL) {
  ezLog::instance().verbose("%s", __PRETTY_FUNCTION__);
}

ezStackFrame::~ezStackFrame() {
  if (!m_carousel->is_local_scoped())
    delete m_local;
}

ezValue *ezStackFrame::addr2val(ezAddress addr) {
  ezValue *v = NULL;
  switch (addr.segment) {
  case EZ_ASM_SEGMENT_CONSTANT:
    if (addr.offset >= m_constants.size())
      throw runtime_error("constant memory access violation");
    v = m_constants[addr.offset];
    break;
  case EZ_ASM_SEGMENT_LOCAL: {
    if (addr.offset >= m_local->size())
      throw runtime_error("local memory access violation");
    v = (*m_local)[addr.offset];
  } break;
  case EZ_ASM_SEGMENT_SCOPE:
    if (!m_scope)
      throw runtime_error("the function does not have a scope");
    if (addr.offset >= m_scope->size())
      throw runtime_error("scope memory access violation");
    v = (*m_scope)[addr.offset];
    break;
  case EZ_ASM_SEGMENT_GLOBAL:
    if (addr.offset >= m_globals.m_memory.size())
      throw runtime_error("global memory access violation");
    v = m_globals.m_memory[addr.offset];
    break;
  default:
    throw runtime_error("out of segment boundary");
  }
  return v;
}

void ezStackFrame::val2addr(vector<ezAddress> &addr, vector<ezValue *> &vals) {
  size_t addr_sz = addr.size(), vals_sz = vals.size();
  size_t gcv = (addr_sz > vals_sz) ? vals_sz : addr_sz;
  for (size_t i = 0; i < gcv; i++) {
    ezValue *v = vals[i];
    val2addr(addr[i], v);
  }
  if (addr_sz > vals_sz) {
    for (size_t i = gcv; i < addr_sz; i++)
      val2addr(addr[i], ezNull::instance());
  } else if (addr_sz < vals_sz) {
    for (size_t i = gcv; i < vals_sz; i++) delete vals[i];
  }
}

void ezStackFrame::val2addr(ezAddress addr, ezValue *v) {
  switch (addr.segment) {
  case EZ_ASM_SEGMENT_CONSTANT:
    throw runtime_error("cannot write to constant");
    break;
  case EZ_ASM_SEGMENT_LOCAL: {
    if (addr.offset >= m_local->size())
      throw runtime_error("local memory access violation");
    (*m_local)[addr.offset] = v;
  } break;
  case EZ_ASM_SEGMENT_SCOPE:
    if (!m_scope)
      throw runtime_error("the function does not have a scope");
    if (addr.offset >= m_scope->size())
      throw runtime_error("scope memory access violation");
    (*m_scope)[addr.offset] = v;
    break;
  case EZ_ASM_SEGMENT_GLOBAL:
    if (addr.offset >= m_globals.m_memory.size())
      throw runtime_error("global memory access violation");
    m_globals.m_memory[addr.offset] = v;
    break;
    break;
    throw runtime_error("out of segment boundary");
    break;
  }
  m_gc.pause();
  if (v != ezNull::instance())
    m_gc.add((ezGCObject *)v);
  m_gc.resume();
}

void ezStackFrame::shift_operation(
    uint8_t ndests, uint8_t nsrcs, uint8_t noffsets,
    function<ezValue *(ezValue *, ezValue *)> func) {
  ezInstDecoder decoder;
  ezAddress dest, addr, cond;
  decoder.argument(m_carousel->instruction[m_pc++], dest);
  ezValue *rst = NULL, *obj = NULL, *offset = NULL;
  switch (ndests) {
  case 2:
    decoder.argument(m_carousel->instruction[m_pc++], cond);
  case 1:
    break;
  default:
    throw runtime_error("the destination of the operation must be 1 or 2");
    break;
  }
  switch (nsrcs) {
  case 1:
    decoder.argument(m_carousel->instruction[m_pc++], addr);
    obj = addr2val(addr);
    break;
  default:
    throw runtime_error("the oject of the operation must be 1");
    break;
  }
  switch (noffsets) {
  case 1:
    decoder.argument(m_carousel->instruction[m_pc++], addr);
    offset = addr2val(addr);
    break;
  default:
    throw runtime_error("the offset of the operation must be 1");
    break;
  }
  rst = func(obj, offset);
  val2addr(dest, rst);
  if (ndests == 2)
    val2addr(cond, rst->condition());
}

void ezStackFrame::lsl(uint8_t ndests, uint8_t nsrcs, uint8_t noffsets) {
  shift_operation(ndests, nsrcs, noffsets, [](ezValue *obj, ezValue *offset) {
    return new ezInteger(obj->to_integer() << offset->to_integer());
  });
}

void ezStackFrame::lsr(uint8_t ndests, uint8_t nsrcs, uint8_t noffsets) {
  shift_operation(ndests, nsrcs, noffsets, [](ezValue *obj, ezValue *offset) {
    return new ezInteger(obj->to_integer() >> offset->to_integer());
  });
}

void ezStackFrame::binary_operation(
    uint8_t ndests, uint8_t nsrcs,
    function<ezValue *(ezValue *, ezValue *)> binary_func) {
  ezInstDecoder decoder;
  ezAddress dest, addr, cond;
  decoder.argument(m_carousel->instruction[m_pc++], dest);
  ezValue *rst = NULL;
  switch (ndests) {
  case 2:
    decoder.argument(m_carousel->instruction[m_pc++], cond);
  case 1:
    break;
  default:
    throw runtime_error("the destination of the operands must be 1 or 2");
    break;
  }
  switch (nsrcs) {
  case 2: {
    ezValue *vr = NULL, *vl = NULL;
    decoder.argument(m_carousel->instruction[m_pc++], addr);
    vl = addr2val(addr);
    decoder.argument(m_carousel->instruction[m_pc++], addr);
    vr = addr2val(addr);
    rst = binary_func(vl, vr);
  } break;
  default:
    throw runtime_error("the number of the operands must be 2 or more");
    break;
  }
  val2addr(dest, rst);
  if (ndests == 2)
    val2addr(cond, rst->condition());
}

void ezStackFrame::add(uint8_t ndests, uint8_t nsrcs) {
  binary_operation(ndests, nsrcs,
                   [&](ezValue *vl, ezValue *vr) { return m_alu.add(vl, vr); });
}

void ezStackFrame::div(uint8_t ndests, uint8_t nsrcs) {
  binary_operation(ndests, nsrcs,
                   [&](ezValue *vl, ezValue *vr) { return m_alu.div(vl, vr); });
}

void ezStackFrame::mod(uint8_t ndests, uint8_t nsrcs) {
  binary_operation(ndests, nsrcs,
                   [&](ezValue *vl, ezValue *vr) { return m_alu.mod(vl, vr); });
}

void ezStackFrame::mul(uint8_t ndests, uint8_t nsrcs) {
  binary_operation(ndests, nsrcs,
                   [&](ezValue *vl, ezValue *vr) { return m_alu.mul(vl, vr); });
}

void ezStackFrame::sub(uint8_t ndests, uint8_t nsrcs) {
  binary_operation(ndests, nsrcs,
                   [&](ezValue *vl, ezValue *vr) { return m_alu.sub(vl, vr); });
}

void ezStackFrame::bitwise_and(uint8_t ndests, uint8_t nsrcs) {
  binary_operation(ndests, nsrcs, [&](ezValue *vl, ezValue *vr) {
    return m_alu.bitwise_and(vl, vr);
  });
}

void ezStackFrame::conditional_bra(uint8_t index,
                                   function<bool(ezCondition *)> func) {
  ezInstDecoder decoder;
  ezAddress addr;
  decoder.argument(m_carousel->instruction[m_pc++], addr);
  ezValue *cond = addr2val(addr);
  if (cond->type != EZ_VALUE_TYPE_CONDITION)
    throw runtime_error("The operation doesn't see condition");
  if (func((ezCondition *)cond))
    bra(index);
}

void ezStackFrame::beq(uint8_t index) {
  conditional_bra(index, [](ezCondition *cond) { return cond->zero; });
}

void ezStackFrame::bge(uint8_t index) {
  conditional_bra(
      index, [](ezCondition *cond) { return (cond->zero || !cond->negative); });
}

void ezStackFrame::blt(uint8_t index) {
  conditional_bra(index, [](ezCondition *cond) { return cond->negative; });
}

void ezStackFrame::bne(uint8_t index) {
  conditional_bra(index, [](ezCondition *cond) { return !cond->zero; });
}

void ezStackFrame::bra(uint8_t index) {
  if (m_carousel->jmptbl.size() <= index)
    throw runtime_error("tried to jump to an invalid label");
  m_pc = m_carousel->jmptbl[index];
}

void ezStackFrame::bitwise_or(uint8_t ndests, uint8_t nsrcs) {
  binary_operation(ndests, nsrcs, [&](ezValue *vl, ezValue *vr) {
    return m_alu.bitwise_or(vl, vr);
  });
}

void ezStackFrame::bitwise_xor(uint8_t ndests, uint8_t nsrcs) {
  binary_operation(ndests, nsrcs, [&](ezValue *vl, ezValue *vr) {
    return m_alu.bitwise_xor(vl, vr);
  });
}

void ezStackFrame::unary_operation(uint8_t ndests, uint8_t nsrcs,
                                   function<ezValue *(ezValue *)> unary_func) {
  ezInstDecoder decoder;
  ezAddress dest, addr, cond;
  decoder.argument(m_carousel->instruction[m_pc++], dest);
  ezValue *v = NULL, *rst = NULL;
  switch (ndests) {
  case 2:
    decoder.argument(m_carousel->instruction[m_pc++], cond);
  case 1:
    break;
  default:
    throw runtime_error("the destination of the operation must be 1 or 2");
    break;
  }
  if (nsrcs != 1)
    throw runtime_error("the operands of the operation must be 1");
  decoder.argument(m_carousel->instruction[m_pc++], addr);
  v = addr2val(addr);
  rst = unary_func(v);
  val2addr(dest, rst);
  if (ndests == 2)
    val2addr(cond, rst->condition());
}

void ezStackFrame::neg(uint8_t ndests, uint8_t nsrcs) {
  unary_operation(ndests, nsrcs, [&](ezValue *v) { return m_alu.neg(v); });
}

void ezStackFrame::bitwise_not(uint8_t ndests, uint8_t nsrcs) {
  unary_operation(ndests, nsrcs,
                  [&](ezValue *v) { return m_alu.bitwise_not(v); });
}

void ezStackFrame::fgc(void) { m_gc.force(); }

void ezStackFrame::ret(uint8_t nsrcs) {
  ezInstDecoder decoder;
  ezAddress dest, addr, cond;
  ezValue *v = NULL;
  for (size_t i = 0; i < nsrcs; i++) {
    decoder.argument(m_carousel->instruction[m_pc++], addr);
    v = addr2val(addr);
    m_returns.push_back(v);
  }
}

void ezStackFrame::mv(uint8_t ndests, uint8_t nsrcs) {
  size_t cnt = (ndests > nsrcs) ? nsrcs : ndests;
  ezInstDecoder decoder;
  ezAddress src_addr;
  ezAddress dest_addr;
  ezValue *v = NULL;
  size_t i = 0;
  vector<ezValue *> q;
  for (i = 0; i < cnt; i++) {
    decoder.argument(m_carousel->instruction[m_pc + i + ndests], src_addr);
    v = addr2val(src_addr);
    q.push_back(v);
  }
  for (i = 0; i < cnt; i++) {
    decoder.argument(m_carousel->instruction[m_pc + i], dest_addr);
    v = addr2val(dest_addr);
    v = q[i];
    val2addr(dest_addr, v);
  }
  if (ndests > nsrcs) {
    for (i = cnt; i < ndests; i++) {
      decoder.argument(m_carousel->instruction[m_pc + cnt + i], dest_addr);
      val2addr(dest_addr, ezNull::instance());
    }
  }
  m_pc += (ndests + nsrcs);
}

void ezStackFrame::cmp(uint8_t ndests, uint8_t nsrcs) {
  ezInstDecoder decoder;
  ezAddress addr, cond;
  ezValue *rst = NULL;
  switch (ndests) {
  case 1:
    decoder.argument(m_carousel->instruction[m_pc++], cond);
    break;
  default:
    throw runtime_error("the destination of CMP must be 1");
    break;
  }
  switch (nsrcs) {
  case 2: {
    ezValue *vr = NULL, *vl = NULL;
    decoder.argument(m_carousel->instruction[m_pc++], addr);
    vl = addr2val(addr);
    decoder.argument(m_carousel->instruction[m_pc++], addr);
    vr = addr2val(addr);
    rst = m_alu.cmp(vl, vr);
  } break;
    throw runtime_error("the number of ADD operands must be 2");
    break;
  }
  val2addr(cond, rst);
}

ezStepState ezStackFrame::call(uint8_t nargs, uint8_t nrets) {
  ezInstDecoder decoder;
  ezAddress addr;
  decoder.argument(m_carousel->instruction[m_pc++], addr);
  ezValue *func = addr2val(addr);
  ezStepState state = EZ_STEP_CONTINUE;
  switch (func->type) {
  case EZ_VALUE_TYPE_NATIVE_CAROUSEL:
    state = call((ezNativeCarousel *)func, nargs, nrets);
    break;
  case EZ_VALUE_TYPE_CAROUSEL:
    state = call((ezCarousel *)func, nargs, nrets);
    break;
  default:
    throw runtime_error("function is not executable");
    break;
  }
  return state;
}

ezStepState ezStackFrame::call(ezCarousel *func, uint8_t nargs, uint8_t nrets) {
  m_callee = new ezStackFrame(func, m_globals, m_constants, m_alu, m_gc);
  ezInstDecoder decoder;
  vector<ezValue *> args;
  ezAddress addr;
  size_t min_args = (func->nargs > nargs) ? nargs : func->nargs;
  for (size_t i = 0; i < min_args; i++, m_pc++) {
    decoder.argument(m_carousel->instruction[m_pc], addr);
    ezValue *v = addr2val(addr);
    (*(m_callee->m_local))[i] = v;
  }
  vector<ezAddress> ret_dest;
  for (size_t i = 0; i < nrets; i++, m_pc++) {
    decoder.argument(m_carousel->instruction[m_pc], addr);
    m_callee->m_return_dest.push_back(addr);
  }
  return EZ_STEP_CALL;
}

ezStepState ezStackFrame::call(ezNativeCarousel *func, uint8_t nargs,
                               uint8_t nrets) {
  ezInstDecoder decoder;
  vector<ezValue *> args;
  ezAddress addr;
  for (size_t i = 0; i < nargs; i++, m_pc++) {
    decoder.argument(m_carousel->instruction[m_pc], addr);
    ezValue *v = addr2val(addr);
    args.push_back(v);
  }
  vector<ezAddress> ret_dest;
  for (size_t i = 0; i < nrets; i++, m_pc++) {
    decoder.argument(m_carousel->instruction[m_pc], addr);
    ret_dest.push_back(addr);
  }
  vector<ezValue *> rets;
  func->run(args, rets);
  if (rets.size())
    val2addr(ret_dest, rets);
  return EZ_STEP_CONTINUE;
}

ezStepState ezStackFrame::step(void) {
  if (m_callee) {
    size_t rets = m_callee->m_returns.size();
    size_t dests = m_callee->m_return_dest.size();
    size_t cnt = (rets > dests) ? dests : rets;
    for (size_t i = 0; i < cnt; i++)
      val2addr(m_callee->m_return_dest[i], m_callee->m_returns[i]);
    if (dests > rets) {
      for (size_t i = cnt; i < dests; i++)
        val2addr(m_callee->m_return_dest[i], ezNull::instance());
    }
    delete m_callee;
    m_callee = NULL;
  }
  if (m_pc >= m_carousel->instruction.size())
    return EZ_STEP_DONE;
  ezStepState status = EZ_STEP_CONTINUE;
  ezInstDecoder decoder;
  ezOpCode op;
  uint8_t arg1, arg2, arg3;
  decoder.opcode(m_carousel->instruction[m_pc++], op, arg1, arg2, arg3);
  switch (op) {
  case EZ_OP_ADD:
    add(arg1, arg2);
    break;
  case EZ_OP_AND:
    bitwise_and(arg1, arg2);
    break;
  case EZ_OP_BEQ:
    beq(arg1);
    break;
  case EZ_OP_BGE:
    bge(arg1);
    break;
  case EZ_OP_BLT:
    blt(arg1);
    break;
  case EZ_OP_BNE:
    bne(arg1);
    break;
  case EZ_OP_BRA:
    bra(arg1);
    break;
  case EZ_OP_CALL:
    status = call(arg1, arg2);
    break;
  case EZ_OP_CMP:
    cmp(arg1, arg2);
    break;
  case EZ_OP_DIV:
    div(arg1, arg2);
    break;
  case EZ_OP_FGC:
    fgc();
  case EZ_OP_LSL:
    lsl(arg1, arg2, arg3);
    break;
  case EZ_OP_LSR:
    lsr(arg1, arg2, arg3);
    break;
  case EZ_OP_MOD:
    mod(arg1, arg2);
    break;
  case EZ_OP_MUL:
    mul(arg1, arg2);
    break;
  case EZ_OP_MV:
    mv(arg1, arg2);
    break;
  case EZ_OP_NEG:
    neg(arg1, arg2);
    break;
  case EZ_OP_NOT:
    bitwise_not(arg1, arg2);
    break;
  case EZ_OP_OR:
    bitwise_or(arg1, arg2);
    break;
  case EZ_OP_RET:
    ret(arg1);
    break;
  case EZ_OP_SUB:
    sub(arg1, arg2);
    break;
  case EZ_OP_XOR:
    bitwise_xor(arg1, arg2);
    break;
  }
  return status;
}

void ezStackFrame::on_mark(void) {
  for (vector<ezValue *>::iterator it = m_local->begin(); it != m_local->end();
       it++)
    (*it)->mark();
  for (vector<ezValue *>::iterator it = m_returns.begin();
       it != m_returns.end(); it++)
    (*it)->mark();
}
