AM_CPPFLAGS = -I../src @BOOST_CPPFLAGS@
AM_LDFLAGS = -L../src/.libs -lsrs2xx @BOOST_UNIT_TEST_FRAMEWORK_LDFLAGS@ @BOOST_UNIT_TEST_FRAMEWORK_LIBS@

EXTRA_DIST = fixtures

noinst_PROGRAMS = test

test_SOURCES = run.cpp \
	basic.cpp \
	base.cpp \
	guarded.cpp \
	shortcut.cpp \
	reversible.cpp \
	separator.cpp \
	varysep.cpp \
	caseless.cpp
