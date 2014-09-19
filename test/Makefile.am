AM_CPPFLAGS = -I../src @BOOST_CPPFLAGS@
AM_LDFLAGS = -L../src/.libs -lsrs2xx @BOOST_UNIT_TEST_FRAMEWORK_LDFLAGS@ @BOOST_UNIT_TEST_FRAMEWORK_LIBS@

noinst_PROGRAMS = test

test_SOURCES = run.cpp \
	basic.cpp \
	guarded.cpp \
	shortcut.cpp
