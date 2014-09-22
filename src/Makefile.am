AM_CPPFLAGS = @BOOST_CPPFLAGS@ @OPENSSL_INCLUDES@
AM_LDFLAGS = @OPENSSL_LDFLAGS@ -lcrypto

lib_LTLIBRARIES = libsrs2xx.la

libsrs2xx_la_SOURCES = \
	base.cpp \
	shortcut.cpp \
	guarded.cpp \
	reversible.cpp \
	split.cpp \
	base64.cpp

# install these headers
srs2headersdir = $(includedir)/srs2

srs2headers_HEADERS = \
	srs2/base.hpp \
	srs2/address.hpp \
	srs2/guarded.hpp \
	srs2/reversible.hpp \
	srs2/shortcut.hpp \
	srs2/srs2.hpp
