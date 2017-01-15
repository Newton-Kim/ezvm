lib_LTLIBRARIES=libezvm.la

libezvm_la_SOURCES=ezarchive.cc \
		   ezalu.cc \
		   ezgc.cc \
		   ezasm.cc \
		   ezdump.cc \
		   ezinstruction.cc \
		   ezval.cc \
		   stack.cc \
		   thread.cc \
		   vm.cc \
		   utility/ezfile.cc \
		   utility/ezlog.cc

includedir=$(prefix)/include/ezvm

include_HEADERS=../../include/ezvm/ezaddr.h \
		../../include/ezvm/ezalu.h \
		../../include/ezvm/ezarchive.h \
		../../include/ezvm/ezasm.h \
		../../include/ezvm/ezdump.h \
		../../include/ezvm/ezfile.h \
		../../include/ezvm/ezinstruction.h \
		../../include/ezvm/ezlog.h \
		../../include/ezvm/ezstack.h \
		../../include/ezvm/ezthread.h \
		../../include/ezvm/ezval.h \
		../../include/ezvm/ezvm.h

AM_CPPFLAGS=-I../../include -std=c++11
libezvm_la_LDFLAGS=-version-info 1:2:0
