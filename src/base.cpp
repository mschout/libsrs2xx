#include <sstream>
#include <boost/algorithm/string.hpp>
#include <openssl/hmac.h>

#include "base64.hpp"
#include "srs2/base.hpp"
#include "srs2/address.hpp"

namespace srs2 {

/* Don't mess with these unless you know what you're doing well
 * enough to rewrite the timestamp functions. These are based on
 * a 2 character timestamp. Changing these in the wild is probably
 * a bad idea. */
const int timestamp_precision = (60 * 60 * 24);  // one day
const int timestamp_basebits  = 5;               // 2^5 = 32 = strlen(CHARS)

const char *timestamp_basechars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";

const int timestamp_size = 2;
const int timestamp_slots = (1 << (timestamp_basebits << (timestamp_size-1)));

using std::string;
using std::vector;

void base::add_secret(const string& secret)
{
    if (!secret.empty())
        m_secrets.push_back(secret);
}

string base::forward(const string& sender, const string& alias)
{
    auto atpos = sender.find_first_of("@");

    if (atpos == string::npos)
        throw std::runtime_error(string("Sender ") + sender + " contains no @");

    string senduser = sender.substr(0,atpos);
    string sendhost = sender.substr(atpos+1);

    if (senduser.find_first_of("@") != string::npos)
        throw std::runtime_error("Sender username may not contain an @");

    string aliashost = alias;

    atpos = alias.find_last_of("@");
    if (atpos != string::npos)
        aliashost = alias.substr(atpos+1);

    if (boost::algorithm::equals(aliashost, sendhost, boost::algorithm::is_iequal()))
        // XXX ref impl has AlwaysRewrite option that determins if we do this or not
        return senduser + "@" + sendhost;

    return compile(sendhost, senduser) + "@" + aliashost;
}

string base::reverse(const string& address)
{
    auto atpos = address.find_last_of("@");

    if (atpos == string::npos)
        throw std::runtime_error("Address contains no '@'");

    string user = address.substr(0,atpos);
    string host = address.substr(atpos+1);

    auto addr = parse(user);

    return addr.user() + "@" + addr.host();
}

string base::create_timestamp(time_t now)
{
    now = now / timestamp_precision;
    std::vector<char> buf(2);

    buf[1] = timestamp_basechars[now & ((1 << timestamp_basebits) - 1)];
    now = now >> timestamp_basebits;
    buf[0] = timestamp_basechars[now & ((1 << timestamp_basebits) - 1)];

    return buf.data();
}

bool base::check_timestamp(const string& stamp)
{
    time_t now;
    time_t then = 0;

    for (const auto& i : stamp) {
        auto bp = strchr(timestamp_basechars, std::toupper(i));
        if (!bp)
            throw std::runtime_error("bad timestamp character");

        auto off = bp - timestamp_basechars;
        then = (then << timestamp_basebits) | off;
    }

    time(&now);
    now = (now / timestamp_precision) % timestamp_slots;

    while (now < then)
        now = now + timestamp_slots;

    if (now <= then + m_maxage)
        return true;
    else
        return false;
}

string base::hash_create(vector<string>::const_iterator begin,
                         vector<string>::const_iterator end,
                         const string& secret)
{
    std::ostringstream buf;
    std::vector<unsigned char> digest(160);

    for (auto& item = begin; item != end; item++)
        buf << *item;

    string data = buf.str();
    boost::algorithm::to_lower(data);

    string key = m_secrets.front();

    HMAC(EVP_sha1(), key.c_str(), key.length(),
        (unsigned char *)data.c_str(), data.length(), digest.data(), NULL);

    return base64_encode(digest.data(), 6).substr(0,4);
}


bool base::verify_hash(const std::string& hash,
                       std::vector<string>::const_iterator begin,
                       std::vector<string>::const_iterator end)
{
    vector<string> valid;

    for (const auto& secret : m_secrets) {
        auto check = hash_create(begin, end, secret);

        if (check == hash)
            return true;

        valid.push_back(check);
    }

    // if no match was found, try case-insensitve search
    for (const auto& check : valid)
        if (boost::algorithm::equals(check, hash, boost::algorithm::is_iequal()))
            return true;

    return false;
}

}; // namespace srs2
