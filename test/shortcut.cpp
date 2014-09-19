#include <vector>
#include <string>
#include <boost/test/unit_test.hpp>

// expose guts for testing
#define protected public
#define private   public

#include "srs2/shortcut.hpp"

struct Addresses
{
    std::vector<std::string> aliases;
    std::string source;

    Addresses() : source("user@host.tld")
    {
        BOOST_TEST_MESSAGE("build addresses");
        for (int i = 0; i <= 5; i++) {
            auto id = std::to_string(i);
            aliases.push_back(std::string("alias") + id + "@host" + id + ".tld" + id);
        }
    }

    ~Addresses()
    {
        BOOST_TEST_MESSAGE("teardown addresses");
    }
};

BOOST_FIXTURE_TEST_SUITE(Shortcut, Addresses);

BOOST_AUTO_TEST_CASE(Constructor) {
    srs2::shortcut srs("foo");

    BOOST_CHECK_EQUAL(srs.secret().compare("foo"), 0);
}

BOOST_AUTO_TEST_CASE(Forward) {
    srs2::shortcut srs("foo");

    auto new0 = srs.forward(source, aliases[0]);

    // made a new address
    BOOST_REQUIRE(!new0.empty());

    // its an srs address
    BOOST_CHECK_EQUAL(new0.substr(0,4).compare("SRS0"), 0);

    auto old0 = srs.reverse(new0);
    // Reversed the address
    BOOST_REQUIRE(!old0.empty());

    // reversal was idempotent
    BOOST_CHECK_EQUAL(old0.compare(source), 0); 

    auto new1 = srs.forward(new0, aliases[1]);
    // made another address
    BOOST_REQUIRE(!new1.empty());

    // It is an SRS address
    BOOST_CHECK_EQUAL(new1.substr(0,3).compare("SRS"), 0);

    auto old1 = srs.reverse(new1);
    BOOST_REQUIRE(!old1.empty());

    // got back the original sender
    BOOST_CHECK_EQUAL(old1.compare(source), 0);
}

BOOST_AUTO_TEST_CASE(Usernames) {
    srs2::shortcut srs("foo");

    std::string tests[] = {
        "user@domain-with-dash.com",
        "user-with-dash@domain.com",
        "user+with+plus@domain.com",
        "user%with!everything&everything=@domain.somewhere"
    };

    const std::string alias = "alias@host.com";

    for (const auto& email : tests) {
        auto srsaddr = srs.forward(email, alias);
        BOOST_REQUIRE(!srsaddr.empty());
        auto oldaddr = srs.reverse(srsaddr);
        BOOST_REQUIRE(!oldaddr.empty());

        BOOST_CHECK_EQUAL(oldaddr.compare(email), 0);

    }
}


BOOST_AUTO_TEST_SUITE_END();
