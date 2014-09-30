AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I m4

noinst_LTLIBRARIES = libhoa.la

libhoa_la_CXXFLAGS = -O2 -fPIC -fpermissive 
libhoa_la_LDFLAGS =
libhoa_la_LIBADD = Hoa2D/libHoa2D.la Hoa3D/libHoa3D.la HoaCommon/libHoaCommon.la

libhoa_la_SOURCES = HoaDefs.h \
 Hoa.h \
 HoaMath.h \
 HoaUtils.h
 
SUBDIRS = Hoa2D Hoa3D HoaCommon

libhoadir=$(pkglibdir)


