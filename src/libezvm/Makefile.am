lib_LTLIBRARIES=libezvm.la

libezvm_la_SOURCES=ezgc.cc \
		   ezinstruction.cc \
		   ezasm.cc \
		   ezdump.cc \
		   stack.cc \
		   thread.cc \
		   ezmemory.cc \
		   vm.cc \
		   value/ezval.cc \
		   value/ezval_bool.cc \
		   value/ezval_carousel.cc \
		   value/ezval_complex.cc \
		   value/ezval_condition.cc \
		   value/ezval_float.cc \
		   value/ezval_integer.cc \
		   value/ezval_nil.cc \
		   value/ezval_string.cc \
		   utility/ezfile.cc

includedir=$(prefix)/include/ezvm

include_HEADERS=../../include/ezvm/ezaddr.h \
		../../include/ezvm/ezasm.h \
		../../include/ezvm/ezdump.h \
		../../include/ezvm/ezfile.h \
		../../include/ezvm/ezgc.h \
		../../include/ezvm/ezinstruction.h \
		../../include/ezvm/ezmemory.h \
		../../include/ezvm/ezstack.h \
		../../include/ezvm/eztable.h \
		../../include/ezvm/ezthread.h \
		../../include/ezvm/ezval.h \
		../../include/ezvm/ezvm.h

AM_CPPFLAGS=-I../../include -std=c++11
libezvm_la_LDFLAGS=-version-info 1:0:0
