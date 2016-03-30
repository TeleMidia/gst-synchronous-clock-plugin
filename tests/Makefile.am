check_PROGRAMS = gstsynchronousclocktest					\
								 gstsynchronousclocktickfortest		\
								 advancetimetest									\
								 tickfortest

AM_CFLAGS = --pedantic -Wall -Werror -std=c99 -Og -I$(top_srcdir)/src \
				 $(GST_CFLAGS) $(GIO_CFLAGS)
AM_LDFLAGS = -L$(top_srcdir)/src/.libs -lgstsynchronousclock $(GST_LIBS) \
						 $(GIO_LIBS) -no-install

gstsynchronousclocktest_SOURCES = gstsynchronousclocktest.c
gstsynchronousclocktest_CFLAGS = $(AM_CFLAGS)
gstsynchronousclocktest_LDFLAGS = $(AM_LDFLAGS)

gstsynchronousclocktickfortest_SOURCES = gstsynchronousclocktickfortest.c
gstsynchronousclocktickfortest_CFLAGS = $(AM_CFLAGS)
gstsynchronousclocktickfortest_LDFLAGS = $(AM_LDFLAGS)

advancetimetest_SOURCES = advance-time-test.c
advancetimetest_CFLAGS = $(AM_CFLAGS)
advancetimetest_LDFLAGS = $(AM_LDFLAGS)

tickfortest_SOURCES = tick-for-test.c
tickfortest_CFLAGS = $(AM_CFLAGS)
tickfortest_LDFLAGS = $(AM_LDFLAGS)

TESTS = advancetimetest
TESTS += tickfortest

noinst_PROGRAMS = gstsynchronousclocktest					\
									gstsynchronousclocktickfortest	\
									advancetimetest									\
									tickfortest
