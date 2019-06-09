lib_LTLIBRARIES=libezvm.la

libezvm_la_SOURCES=ezgc.cc \
		   asm/ezasm.cc \
		   asm/ezasm_proc.cc \
		   asm/ezasm_instr.cc \
		   ezdump.cc \
		   stack.cc \
		   thread.cc \
		   ezmemory.cc \
		   vm.cc \
		   object/ezobj.cc \
		   object/ezobj_function.cc \
		   object/ezobj_condition.cc \
		   object/ezobj_nil.cc \
		   value/ezval.cc \
		   utility/ezfile.cc

includedir=$(prefix)/include/ezvm

nobase_include_HEADERS=../../include/ezvm/ezaddr.h \
		../../include/ezvm/asm/ezasm_instr.h \
		../../include/ezvm/asm/ezasm_proc.h \
		../../include/ezvm/asm/ezasm.h \
		../../include/ezvm/ezdump.h \
		../../include/ezvm/ezfile.h \
		../../include/ezvm/ezgc.h \
		../../include/ezvm/ezinstruction.h \
		../../include/ezvm/ezmemory.h \
		../../include/ezvm/ezstack.h \
		../../include/ezvm/eztable.h \
		../../include/ezvm/ezthread.h \
		../../include/ezvm/ezval.h \
		../../include/ezvm/ezobject.h \
		../../include/ezvm/ezfunc.h \
		../../include/ezvm/ezvm.h

AM_CPPFLAGS=-I../../include -std=c++11
libezvm_la_LDFLAGS=-version-info 1:0:0
