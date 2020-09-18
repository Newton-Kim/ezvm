lib_LTLIBRARIES=libezvm.la

libezvm_la_SOURCES=ezgc.cc \
		   ezalu.cc \
		   ezsegment.cc \
		   stack.cc \
		   thread.cc \
		   ezmemory.cc \
		   vm.cc \
		   object/ezobj.cc \
		   object/ezobj_function.cc \
		   object/ezobj_condition.cc \
		   object/ezobj_nil.cc \
		   object/ezobj_array.cc \
		   value/ezval.cc \
		   utility/ezfile.cc \
		   utility/ezlog.cc \
		   asm/ezasm.cc \
		   asm/ezasm_proc.cc \
		   asm/ezasm_instr.cc \
		   asm/instruction/ezinst_mv.cc \
		   asm/instruction/ezinst_cmp.cc \
		   asm/instruction/ezinst_call.cc \
		   asm/instruction/ezinst_get.cc \
		   asm/instruction/ezinst_set.cc \
		   asm/instruction/ezinst_dup.cc \
		   asm/instruction/ezinst_conditional_bra.cc \
		   asm/instruction/ezinst_unary_operation.cc \
		   asm/instruction/ezinst_binary_operation.cc

includedir=$(prefix)/include/ezvm

nobase_include_HEADERS=../../include/ezvm/ezaddr.h \
		../../include/ezvm/asm/ezasm_instr.h \
		../../include/ezvm/asm/ezasm_proc.h \
		../../include/ezvm/asm/ezasm.h \
		../../include/ezvm/ezfile.h \
		../../include/ezvm/ezgc.h \
		../../include/ezvm/ezalu.h \
		../../include/ezvm/ezsegment.h \
		../../include/ezvm/ezinstruction.h \
		../../include/ezvm/ezmemory.h \
		../../include/ezvm/ezstack.h \
		../../include/ezvm/eztable.h \
		../../include/ezvm/ezthread.h \
		../../include/ezvm/ezval.h \
		../../include/ezvm/ezobject.h \
		../../include/ezvm/ezfunc.h \
		../../include/ezvm/ezlog.h \
		../../include/ezvm/ezvm.h

AM_CPPFLAGS=-I../../include -std=c++11 -gstabs -UEZ_LOG_ENABLED
libezvm_la_LDFLAGS=-version-info 1:0:0
