instdir=/usr/local/lib

include_HEADERS=include/signals.h
inst_LIBRARIES = libsignals.a
libsignals_a_SOURCES = src/signals.cpp
libsignals_a_CPPFLAGS = -I$(top_srcdir)/include
libsignals_a_CXXFLAGS=-O3 -std=c++14
TESTS = signals_test

check_PROGRAMS = signals_test

signals_test_SOURCES = test/test.cpp
signals_test_LDFLAGS = -L$(top_srcdir) 
signals_test_LDADD =  -lsignals
signals_test_CPPFLAGS = -I$(top_srcdir)/include
signals_test_CXXFLAGS = -std=c++14
