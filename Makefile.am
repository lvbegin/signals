instdir=/usr/local/lib

include_HEADERS=include/signalHandler.h include/signal_handler.h
noinst_HEADERS=include/signalHandlerExecutor.h 
inst_LIBRARIES = libsignals.a 
libsignals_a_SOURCES = src/signalHandlerExecutor.cpp src/signalHandler.cpp src/signalHandlerWrapper.cpp
libsignals_a_CPPFLAGS = -I$(top_srcdir)/include
TESTS = signals_test signals_c_wrapper_test

check_PROGRAMS = signals_test signals_c_wrapper_test

signals_test_SOURCES = test/test.cpp
signals_test_LDFLAGS = -L$(top_srcdir) 
signals_test_LDADD =  -lsignals 
signals_test_CPPFLAGS = -I$(top_srcdir)/include

signals_c_wrapper_test_SOURCES = test/test.c
signals_c_wrapper_test_LDFLAGS = -L$(top_srcdir) 
signals_c_wrapper_test_LDADD =  -lsignals  $(CXXLIB) 
signals_c_wrapper_test_CPPFLAGS = -I$(top_srcdir)/include
