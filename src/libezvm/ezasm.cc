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
#include "ezvm/ezasm.h"
#include "ezvm/ezlog.h"
#include "intrinsic/ezintrinsic.h"
#include <iostream>
#include <stdexcept>

using namespace std;

ezAsmProcedure::ezAsmProcedure(ezCarousel* carousel) : m_carousel(carousel) {}

void ezAsmProcedure::call(const ezAddress& func, vector<ezAddress>& args,
                          vector<ezAddress>& rets) {
  ezInstEncoder instruction(m_carousel->instruction);
  instruction.opcode(EZ_OP_CALL, args.size(), rets.size());
  instruction.argument(func);
  for (vector<ezAddress>::iterator it = args.begin(); it != args.end(); it++)
    instruction.argument(*it);
  for (vector<ezAddress>::iterator it = rets.begin(); it != rets.end(); it++)
    instruction.argument(*it);
}

void ezAsmProcedure::cmp(const ezAddress& cond, const ezAddress& larg,
                         const ezAddress& rarg) {
  ezInstEncoder instruction(m_carousel->instruction);
  instruction.opcode(EZ_OP_CMP, 1, 2);
  instruction.argument(cond);
  instruction.argument(larg);
  instruction.argument(rarg);
}

void ezAsmProcedure::mv(vector<ezAddress>& dest, vector<ezAddress>& src) {
  ezInstEncoder instruction(m_carousel->instruction);
  instruction.opcode(EZ_OP_MV, dest.size(), src.size());
  for (vector<ezAddress>::iterator it = dest.begin(); it != dest.end(); it++)
    instruction.argument(*it);
  for (vector<ezAddress>::iterator it = src.begin(); it != src.end(); it++)
    instruction.argument(*it);
}

void ezAsmProcedure::instruction_with_1_1_1_arguments(const ezOpCode op,
                                                      const ezAddress dest,
                                                      const ezAddress obj,
                                                      const ezAddress offset) {
  ezInstEncoder instruction(m_carousel->instruction);
  instruction.opcode(op, 1, 1, 1);
  instruction.argument(dest);
  instruction.argument(obj);
  instruction.argument(offset);
}

void ezAsmProcedure::instruction_with_1_1_1_arguments(const ezOpCode op,
                                                      const ezAddress dest,
                                                      const ezAddress cond,
                                                      const ezAddress obj,
                                                      const ezAddress offset) {
  ezInstEncoder instruction(m_carousel->instruction);
  instruction.opcode(op, 2, 1, 1);
  instruction.argument(dest);
  instruction.argument(cond);
  instruction.argument(obj);
  instruction.argument(offset);
}

void ezAsmProcedure::ld(const ezAddress dest, const ezAddress obj,
                        const ezAddress offset) {
  instruction_with_1_1_1_arguments(EZ_OP_LD, dest, obj, offset);
}

void ezAsmProcedure::lsl(const ezAddress dest, const ezAddress obj,
                         const ezAddress offset) {
  instruction_with_1_1_1_arguments(EZ_OP_LSL, dest, obj, offset);
}

void ezAsmProcedure::lsl(const ezAddress dest, const ezAddress cond,
                         const ezAddress obj, const ezAddress offset) {
  instruction_with_1_1_1_arguments(EZ_OP_LSL, dest, cond, obj, offset);
}

void ezAsmProcedure::lsr(const ezAddress dest, const ezAddress obj,
                         const ezAddress offset) {
  instruction_with_1_1_1_arguments(EZ_OP_LSR, dest, obj, offset);
}

void ezAsmProcedure::lsr(const ezAddress dest, const ezAddress cond,
                         const ezAddress obj, const ezAddress offset) {
  instruction_with_1_1_1_arguments(EZ_OP_LSR, dest, cond, obj, offset);
}

void ezAsmProcedure::instruction_with_array_arguments(ezOpCode op,
                                                      const ezAddress dest,
                                                      vector<ezAddress>& src) {
  ezInstEncoder instruction(m_carousel->instruction);
  instruction.opcode(op, 1, src.size());
  instruction.argument(dest);
  for (vector<ezAddress>::iterator it = src.begin(); it != src.end(); it++)
    instruction.argument(*it);
}

void ezAsmProcedure::instruction_with_array_arguments(ezOpCode op,
                                                      const ezAddress dest,
                                                      const ezAddress cond,
                                                      vector<ezAddress>& src) {
  ezInstEncoder instruction(m_carousel->instruction);
  instruction.opcode(op, 2, src.size());
  instruction.argument(dest);
  instruction.argument(cond);
  for (vector<ezAddress>::iterator it = src.begin(); it != src.end(); it++)
    instruction.argument(*it);
}

