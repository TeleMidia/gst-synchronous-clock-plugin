# Note: plugindir is set in configure

##############################################################################
# TODO: change libgstplugin.la to something else, e.g. libmysomething.la     #
##############################################################################
plugin_LTLIBRARIES = libgstsynchronousclock.la

##############################################################################
# TODO: for the next set of variables, name the prefix if you named the .la, #
#  e.g. libmysomething.la => libmysomething_la_SOURCES                       #
#                            libmysomething_la_CFLAGS                        #
#                            libmysomething_la_LIBADD                        #
#                            libmysomething_la_LDFLAGS                       #
##############################################################################

# sources used to compile this plug-in
libgstsynchronousclock_la_SOURCES = gstsynchronousclock.c gstsynchronousclock.h

# compiler and linker flags used to compile this plugin, set in configure.ac
libgstsynchronousclock_la_CFLAGS = $(GST_CFLAGS)
libgstsynchronousclock_la_LIBADD = $(GST_LIBS)
libgstsynchronousclock_la_LDFLAGS = $(GST_PLUGIN_LDFLAGS)
libgstsynchronousclock_la_LIBTOOLFLAGS = --tag=disable-static

include_HEADERS = gstsynchronousclock.h

