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
    for (size_t i = gcv; i < vals_sz; i++)
      delete vals[i];
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
    ezAddress &dest, ezAddress &src, ezAddress &offset,
    function<ezValue *(ezValue *, ezValue *)> func) {
  ezValue *rst = NULL, *obj = NULL, *shift = NULL;
  obj = addr2val(src);
  shift = addr2val(offset);
  rst = func(obj, shift);
  val2addr(dest, rst);
}

void ezStackFrame::shift_operation(
    ezAddress &dest, ezAddress &cond, ezAddress &src, ezAddress &offset,
    function<ezValue *(ezValue *, ezValue *)> func) {
  ezValue *rst = NULL, *obj = NULL, *shift = NULL;
  obj = addr2val(src);
  shift = addr2val(offset);
  rst = func(obj, shift);
  val2addr(dest, rst);
  val2addr(cond, rst->condition());
}

void ezStackFrame::lsl(ezAddress &dest, ezAddress &src, ezAddress &offset) {
  shift_operation(dest, src, offset, [](ezValue *obj, ezValue *offset) {
    return new ezInteger(obj->to_integer() << offset->to_integer());
  });
}

void ezStackFrame::lsl(ezAddress &dest, ezAddress &cond, ezAddress &src,
                       ezAddress &offset) {
  shift_operation(dest, cond, src, offset, [](ezValue *obj, ezValue *offset) {
    return new ezInteger(obj->to_integer() << offset->to_integer());
  });
}

void ezStackFrame::lsr(ezAddress &dest, ezAddress &src, ezAddress &offset) {
  shift_operation(dest, src, offset, [](ezValue *obj, ezValue *offset) {
    return new ezInteger(obj->to_integer() >> offset->to_integer());
  });
}

void ezStackFrame::lsr(ezAddress &dest, ezAddress &cond, ezAddress &src,
                       ezAddress &offset) {
  shift_operation(dest, cond, src, offset, [](ezValue *obj, ezValue *offset) {
    return new ezInteger(obj->to_integer() >> offset->to_integer());
  });
}

void ezStackFrame::binary_operation(
    ezAddress &dest, ezAddress &src1, ezAddress &src2,
    function<ezValue *(ezValue *, ezValue *)> binary_func) {
  ezValue *rst = NULL;
  ezValue *vr = NULL, *vl = NULL;
  vl = addr2val(src1);
  vr = addr2val(src2);
  rst = binary_func(vl, vr);
  val2addr(dest, rst);
}

void ezStackFrame::binary_operation(
    ezAddress &dest, ezAddress &cond, ezAddress &src1, ezAddress &src2,
    function<ezValue *(ezValue *, ezValue *)> binary_func) {
  ezValue *rst = NULL;
  ezValue *vr = NULL, *vl = NULL;
  vl = addr2val(src1);
  vr = addr2val(src2);
  rst = binary_func(vl, vr);
  val2addr(dest, rst);
  val2addr(cond, rst->condition());
}

void ezStackFrame::add(ezAddress &dest, ezAddress &src1, ezAddress &src2) {
  binary_operation(dest, src1, src2,
                   [&](ezValue *vl, ezValue *vr) { return m_alu.add(vl, vr); });
}

void ezStackFrame::add(ezAddress &dest, ezAddress &cond, ezAddress &src1,
                       ezAddress &src2) {
  binary_operation(dest, cond, src1, src2,
                   [&](ezValue *vl, ezValue *vr) { return m_alu.add(vl, vr); });
}

void ezStackFrame::div(ezAddress &dest, ezAddress &src1, ezAddress &src2) {
  binary_operation(dest, src1, src2,
                   [&](ezValue *vl, ezValue *vr) { return m_alu.div(vl, vr); });
}

void ezStackFrame::div(ezAddress &dest, ezAddress &cond, ezAddress &src1,
                       ezAddress &src2) {
  binary_operation(dest, cond, src1, src2,
                   [&](ezValue *vl, ezValue *vr) { return m_alu.div(vl, vr); });
}

void ezStackFrame::mod(ezAddress &dest, ezAddress &src1, ezAddress &src2) {
  binary_operation(dest, src1, src2,
                   [&](ezValue *vl, ezValue *vr) { return m_alu.mod(vl, vr); });
}