void ezAsmProcedure::add(const ezAddress dest, vector<ezAddress>& src) {
  instruction_with_array_arguments(EZ_OP_ADD, dest, src);
}

void ezAsmProcedure::add(const ezAddress dest, const ezAddress cond,
                         vector<ezAddress>& src) {
  instruction_with_array_arguments(EZ_OP_ADD, dest, cond, src);
}

void ezAsmProcedure::bitwise_and(const ezAddress dest, vector<ezAddress>& src) {
  instruction_with_array_arguments(EZ_OP_AND, dest, src);
}

void ezAsmProcedure::bitwise_and(const ezAddress dest, const ezAddress cond,
                                 vector<ezAddress>& src) {
  instruction_with_array_arguments(EZ_OP_AND, dest, cond, src);
}

void ezAsmProcedure::div(const ezAddress dest, vector<ezAddress>& src) {
  instruction_with_array_arguments(EZ_OP_DIV, dest, src);
}

void ezAsmProcedure::div(const ezAddress dest, const ezAddress cond,
                         vector<ezAddress>& src) {
  instruction_with_array_arguments(EZ_OP_DIV, dest, cond, src);
}

void ezAsmProcedure::mod(const ezAddress dest, vector<ezAddress>& src) {
  instruction_with_array_arguments(EZ_OP_MOD, dest, src);
}

void ezAsmProcedure::mod(const ezAddress dest, const ezAddress cond,
                         vector<ezAddress>& src) {
  instruction_with_array_arguments(EZ_OP_MOD, dest, cond, src);
}

void ezAsmProcedure::mul(const ezAddress dest, vector<ezAddress>& src) {
  instruction_with_array_arguments(EZ_OP_MUL, dest, src);
}

void ezAsmProcedure::mul(const ezAddress dest, const ezAddress cond,
                         vector<ezAddress>& src) {
  instruction_with_array_arguments(EZ_OP_MUL, dest, cond, src);
}

void ezAsmProcedure::instruction_with_an_argument(ezOpCode op,
                                                  const ezAddress dest,
                                                  const ezAddress org) {
  ezInstEncoder instruction(m_carousel->instruction);
  instruction.opcode(op, 1, 1);
  instruction.argument(dest);
  instruction.argument(org);
}

void ezAsmProcedure::instruction_with_an_argument(ezOpCode op,
                                                  const ezAddress dest,
                                                  const ezAddress cond,
                                                  const ezAddress org) {
  ezInstEncoder instruction(m_carousel->instruction);
  instruction.opcode(op, 2, 1);
  instruction.argument(dest);
  instruction.argument(cond);
  instruction.argument(org);
}

void ezAsmProcedure::neg(const ezAddress dest, const ezAddress org) {
  instruction_with_an_argument(EZ_OP_NEG, dest, org);
}

void ezAsmProcedure::neg(const ezAddress dest, const ezAddress cond,
                         const ezAddress org) {
  instruction_with_an_argument(EZ_OP_NEG, dest, cond, org);
}

void ezAsmProcedure::bitwise_not(const ezAddress dest, const ezAddress org) {
  instruction_with_an_argument(EZ_OP_NOT, dest, org);
}

void ezAsmProcedure::bitwise_not(const ezAddress dest, const ezAddress cond,
                                 const ezAddress org) {
  instruction_with_an_argument(EZ_OP_NOT, dest, cond, org);
}

void ezAsmProcedure::bitwise_or(const ezAddress dest, vector<ezAddress>& src) {
  instruction_with_array_arguments(EZ_OP_OR, dest, src);
}

void ezAsmProcedure::bitwise_or(const ezAddress dest, const ezAddress cond,
                                vector<ezAddress>& src) {
  instruction_with_array_arguments(EZ_OP_OR, dest, cond, src);
}

void ezAsmProcedure::ret(void) {
  ezInstEncoder instruction(m_carousel->instruction);
  instruction.opcode(EZ_OP_RET);
}

void ezAsmProcedure::ret(vector<ezAddress>& src) {
  ezInstEncoder instruction(m_carousel->instruction);
  instruction.opcode(EZ_OP_RET, src.size());
  for (vector<ezAddress>::iterator it = src.begin(); it != src.end(); it++)
    instruction.argument(*it);
}

void ezAsmProcedure::sub(const ezAddress dest, vector<ezAddress>& src) {
  instruction_with_array_arguments(EZ_OP_SUB, dest, src);
}

