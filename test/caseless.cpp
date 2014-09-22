// Copyright (c) 2014, Michael Schout
// Distributed under a 3-clause BSD license. See LICENSE.
#include <string>
#include <boost/algorithm/string.hpp>
#include <boost/test/unit_test.hpp>

#include "srs2/guarded.hpp"
#include "srs2/reversible.hpp"
#include "srs2/shortcut.hpp"

using std::string;

// these all have an uppercase char so that smashing case does at least
// something.
string tests[] = {
    "User@domain-with-dash.com",
    "User-with-dash@domain.com",
    "User+with+plus@domain.com",
    "User=with=equals@domain.com",
    "User%with!everything&everything=@domain.somewhere"
};

string alias0 = "alias@host.com";
string alias1 = "name@forwarder.com";
string alias2 = "user@postal.com";

void run_test(srs2::base& srs, bool is_shortcut=false)
{
    for (const auto& test : tests) {
        auto srs0addr = srs.forward(test, alias0);
        boost::to_lower(srs0addr);
        auto srs0rev = srs.reverse(srs0addr);
        BOOST_REQUIRE(boost::iequals(srs0rev, test));

        auto srs1addr = srs.forward(srs0addr, alias1);
        boost::to_lower(srs1addr);
        auto srs1rev = srs.reverse(srs1addr);

        if (is_shortcut)
            BOOST_REQUIRE(boost::iequals(srs1rev, test));
        else
            BOOST_REQUIRE(boost::iequals(srs1rev, srs0addr));
    }
}

BOOST_AUTO_TEST_CASE(CaselessGuarded) {
    srs2::guarded srs("foo", '+');
    run_test(srs);
}

BOOST_AUTO_TEST_CASE(CaselessReversible) {
    srs2::reversible srs("foo", '+');
    run_test(srs);
}

BOOST_AUTO_TEST_CASE(CaselessShortcut) {
    srs2::shortcut srs("foo", '+');
    run_test(srs, true);
}


