#include <boost/test/unit_test.hpp>

// expose protected/private members for testing purposes
#define protected public
#define private   public

#include "srs2/guarded.hpp"

using std::string;
using std::vector;

BOOST_AUTO_TEST_SUITE(Basic)

BOOST_AUTO_TEST_CASE(hashing) {
    srs2::guarded srs("foo");

    BOOST_CHECK_EQUAL(srs.secret().compare("foo"), 0);

    srs.add_secret("bar");
    BOOST_CHECK_EQUAL(srs.secret().compare("bar"), 0);

    BOOST_CHECK_EQUAL(srs.m_secrets[0].compare("foo"), 0);

    auto h = srs.hash_create({ "foo" });
    BOOST_REQUIRE(!h.empty());                // hashing works
    BOOST_REQUIRE(srs.verify_hash(h, { "foo" })); // hash verify
    BOOST_REQUIRE(!srs.verify_hash("random", { "foo" })); // bad hashes fail
    BOOST_REQUIRE(!srs.verify_hash(h, { "bar" }));        // wrong data fails
}

BOOST_AUTO_TEST_CASE(timestamp) {
    srs2::guarded srs("foo");

    auto t = srs.create_timestamp();
    BOOST_REQUIRE(!t.empty());           // created a timestamp
    BOOST_CHECK_EQUAL(t.length(), 2);    // timestamp len = 2
    BOOST_REQUIRE(srs.check_timestamp(t)); // timestamp verifies

    time_t notlong = 60 * 60 * 24 * 3;
    time_t ages = 60 * 60 * 24 * 50;
    time_t now;
    time(&now);

    BOOST_REQUIRE(srs.check_timestamp(srs.create_timestamp(now - notlong)));   // past timestamp ok
    BOOST_REQUIRE(!srs.check_timestamp(srs.create_timestamp(now - ages)));     // antique timestamp fails
    BOOST_REQUIRE(!srs.check_timestamp(srs.create_timestamp(now + notlong)));  // future timestamp fails
    BOOST_REQUIRE(!srs.check_timestamp(srs.create_timestamp(now + ages)));     // future timestamp fails
}

// NOTE: reference implementation of  "Ignore timestamp" not implemented, so not tested.

BOOST_AUTO_TEST_SUITE_END();
