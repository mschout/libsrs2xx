# Libsrs2xx

## Welcome

Libsrs2xx is a C++11 implementation of the Mail::SRS implementation of Email
Sender Rewriting Scheme (SRS).

## Building and installation

The following packages are required to build libsrs2xx:

- A C++11 compliant compiler
- Boost libraries (only needed to build and run tests, not needed at runtime)
- GNU Autoconf and automake

If building from git, you must run `./autogen.sh' first

```
./configure
make
make install
```

If you wish to run the tests, simply run the `./test/test' binary after `make'

## Copying

This software is licensed under the BSD 3-Clause license.  For details see the
file LICENSE included with the distribution.

