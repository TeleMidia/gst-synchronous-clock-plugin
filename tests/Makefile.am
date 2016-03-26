bin_PROGRAMS = gstsynchronousclocktest
gstsynchronousclocktest_CFLAGS = --pedantic -Wall -std=c99 -O2	\
																 -I$(top_srcdir)/src						\
																 $(GST_CFLAGS)

gstsynchronousclocktest_LDFLAGS = $(GST_LIBS)										\
																 -L$(top_srcdir)/src/.libs			\
																 -lgstsynchronousclock

gstsynchronousclocktest_SOURCES = gstsynchronousclocktest.c