void ezAsmProcedure::sub(const ezAddress dest, const ezAddress cond,
                         vector<ezAddress>& src) {
  instruction_with_array_arguments(EZ_OP_SUB, dest, cond, src);
}

void ezAsmProcedure::bitwise_xor(const ezAddress dest, vector<ezAddress>& src) {
  instruction_with_array_arguments(EZ_OP_XOR, dest, src);
}

void ezAsmProcedure::bitwise_xor(const ezAddress dest, const ezAddress cond,
                                 vector<ezAddress>& src) {
  instruction_with_array_arguments(EZ_OP_XOR, dest, cond, src);
}

size_t ezAsmProcedure::label2index(string label) {
  if (m_carousel->symtab.find(label) == m_carousel->symtab.end()) {
    m_carousel->symtab[label] = m_carousel->jmptbl.size();
    m_carousel->jmptbl.push_back(0);
  }
  return m_carousel->symtab[label];
}

void ezAsmProcedure::branch_instruction(ezOpCode op, const ezAddress cond,
                                        string label) {
  ezInstEncoder instruction(m_carousel->instruction);
  size_t index = label2index(label);
  instruction.opcode(op, index);
  instruction.argument(cond);
}

void ezAsmProcedure::beq(const ezAddress cond, string label) {
  branch_instruction(EZ_OP_BEQ, cond, label);
}

void ezAsmProcedure::bge(const ezAddress cond, string label) {
  branch_instruction(EZ_OP_BGE, cond, label);
}

void ezAsmProcedure::blt(const ezAddress cond, string label) {
  branch_instruction(EZ_OP_BLT, cond, label);
}

void ezAsmProcedure::bne(const ezAddress cond, string label) {
  branch_instruction(EZ_OP_BNE, cond, label);
}

void ezAsmProcedure::bra(string label) {
  ezInstEncoder instruction(m_carousel->instruction);
  size_t index = label2index(label);
  instruction.opcode(EZ_OP_BRA, index);
}

void ezAsmProcedure::label(string name) {
  size_t index = label2index(name);
  m_carousel->jmptbl[index] = m_carousel->instruction.size();
}

ezASM::ezASM(ezAddress& entry, vector<ezValue*>& constants,
             vector<vector<ezValue*>*>& globals)
    : m_entry(entry), m_constants(constants), m_globals(globals) {
  m_offset_symtab.push_back(new map<string, size_t>);
}

ezASM::~ezASM() {
  for (vector<map<string, size_t>*>::iterator it = m_offset_symtab.begin();
       it != m_offset_symtab.end(); it++)
    if (*it) delete *it;
}

void ezASM::load_intrinsics(const string entry, char** symtab, ezValue** constants) {
  ezLog& log = ezLog::instance();
  if (!symtab || !constants) throw runtime_error(entry + " is not found");
  size_t segment = m_globals.size();
  vector<ezValue*>* offsets = new vector<ezValue*>;
  map<string, size_t>* offset_symtab = new map<string, size_t>;
  for (size_t i = 0; constants[i] && symtab[i]; i++) {
    ezValue* constant = constants[i];
    const char* symbol = symtab[i];
    size_t offset = m_constants.size();
    offsets->push_back(constant);
    ezLog::instance().debug("global[%lu][%lu] = %s", m_globals.size(),
                            offsets->size() - 1, symbol);
    m_constants.push_back(constant);
    (*offset_symtab)[symbol] = offset;
    log.debug("constant[%lu] = %s", offset, symbol);
  }
  if (!offsets->size()) {
    log.debug("no content is found in %s", entry.c_str());
    return;
  }
  m_seg_symtab[entry] = segment;
  m_offset_symtab.push_back(offset_symtab);
  log.debug("global[%lu] = %s", m_globals.size(), entry.c_str());
  m_globals.push_back(offsets);
}

void ezASM::entry(const string entry) { m_entry_string = entry; }

ezAsmProcedure* ezASM::new_proc(const string name, int argc, int retc,
                                int mems) {
  vector<ezValue*>* offset = m_globals[0];
  map<string, size_t>* offset_symtab = m_offset_symtab[0];
  map<string, size_t>::iterator it = offset_symtab->find(name);
  if (it != offset_symtab->end())
    throw runtime_error("global symbol " + name + " already exists");
  ezCarousel* carousel = new ezCarousel(argc, retc, mems);
  carousel->reference();
  m_constants.push_back(carousel);
  carousel->reference();
  offset->push_back(carousel);
  (*offset_symtab)[name] = offset->size() - 1;
  if (name == m_entry_string) {
    m_entry.segment = EZ_ASM_SEGMENT_GLOBAL;
    m_entry.offset = offset->size() - 1;
  }
  return new ezAsmProcedure(carousel);
}