void ezStackFrame::mod(ezAddress &dest, ezAddress &cond, ezAddress &src1,
                       ezAddress &src2) {
  binary_operation(dest, cond, src1, src2,
                   [&](ezValue *vl, ezValue *vr) { return m_alu.mod(vl, vr); });
}

void ezStackFrame::mul(ezAddress &dest, ezAddress &src1, ezAddress &src2) {
  binary_operation(dest, src1, src2,
                   [&](ezValue *vl, ezValue *vr) { return m_alu.mul(vl, vr); });
}

void ezStackFrame::mul(ezAddress &dest, ezAddress &cond, ezAddress &src1,
                       ezAddress &src2) {
  binary_operation(dest, cond, src1, src2,
                   [&](ezValue *vl, ezValue *vr) { return m_alu.mul(vl, vr); });
}

void ezStackFrame::sub(ezAddress &dest, ezAddress &src1, ezAddress &src2) {
  binary_operation(dest, src1, src2,
                   [&](ezValue *vl, ezValue *vr) { return m_alu.sub(vl, vr); });
}

void ezStackFrame::sub(ezAddress &dest, ezAddress &cond, ezAddress &src1,
                       ezAddress &src2) {
  binary_operation(dest, cond, src1, src2,
                   [&](ezValue *vl, ezValue *vr) { return m_alu.sub(vl, vr); });
}

void ezStackFrame::bitwise_and(ezAddress &dest, ezAddress &src1,
                               ezAddress &src2) {
  binary_operation(dest, src1, src2, [&](ezValue *vl, ezValue *vr) {
    return m_alu.bitwise_and(vl, vr);
  });
}

void ezStackFrame::bitwise_and(ezAddress &dest, ezAddress &cond,
                               ezAddress &src1, ezAddress &src2) {
  binary_operation(dest, cond, src1, src2, [&](ezValue *vl, ezValue *vr) {
    return m_alu.bitwise_and(vl, vr);
  });
}

void ezStackFrame::conditional_bra(ezAddress &cond, size_t index,
                                   function<bool(ezCondition *)> func) {
  ezValue *vcond = addr2val(cond);
  if (vcond->type != EZ_VALUE_TYPE_CONDITION)
    throw runtime_error("The operation doesn't see condition");
  if (func((ezCondition *)vcond))
    bra(index);
}

void ezStackFrame::beq(ezAddress &cond, size_t index) {
  conditional_bra(cond, index, [](ezCondition *cond) { return cond->zero; });
}

void ezStackFrame::bge(ezAddress &cond, size_t index) {
  conditional_bra(cond, index, [](ezCondition *cond) {
    return (cond->zero || !cond->negative);
  });
}

void ezStackFrame::blt(ezAddress &cond, size_t index) {
  conditional_bra(cond, index,
                  [](ezCondition *cond) { return cond->negative; });
}

void ezStackFrame::bne(ezAddress &cond, size_t index) {
  conditional_bra(cond, index, [](ezCondition *cond) { return !cond->zero; });
}

void ezStackFrame::bra(size_t index) {
  if (m_carousel->jmptbl.size() <= index)
    throw runtime_error("tried to jump to an invalid label");
  m_pc = m_carousel->jmptbl[index];
}

void ezStackFrame::bitwise_or(ezAddress &dest, ezAddress &src1,
                              ezAddress &src2) {
  binary_operation(dest, src1, src2, [&](ezValue *vl, ezValue *vr) {
    return m_alu.bitwise_or(vl, vr);
  });
}

void ezStackFrame::bitwise_or(ezAddress &dest, ezAddress &cond, ezAddress &src1,
                              ezAddress &src2) {
  binary_operation(dest, cond, src1, src2, [&](ezValue *vl, ezValue *vr) {
    return m_alu.bitwise_or(vl, vr);
  });
}

void ezStackFrame::bitwise_xor(ezAddress &dest, ezAddress &src1,
                               ezAddress &src2) {
  binary_operation(dest, src1, src2, [&](ezValue *vl, ezValue *vr) {
    return m_alu.bitwise_xor(vl, vr);
  });
}

void ezStackFrame::bitwise_xor(ezAddress &dest, ezAddress &cond,
                               ezAddress &src1, ezAddress &src2) {
  binary_operation(dest, cond, src1, src2, [&](ezValue *vl, ezValue *vr) {
    return m_alu.bitwise_xor(vl, vr);
  });
}

