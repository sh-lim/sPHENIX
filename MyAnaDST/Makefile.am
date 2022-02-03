AUTOMAKE_OPTIONS = foreign

AM_CPPFLAGS = \
  -I$(includedir) \
  -I$(OFFLINE_MAIN)/include \
  -I`root-config --incdir`

lib_LTLIBRARIES = \
  libMyAnaDst.la

pkginclude_HEADERS = \
	MyAnaDstBasic.h

libMyAnaDst_la_LDFLAGS = \
  -L$(libdir) \
  -L$(OFFLINE_MAIN)/lib \
  -L$(OFFLINE_MAIN)/lib64 \
  `root-config --libs`

libMyAnaDst_la_LIBADD = \
  -lfun4all \
  -lg4dst \
  -lg4eval \
	-lphhepmc

if ! MAKEROOT6
ROOT5_DICTS = \
	MyAnaDstBasicDict.cc
endif

libMyAnaDst_la_SOURCES = \
	$(ROOT5_DICTS) \
	MyAnaDstBasic.cc

install-exec-hook:
	@[ $$OSTYPE = darwin ] && ln -sf $(DESTDIR)$(libdir)/libMyAnaDst.dylib $(DESTDIR)$(libdir)/libMyAnaDst.so || true

BUILT_SOURCES = \
  testexternals.C

testexternals_SOURCES = \
  testexternals.C

noinst_PROGRAMS = \
  testexternals

testexternals_LDADD = \
  libMyAnaDst.la

testexternals.C:
	echo "//*** this is a generated file. Do not commit, do not edit" > $@
	echo "int main()" >> $@
	echo "{" >> $@
	echo "  return 0;" >> $@
	echo "}" >> $@

# Rule for generating CINT dictionaries from class headers.
%_Dict.cc: %.h %LinkDef.h
	rootcint -f $@ @CINTDEFS@ -c $(DEFAULT_INCLUDES) $(AM_CPPFLAGS) $^

clean-local:
	rm -f *Dict* $(BUILT_SOURCES)
