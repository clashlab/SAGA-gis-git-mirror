#
# $Id: Makefile.am 911 2011-02-14 16:38:15Z reklov_w $
#
if DEBUG
DBGFLAGS = -g -DDEBUG
endif

DEF_SAGA           = -D_SAGA_LINUX -D_TYPEDEF_BYTE -D_TYPEDEF_WORD -D_SAGA_UNICODE
CXX_INCS           = -I$(top_srcdir)/src/saga_core
AM_CXXFLAGS        = -fPIC $(CXX_INCS) $(DEF_SAGA) $(DBGFLAGS) $(GOMPFLAGS)
AM_LDFLAGS         = -fPIC -shared -avoid-version
pkglib_LTLIBRARIES = libta_channels.la
libta_channels_la_SOURCES =\
ChannelNetwork.cpp\
ChannelNetwork_Altitude.cpp\
ChannelNetwork_Distance.cpp\
D8_Flow_Analysis.cpp\
MLB_Interface.cpp\
Strahler.cpp\
Watersheds.cpp\
Watersheds_ext.cpp\
ChannelNetwork.h\
ChannelNetwork_Altitude.h\
ChannelNetwork_Distance.h\
D8_Flow_Analysis.h\
MLB_Interface.h\
Strahler.h\
Watersheds.h\
Watersheds_ext.h

libta_channels_la_LIBADD = $(top_srcdir)/src/saga_core/saga_api/libsaga_api.la

