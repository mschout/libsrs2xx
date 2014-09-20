// Copyright (c) 2014, Michael Schout
// Distributed under a 3-clause BSD license. See LICENSE.

#include <vector>
#include <string>
#include <boost/test/unit_test.hpp>

#include "fixtures/addresses.hpp"

// expose guts for testing
#define protected public
#define private   public

#include "srs2/reversible.hpp"

BOOST_FIXTURE_TEST_SUITE(Reversible, Addresses);

BOOST_AUTO_TEST_CASE(Constructor) {
    srs2::reversible srs("foo");
    BOOST_REQUIRE(!srs.secret().empty());
}

BOOST_AUTO_TEST_CASE(Forward) {
    srs2::reversible srs("foo");

    auto new0 = srs.forward(aliases[0], aliases[1]);
    // made an SRS address
    BOOST_REQUIRE(!new0.empty());

    BOOST_TEST_MESSAGE(new0);

    // It is an SRS address
    BOOST_CHECK_EQUAL(new0.substr(0,3).compare("SRS"), 0);

    auto old0 = srs.reverse(new0);

    // reversed the address
    BOOST_REQUIRE(!old0.empty());

    // reversal was idempotent
    BOOST_CHECK_EQUAL(old0.compare(aliases[0]), 0);

    auto new1 = srs.forward(new0, aliases[2]);
    BOOST_TEST_MESSAGE(std::string("Composed is ") + new1);

    // Made another address with the SRS addresss
    BOOST_REQUIRE(!new1.empty());

    // It is an SRS address
    BOOST_CHECK_EQUAL(new1.substr(0,3).compare("SRS"), 0);

    auto old1 = srs.reverse(new1);

    // reversed it again
    BOOST_REQUIRE(!old1.empty());

    // Reversal was idempotent again
    BOOST_CHECK_EQUAL(old1.compare(new0), 0);
}

BOOST_AUTO_TEST_SUITE_END();
