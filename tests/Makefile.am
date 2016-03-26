bin_PROGRAMS = gstsynchronousclocktest advancetimetest

CFLAGS = --pedantic -Wall -Werror -std=c99 -Og -I$(top_srcdir)/src \
				 $(GST_CFLAGS)
LDFLAGS = -L$(top_srcdir)/src/.libs -lgstsynchronousclock $(GST_LIBS)

gstsynchronousclocktest_SOURCES = gstsynchronousclocktest.c
gstsynchronousclocktest_CFLAGS = $(CFLAGS)
gstsynchronousclocktest_LDFLAGS = $(LDFLAGS)

advancetimetest_SOURCES = advance-time-test.c
advancetimetest_CFLAGS = $(CFLAGS)
advancetimetest_LDFLAGS = $(LDFLAGS)

