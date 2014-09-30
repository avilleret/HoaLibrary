AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES = libPD2D.la

libPD2D_la_CXXFLAGS = @PD_CPPFLAGS@ -s -O2 -fPIC -fpermissive 
libPD2D_la_LDFLAGS = -module -avoid-version -shared -shrext .@EXTENSION@  @PD_LDFLAGS@ -fPIC
libPD2D_la_LIBADD = 

libPD2D_la_SOURCES = Hoa2D.pd.h \
 hoa.2d.decoder_tilde.cpp \
 hoa.2d.encoder_tilde.cpp \
 hoa.2d.map_tilde.cpp \
 hoa.2d.meter_gui_tilde.cpp \
 hoa.2d.optim_tilde.cpp \
 hoa.2d.projector_tilde.cpp \
 hoa.2d.recomposer_tilde.cpp \
 hoa.2d.rotate_tilde.cpp \
 hoa.2d.scope_gui_tilde.cpp \
 hoa.2d.space_gui.cpp \
 hoa.2d.wider_tilde.cpp

