bin_PROGRAMS=ezas
ezas_SOURCES=ezas.cc \
		ezio.cc \
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
AM_YFLAGS=-d --warning=all
CLEANFILES=parse.cc scan.cc parse.hh parse.output
