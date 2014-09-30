AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES = libPD3D.la

libPD3D_la_CXXFLAGS = @PD_CPPFLAGS@ -s -O2 -fPIC -fpermissive 
#~libPD3D_la_LDFLAGS = -module -avoid-version -shared -shrext .@EXTENSION@  @PD_LDFLAGS@ -fPIC
libPD3D_la_LDFLAGS =
libPD3D_la_LIBADD = 

libPD3D_la_SOURCES = Hoa3D.pd.h \
 hoa.3d.decoder_tilde.cpp \
 hoa.3d.encoder_tilde.cpp \
 hoa.3d.map_tilde.cpp \
 hoa.3d.meter_gui_tilde.cpp \
 hoa.3d.optim_tilde.cpp \
 hoa.3d.scope_gui_tilde.cpp \
 hoa.3d.wider_tilde.cpp