size_t ezASM::global(const string value) {
  //	vector<ezValue*>* offset = m_globals[0];
  map<string, size_t>* offset_symtab = m_offset_symtab[0];
  map<string, size_t>::iterator it = offset_symtab->find(value);
  if (it == offset_symtab->end())
    throw runtime_error("global symbol " + value + " is not found");
  return it->second;
}

size_t ezASM::constant_null(void) {
  for (size_t i = 0; i < m_constants.size(); i++) {
    ezValue* v = m_constants[i];
    if (v->type == EZ_VALUE_TYPE_NULL)
      return i;
  }
  size_t idx = m_constants.size();
  ezValue* v = ezNull::instance();
  v->reference();
  m_constants.push_back(v);
  return idx;
}

size_t ezASM::constant(const char* arg) {
  string value = arg;
  for (size_t i = 0; i < m_constants.size(); i++) {
    ezValue* v = m_constants[i];
    if (v->type == EZ_VALUE_TYPE_STRING && ((ezString*)v)->to_string() == value)
      return i;
  }
  size_t idx = m_constants.size();
  ezValue* v = new ezString(value);
  v->reference();
  m_constants.push_back(v);
  return idx;
}

size_t ezASM::constant(const int value) {
  for (size_t i = 0; i < m_constants.size(); i++) {
    ezValue* v = m_constants[i];
    if (v->type == EZ_VALUE_TYPE_INTEGER &&
        ((ezInteger*)v)->to_integer() == value)
      return i;
  }
  size_t idx = m_constants.size();
  ezValue* v = new ezInteger(value);
  v->reference();
  m_constants.push_back(v);
  return idx;
}

size_t ezASM::constant(const bool value) {
  for (size_t i = 0; i < m_constants.size(); i++) {
    ezValue* v = m_constants[i];
    if (v->type == EZ_VALUE_TYPE_BOOL && ((ezBool*)v)->to_bool() == value)
      return i;
  }
  size_t idx = m_constants.size();
  ezValue* v = new ezBool(value);
  v->reference();
  m_constants.push_back(v);
  return idx;
}

size_t ezASM::constant(const double value) {
  for (size_t i = 0; i < m_constants.size(); i++) {
    ezValue* v = m_constants[i];
    if (v->type == EZ_VALUE_TYPE_FLOAT && ((ezBool*)v)->to_float() == value)
      return i;
  }
  size_t idx = m_constants.size();
  ezValue* v = new ezFloat(value);
  v->reference();
  m_constants.push_back(v);
  return idx;
}

size_t ezASM::offset(const string segment, const string value) {
  map<string, size_t>::iterator sit = m_seg_symtab.find(segment);
  if (sit == m_seg_symtab.end())
    throw runtime_error("a segment of " + segment + " is not found");
  size_t seg = sit->second;
  if (!(m_globals.size() > seg))
    throw runtime_error("invalid segment id for globals : " + seg);
  //	vector<ezValue*>* offset = m_globals[seg];
  if (!(m_offset_symtab.size() > seg))
    throw runtime_error("invalid segment id for offset table : " + seg);
  map<string, size_t>* offset_symtab = m_offset_symtab[seg];
  map<string, size_t>::iterator it = offset_symtab->find(value);
  if (it == offset_symtab->end())
    throw runtime_error("an offset of " + segment + ":" + value +
                        " is not found");
  return it->second;
}

size_t ezASM::segment(const string value) {
  map<string, size_t>::iterator it = m_seg_symtab.find(value);
  if (it == m_seg_symtab.end()) throw runtime_error(value + " not found");
  return it->second;
}

void ezASM::dump(ezFile& sink) {
  sink.print(".symtab:\n");
  sink.print("  segments:\n");
  for (map<string, size_t>::iterator it = m_seg_symtab.begin();
       it != m_seg_symtab.end(); it++)
    sink.print("    %s->%lu\n", it->first.c_str(), it->second);
  sink.print("  offsets\n");
  for (size_t i = 0; i < m_offset_symtab.size(); i++) {
    sink.print("    [%lu]:\n", i);
    map<string, size_t>* offset = m_offset_symtab[i];
    for (map<string, size_t>::iterator it = offset->begin();
         it != offset->end(); it++)
      sink.print("      %s->%lu\n", it->first.c_str(), it->second);
  }
}
