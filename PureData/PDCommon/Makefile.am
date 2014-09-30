AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES = libPDCommon.la

libPDCommon_la_CXXFLAGS = @PD_CPPFLAGS@ -s -O2 -fPIC -fpermissive 
#~libPDCommon_la_LDFLAGS = -module -avoid-version -shared -shrext .@EXTENSION@  @PD_LDFLAGS@ -fPIC
libPDCommon_la_LDFLAGS =
libPDCommon_la_LIBADD = 

libPDCommon_la_SOURCES = HoaCommon.pd.h \
 hoa.connect.cpp \
 hoa.converter~.cpp \
 hoa.dac_tilde.cpp \
 hoa.in.cpp \
 hoa.in_tilde.cpp \
 hoa.map_gui.cpp \
 hoa.out.cpp \
 hoa.out_tilde.cpp \
 hoa.pi.cpp \
 hoa.pi_tilde.cpp \
 hoa.process_tilde.cpp \
 hoa.thisprocess_tilde.cpp


