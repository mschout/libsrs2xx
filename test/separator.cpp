// Copyright (c) 2014, Michael Schout
// Distributed under a 3-clause BSD license. See LICENSE.
#include <string>
#include <boost/test/unit_test.hpp>

#include "fixtures/addresses.hpp"
#include "srs2/guarded.hpp"

using std::string;

BOOST_FIXTURE_TEST_SUITE(Separator, Addresses);

BOOST_AUTO_TEST_CASE(Separator) {
    char chars[] = { '-', '+', '=' };

    for (const auto separator : chars) {
        srs2::guarded srs("foo", separator);
        BOOST_REQUIRE(srs.separator() == separator);

        auto srs0 = srs.forward(source, aliases[0]);
        BOOST_REQUIRE(srs.is_srs0(srs0));

        auto old0 = srs.reverse(srs0);
        BOOST_REQUIRE(!old0.empty());
        BOOST_CHECK_EQUAL(old0.compare(source), 0);

        auto srs1 = srs.forward(srs0, aliases[1]);
        BOOST_REQUIRE(!srs1.empty());
        BOOST_REQUIRE(srs.is_srs1(srs1));

        auto old1 = srs.reverse(srs1);
        BOOST_REQUIRE(!old1.empty());
        BOOST_REQUIRE(srs.is_srs0(old1));
        BOOST_CHECK_EQUAL(old1.compare(srs0), 0);
        auto orig = srs.reverse(old1);
        BOOST_CHECK_EQUAL(orig.compare(source), 0);

        string tests[] = {
            "user@domain-with-dash.com",
            "user-with-dash@domain.com",
            "user+with+plus@domain.com",
            "user=with=equals@domain.com",
            "user%with!everything&everything=@domain.somewhere"
        };

        for (const auto& test : tests) {
            auto srs0addr = srs.forward(test, aliases[0]);
            auto oldaddr = srs.reverse(srs0addr);
            BOOST_CHECK_EQUAL(oldaddr.compare(test), 0);

            auto srs1addr = srs.forward(srs0addr, aliases[1]);
            auto srs0rev = srs.reverse(srs1addr);
            BOOST_CHECK_EQUAL(srs0rev.compare(srs0addr), 0);
        }
    }
}

BOOST_AUTO_TEST_SUITE_END();