void ezStackFrame::unary_operation(ezAddress &dest, ezAddress &src,
                                   function<ezValue *(ezValue *)> unary_func) {
  ezValue *v = NULL, *rst = NULL;
  v = addr2val(src);
  rst = unary_func(v);
  val2addr(dest, rst);
}

void ezStackFrame::unary_operation(ezAddress &dest, ezAddress &cond,
                                   ezAddress &src,
                                   function<ezValue *(ezValue *)> unary_func) {
  ezValue *v = NULL, *rst = NULL;
  v = addr2val(src);
  rst = unary_func(v);
  val2addr(dest, rst);
  val2addr(cond, rst->condition());
}

void ezStackFrame::neg(ezAddress &dest, ezAddress &src) {
  unary_operation(dest, src, [&](ezValue *v) { return m_alu.neg(v); });
}

void ezStackFrame::neg(ezAddress &dest, ezAddress &cond, ezAddress &src) {
  unary_operation(dest, cond, src, [&](ezValue *v) { return m_alu.neg(v); });
}

void ezStackFrame::bitwise_not(ezAddress &dest, ezAddress &src) {
  unary_operation(dest, src, [&](ezValue *v) { return m_alu.bitwise_not(v); });
}

void ezStackFrame::bitwise_not(ezAddress &dest, ezAddress &cond,
                               ezAddress &src) {
  unary_operation(dest, cond, src,
                  [&](ezValue *v) { return m_alu.bitwise_not(v); });
}

void ezStackFrame::fgc(void) { m_gc.force(); }

void ezStackFrame::ret(vector<ezAddress> &srcs) {
  ezAddress dest, addr, cond;
  ezValue *v = NULL;
  for (size_t i = 0; i < srcs.size(); i++) {
    v = addr2val(srcs[i]);
    m_returns.push_back(v);
  }
}

void ezStackFrame::mv(vector<ezAddress> &dests, vector<ezAddress> &srcs) {
  size_t cnt = (dests.size() > srcs.size()) ? srcs.size() : dests.size();
  ezValue *v = NULL;
  size_t i = 0;
  vector<ezValue *> q;
  for (i = 0; i < cnt; i++) {
    v = addr2val(srcs[i]);
    q.push_back(v);
  }
  for (i = 0; i < cnt; i++) {
    val2addr(dests[i], q[i]);
  }
  if (dests.size() > srcs.size()) {
    for (i = cnt; i < dests.size(); i++) {
      val2addr(dests[i], ezNull::instance());
    }
  }
}

void ezStackFrame::cmp(ezAddress &cond, ezAddress &src1, ezAddress &src2) {
  ezValue *rst = NULL, *vr = NULL, *vl = NULL;
  vl = addr2val(src1);
  vr = addr2val(src2);
  rst = m_alu.cmp(vl, vr);
  val2addr(cond, rst);
}

ezStepState ezStackFrame::call(ezAddress &func, vector<ezAddress> &args,
                               vector<ezAddress> &rets) {
  ezValue *proc = addr2val(func);
  ezStepState state = EZ_STEP_CONTINUE;
  switch (proc->type) {
  case EZ_VALUE_TYPE_NATIVE_CAROUSEL:
    state = call((ezNativeCarousel *)proc, args, rets);
    break;
  case EZ_VALUE_TYPE_CAROUSEL:
    state = call((ezCarousel *)proc, args, rets);
    break;
  default:
    throw runtime_error("function is not executable");
    break;
  }
  return state;
}

ezStepState ezStackFrame::call(ezCarousel *func, vector<ezAddress> &args,
                               vector<ezAddress> &rets) {
  m_callee = new ezStackFrame(func, m_globals, m_constants, m_alu, m_gc);
  ezAddress addr;
  size_t min_args = (func->nargs > args.size()) ? args.size() : func->nargs;
  for (size_t i = 0; i < min_args; i++) {
    ezValue *v = addr2val(args[i]);
    (*(m_callee->m_local))[i] = v;
  }
  for (size_t i = 0; i < rets.size(); i++) {
    m_callee->m_return_dest.push_back(rets[i]);
  }
  return EZ_STEP_CALL;
}

