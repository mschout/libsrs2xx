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

## Usage

For the Guarded scheme (the default for Mail::SRS reference implementation):

```
#include <srs2/srs2.hpp>
....
srs2::guarded srs("my secret");

std::string srsaddress = srs.forward("user@nowhere.com", "bounces.forwarder.com");

std::string orig = srs.reverse(srsaddress);
```

The other schemes available can be used by simply using a different class other
than srs2::guarded.  The available schemes are:

  - Shortcut: implemented in `srs2::shortcut'
  - Reversible: implemented in `srs2::reversible'

## Copying

This software is licensed under the BSD 3-Clause license.  For details see the
file LICENSE included with the distribution.

