bin_PROGRAMS = gstsynchronousclocktest advancetimetest

AM_CFLAGS = --pedantic -Wall -Werror -std=c99 -Og -I$(top_srcdir)/src \
				 $(GST_CFLAGS)
AM_LDFLAGS = -L$(top_srcdir)/src/.libs -lgstsynchronousclock $(GST_LIBS)

gstsynchronousclocktest_SOURCES = gstsynchronousclocktest.c
gstsynchronousclocktest_CFLAGS = $(AM_CFLAGS)
gstsynchronousclocktest_LDFLAGS = $(AM_LDFLAGS)

advancetimetest_SOURCES = advance-time-test.c
advancetimetest_CFLAGS = $(AM_CFLAGS)
advancetimetest_LDFLAGS = $(AM_LDFLAGS)