ezStepState ezStackFrame::call(ezNativeCarousel *func, vector<ezAddress> &args,
                               vector<ezAddress> &rets) {
  vector<ezValue *> vargs;
  for (size_t i = 0; i < args.size(); i++) {
    ezValue *v = addr2val(args[i]);
    vargs.push_back(v);
  }
  vector<ezValue *> vrets;
  func->run(vargs, vrets);
  if (rets.size())
    val2addr(rets, vrets);
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
  ezInstruction *inst = m_carousel->instruction[m_pc++];
  ezStepState status = EZ_STEP_CONTINUE;
  uint8_t arg1, arg2, arg3;
  switch (inst->cmd) {
  case EZ_OP_ADD:
    if (inst->dests.size() == 1)
      add(inst->dests[0], inst->srcs[0], inst->srcs[1]);
    else
      add(inst->dests[0], inst->dests[1], inst->srcs[0], inst->srcs[1]);
    break;
  case EZ_OP_AND:
    if (inst->dests.size() == 1)
      bitwise_and(inst->dests[0], inst->srcs[0], inst->srcs[1]);
    else
      bitwise_and(inst->dests[0], inst->dests[1], inst->srcs[0], inst->srcs[1]);
    break;
  case EZ_OP_BEQ:
    beq(inst->arg, inst->offset);
    break;
  case EZ_OP_BGE:
    bge(inst->arg, inst->offset);
    break;
  case EZ_OP_BLT:
    blt(inst->arg, inst->offset);
    break;
  case EZ_OP_BNE:
    bne(inst->arg, inst->offset);
    break;
  case EZ_OP_BRA:
    bra(inst->offset);
    break;
  case EZ_OP_CALL:
    status = call(inst->arg, inst->srcs, inst->dests);
    break;
  case EZ_OP_CMP:
    cmp(inst->dests[0], inst->srcs[0], inst->srcs[1]);
    break;
  case EZ_OP_DIV:
    if (inst->dests.size() == 1)
      div(inst->dests[0], inst->srcs[0], inst->srcs[1]);
    else
      div(inst->dests[0], inst->dests[1], inst->srcs[0], inst->srcs[1]);
    break;
  case EZ_OP_FGC:
    fgc();
  case EZ_OP_LSL:
    if (inst->dests.size() == 1)
      lsl(inst->dests[0], inst->srcs[0], inst->arg);
    else
      lsl(inst->dests[0], inst->dests[1], inst->srcs[0], inst->arg);
    break;
  case EZ_OP_LSR:
    if (inst->dests.size() == 1)
      lsr(inst->dests[0], inst->srcs[0], inst->arg);
    else
      lsr(inst->dests[0], inst->dests[1], inst->srcs[0], inst->arg);
    break;
    break;
  case EZ_OP_MOD:
    if (inst->dests.size() == 1)
      mod(inst->dests[0], inst->srcs[0], inst->srcs[1]);
    else
      mod(inst->dests[0], inst->dests[1], inst->srcs[0], inst->srcs[1]);
    break;
  case EZ_OP_MUL:
    if (inst->dests.size() == 1)
      mul(inst->dests[0], inst->srcs[0], inst->srcs[1]);
    else
      mul(inst->dests[0], inst->dests[1], inst->srcs[0], inst->srcs[1]);
    break;
  case EZ_OP_MV:
    mv(inst->dests, inst->srcs);
    break;
  case EZ_OP_NEG:
    if (inst->dests.size() == 1)
      neg(inst->dests[0], inst->srcs[0]);
    else
      neg(inst->dests[0], inst->dests[1], inst->srcs[0]);
    break;
  case EZ_OP_NOT:
    if (inst->dests.size() == 1)
      bitwise_not(inst->dests[0], inst->srcs[0]);
    else
      bitwise_not(inst->dests[0], inst->dests[1], inst->srcs[0]);
    break;
  case EZ_OP_OR:
    if (inst->dests.size() == 1)
      bitwise_or(inst->dests[0], inst->srcs[0], inst->srcs[1]);
    else
      bitwise_or(inst->dests[0], inst->dests[1], inst->srcs[0], inst->srcs[1]);
    break;
  case EZ_OP_RET:
    ret(inst->dests);
    break;
  case EZ_OP_SUB:
    if (inst->dests.size() == 1)
      sub(inst->dests[0], inst->srcs[0], inst->srcs[1]);
    else
      sub(inst->dests[0], inst->dests[1], inst->srcs[0], inst->srcs[1]);
    break;
  case EZ_OP_XOR:
    if (inst->dests.size() == 1)
      bitwise_xor(inst->dests[0], inst->srcs[0], inst->srcs[1]);
    else
      bitwise_xor(inst->dests[0], inst->dests[1], inst->srcs[0], inst->srcs[1]);
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
