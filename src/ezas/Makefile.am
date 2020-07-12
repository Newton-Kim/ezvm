bin_PROGRAMS=ezas
ezas_SOURCES=ezas.cc \
		ezio.cc \
		alu/eaalu_state.cc \
		alu/eaalu.cc \
		alu/eabool.cc \
		alu/eacplx.cc \
		alu/eafloat.cc \
		alu/eaint.cc \
		alu/eastring.cc \
		parse.yy \
		scan.ll \
		value/eaval.cc \
		value/eaval_bool.cc \
		value/eaval_complex.cc \
		value/eaval_float.cc \
		value/eaval_integer.cc \
		value/eaval_string.cc

ezas_LDADD=../libezvm/libezvm.la -lfl
AM_CPPFLAGS=-I../../include -std=c++11
BUILT_SOURCES=parse.hh
AM_YFLAGS=-Wno-yacc -d --warning=all
CLEANFILES=parse.cc scan.cc parse.hh parse.output
