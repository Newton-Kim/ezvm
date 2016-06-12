bin_PROGRAMS=ezas
ezas_SOURCES=ezas.cc ezalu.cc parse.yy scan.ll
ezas_LDADD=../libezvm/libezvm.la -lfl
AM_CPPFLAGS=-I../../include -std=c++11
BUILT_SOURCES=parse.hh
AM_YFLAGS=-d
CLEANFILES=parse.cc scan.cc parse.hh
