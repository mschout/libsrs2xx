#include <vector>
#include <string>
#include <boost/test/unit_test.hpp>
#include "fixtures/addresses.hpp"

// expose guts for testing
#define protected public
#define private   public

#include "srs2/guarded.hpp"

BOOST_FIXTURE_TEST_SUITE(Guarded, Addresses);

BOOST_AUTO_TEST_CASE(forward) {
    srs2::guarded srs("foo");
    BOOST_REQUIRE(!srs.secret().empty());

    auto srs0 = srs.forward(source, aliases.front());

    // check that we got an address back
    BOOST_REQUIRE(!srs0.empty());

    // check that its an SRS0 address
    BOOST_CHECK_EQUAL(srs0.substr(0,4).compare("SRS0"), 0);

    auto old0 = srs.reverse(srs0);

    // reversed the address
    BOOST_REQUIRE(!old0.empty());

    // reversal was idempotent
    BOOST_CHECK_EQUAL(old0.compare(source), 0);

    auto srs1 = srs.forward(srs0, aliases[1]);
    BOOST_TEST_MESSAGE(std::string("composed is ") + srs1);

    BOOST_REQUIRE(!srs1.empty());

    // its an SRS1 address
    BOOST_CHECK_EQUAL(srs1.substr(0,4).compare("SRS1"), 0);

    // its not an SRS0 address
    BOOST_REQUIRE(srs1.substr(0,4).compare("SRS0") != 0);

    auto old1 = srs.reverse(srs1);
    BOOST_REQUIRE(!old1.empty());
    // its an SRS0 address again
    BOOST_CHECK_EQUAL(old1.substr(0,4).compare("SRS0"), 0);

    auto orig = srs.reverse(old1);

    // got back the original sender
    BOOST_CHECK_EQUAL(orig.compare(source), 0);

    // error detected reversing garbage
    BOOST_CHECK_THROW(srs.reverse("garbage"), std::runtime_error);

    // NOTE: hashless tests skipped because not supported/implemented. hashes are required
}

BOOST_AUTO_TEST_CASE(Usernames) {
    std::vector<std::string> tests = {
        "user@domain-with-dash.com",
        "user-with-dash@domain.com",
        "user+with+plus@domain.com",
        "user%with!everything&everything=@domain.somewhere"
    };

    srs2::guarded srs("foo");

    for (const auto& email : tests) {
        auto srs0addr = srs.forward(email, aliases.front());
        BOOST_TEST_MESSAGE(srs0addr);

        // idempotent on email
        auto oldaddr = srs.reverse(srs0addr);
        BOOST_CHECK_EQUAL(oldaddr.compare(email), 0);

        // idempotent on sr0addr
        auto srs1addr = srs.forward(srs0addr, aliases[1]);
        auto srs0rev = srs.reverse(srs1addr);
        BOOST_CHECK_EQUAL(srs0rev.compare(srs0addr), 0);

        // idempotent from same domain
        auto idemaddr = srs.forward(srs0addr, aliases[0]);
        BOOST_CHECK_EQUAL(srs0addr.compare(idemaddr), 0);

        // NOTE always rewrite skipped because not implemented
    }
}

BOOST_AUTO_TEST_SUITE_END();
