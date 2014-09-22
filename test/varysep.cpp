// Copyright (c) 2014, Michael Schout
// Distributed under a 3-clause BSD license. See LICENSE.

#include <vector>
#include <string>
#include <boost/test/unit_test.hpp>

#include "srs2/reversible.hpp"
#include "srs2/guarded.hpp"
#include "srs2/shortcut.hpp"

struct Addresses {
    std::vector<std::string> aliases;
    std::string source;

    Addresses() : source("user@host.tld")
    {
        aliases.push_back("user@domain-with-dash.com");
        aliases.push_back("user-with-dash@domain.com");
        aliases.push_back("user+with+plus@domain.com");
        aliases.push_back("user=with=equals@domain.com");
        aliases.push_back("user%with!everything&everything=@domain.somewhere");
    }

    ~Addresses() { }
};

BOOST_FIXTURE_TEST_SUITE(Reversible, Addresses);

BOOST_AUTO_TEST_CASE(Guarded) {
    auto srs0 = srs2::guarded("foo", '+');
    auto srs1 = srs2::guarded("foo", '-');
    auto srs2 = srs2::guarded("foo", '=');

    std::string alias0 = "alias@host.com";
    std::string alias1 = "name@forwarder.com";
    std::string alias2 = "user@postal.com";

    for (auto alias : aliases) {
        BOOST_TEST_MESSAGE(alias);
        BOOST_TEST_MESSAGE(alias0);
        auto srs0addr = srs0.forward(alias, alias0);
        BOOST_TEST_MESSAGE(srs0addr);
        auto srs0rev = srs0.reverse(srs0addr);
        BOOST_TEST_MESSAGE("reverse done");

        // Idempotent
        BOOST_CHECK_EQUAL(srs0rev.compare(alias), 0);

        auto srs1addr = srs1.forward(srs0addr, alias1);
        auto srs1rev = srs1.reverse(srs1addr);
        BOOST_CHECK_EQUAL(srs1rev.compare(srs0addr), 0);

        auto srs2addr = srs2.forward(srs1addr, alias2);
        auto srs2rev = srs2.reverse(srs2addr);
        BOOST_CHECK_EQUAL(srs2rev.compare(srs0addr), 0);
    }
}

BOOST_AUTO_TEST_CASE(Reversible) {
    auto srs0 = srs2::reversible("foo", '+');
    auto srs1 = srs2::reversible("foo", '-');
    auto srs2 = srs2::reversible("foo", '=');

    std::string alias0 = "alias@host.com";
    std::string alias1 = "name@forwarder.com";
    std::string alias2 = "user@postal.com";

    for (auto alias : aliases) {
        BOOST_TEST_MESSAGE(alias);
        BOOST_TEST_MESSAGE(alias0);
        auto srs0addr = srs0.forward(alias, alias0);
        BOOST_TEST_MESSAGE(srs0addr);
        auto srs0rev = srs0.reverse(srs0addr);
        BOOST_TEST_MESSAGE("reverse done");

        // Idempotent
        BOOST_CHECK_EQUAL(srs0rev.compare(alias), 0);

        auto srs1addr = srs1.forward(srs0addr, alias1);
        auto srs1rev = srs1.reverse(srs1addr);
        BOOST_CHECK_EQUAL(srs1rev.compare(srs0addr), 0);

        auto srs2addr = srs2.forward(srs1addr, alias2);
        auto srs2rev = srs2.reverse(srs2addr);
        BOOST_CHECK_EQUAL(srs2rev.compare(srs1addr), 0);
    }
}

BOOST_AUTO_TEST_CASE(Shortcut) {
    auto srs0 = srs2::shortcut("foo", '+');
    auto srs1 = srs2::shortcut("foo", '-');
    auto srs2 = srs2::shortcut("foo", '=');

    std::string alias0 = "alias@host.com";
    std::string alias1 = "name@forwarder.com";
    std::string alias2 = "user@postal.com";

    for (auto alias : aliases) {
        auto srs0addr = srs0.forward(alias, alias0);
        auto srs0rev = srs0.reverse(srs0addr);

        // Idempotent
        BOOST_CHECK_EQUAL(srs0rev.compare(alias), 0);

        auto srs1addr = srs1.forward(srs0addr, alias1);
        auto srs1rev = srs1.reverse(srs1addr);
        BOOST_CHECK_EQUAL(srs1rev.compare(alias), 0);
    }
}

BOOST_AUTO_TEST_SUITE_END();
