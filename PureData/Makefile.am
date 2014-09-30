AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I m4

pkglib_LTLIBRARIES = hoa.la

hoa_la_CXXFLAGS = @PD_CPPFLAGS@ -s -O2 -fPIC -fpermissive 
hoa_la_LDFLAGS = -module -avoid-version -shared -shrext .@EXTENSION@  @PD_LDFLAGS@ -fPIC
#~hoa_la_LIBADD = ../Sources/Hoa2D/libHoa2D.la ../Sources/Hoa3D/libHoa3D.la ../Sources/HoaCommon/libHoaCommon.la
hoa_la_LIBADD = ../Sources/libhoa.la
hoa_la_LIBADD += PD2D/libPD2D.la PD3D/libPD3D.la PDCommon/libPDCommon.la
hoa_la_LIBADD += ../ThirdParty/PureData/Sources/libCicmWrapper.la
hoa_la_LDFLAGS += ../ThirdParty/CBlas/Linux64/lib/libopenblas.a

hoa_la_SOURCES = hoa.pd.h \
 hoa.pd_commonsyms.h \
 hoa.pd.cpp \
 hoa.library.cpp

ALL_CFLAGS = -I"$(PD_INCLUDE)"

hoadir=$(pkglibdir)

dist_hoa_DATA = \
	hoa-meta.pd \
	README.txt
	
EXTRA_DIST = autogen.sh

DISTDIR=$(PACKAGE_NAME)-$(PACKAGE_VERSION)
ORIGDIR=pd-$(PACKAGE_NAME)_$(PACKAGE_VERSION)
OS=$(shell uname -s)
DISTBINDIR=$(DISTDIR)-$(OS)

SUBDIRS= PD2D PD3D PDCommon
SUBDIRS+= Package/HoaLibrary

#~SUBDIRS+= ../Sources/Hoa2D ../Sources/Hoa3D ../Sources/HoaCommon
#~SUBDIRS+= ../ThirdParty/PureData
	
libdir: all
	$(INSTALL) -p -m 755 -d $(DISTBINDIR)
	$(INSTALL_DATA) $(PACKAGE_NAME)-meta.pd  $(DISTBINDIR)
	$(INSTALL_DATA) $(PACKAGE_NAME).$(EXTENSION)  $(DISTBINDIR)
	$(INSTALL_DATA) $(dist_hoa_DATA) $(DISTBINDIR)
	test -z "$(strip $(EXTRA_DIST))" || \
		$(INSTALL_DATA) $(EXTRA_DIST)    $(DISTBINDIR)
#	tar --exclude-vcs -czpf $(DISTBINDIR).tar.gz $(DISTBINDIR)

$(abs_builddir)/.libs/hoa.@EXTENSION@: hoa.la

$(top_builddir)/hoa.@EXTENSION@: $(abs_builddir)/.libs/hoa.@EXTENSION@ 
		rm -f $@ && cd $(top_builddir) && test -e $< && $(LN_S) $< hoa.@EXTENSION@ || true

.PHONY: clean-conveniencelink help

clean-conveniencelink:
	test -L $(top_builddir)/hoa.@EXTENSION@ && rm -f $(top_builddir)/hoa.@EXTENSION@ || true


all-local:: $(top_builddir)/hoa.@EXTENSION@ help

clean-local:: clean-conveniencelink

.o: .cc
	$(CXX) $(ALL_CFLAGS) -o "$*.o" -c "$*.cc"
	
README.txt:
	cp README.md README.txt
