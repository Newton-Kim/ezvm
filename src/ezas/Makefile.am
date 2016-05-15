bin_PROGRAMS=ezas
ezas_SOURCES=ezas.cc parse.yy scan.ll
ezas_LDADD=../libezvm/libezvm.la -lfl
AM_CPPFLAGS=-I../../include
BUILT_SOURCES=parse.hh
AM_YFLAGS=-d
CLEANFILES=parse.cc scan.cc parse.hh
