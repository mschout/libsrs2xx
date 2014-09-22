// Copyright (c) 2014, Michael Schout
// Distributed under a 3-clause BSD license. See LICENSE.
#include <vector>
#include <string>
#include <boost/test/unit_test.hpp>

// expose guts for testing
#define protected public
#define private   public

#include "srs2/guarded.hpp"

using std::string;

BOOST_AUTO_TEST_CASE(IsSRS) {
    srs2::guarded srs("foo");

    string addr = "user@domain-with-dash.com";

    auto alias0 = srs.forward(addr, "foo.com");
    auto alias1 = srs.forward(alias0, addr);

    //BOOST_TEST_MESSAGE(alias0);
    //BOOST_TEST_MESSAGE(alias1);
    BOOST_REQUIRE(srs.is_srs(alias0));
    BOOST_REQUIRE(srs.is_srs(alias1));
    BOOST_REQUIRE(srs.is_srs0(alias0));
    BOOST_REQUIRE(srs.is_srs1(alias1));
    BOOST_REQUIRE(alias0[4] == '=');
    BOOST_REQUIRE(alias1[4] == '=');

    alias0[4] = '-';
    alias1[4] = '-';
    BOOST_REQUIRE(srs.is_srs(alias0));
    BOOST_REQUIRE(srs.is_srs(alias1));
    BOOST_REQUIRE(srs.is_srs0(alias0));
    BOOST_REQUIRE(srs.is_srs1(alias1));

    alias0[4] = '+';
    alias1[4] = '+';
    BOOST_REQUIRE(srs.is_srs(alias0));
    BOOST_REQUIRE(srs.is_srs(alias1));
    BOOST_REQUIRE(srs.is_srs0(alias0));
    BOOST_REQUIRE(srs.is_srs1(alias1));
}

BOOST_AUTO_TEST_CASE(RemoveTag) {
    srs2::guarded srs("foo");

    string addr = "user@domain-with-dash.com";
    auto alias0 = srs.forward(addr, "foo.com");
    auto alias1 = srs.forward(alias0, addr);

    BOOST_CHECK_EQUAL(alias0.substr(0,4).compare("SRS0"),0);
    BOOST_CHECK_EQUAL(alias1.substr(0,4).compare("SRS1"),0);

    srs.remove_tag(alias0);
    srs.remove_tag(alias1);

    BOOST_REQUIRE(alias0.substr(0,4).compare("SRS0") != 0);
    BOOST_REQUIRE(alias0[0] != '=');
    BOOST_REQUIRE(alias1.substr(0,4).compare("SRS1") != 0);
    BOOST_REQUIRE(alias1[0] != '=');
}

