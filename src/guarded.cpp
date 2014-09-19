#include <boost/algorithm/string.hpp>
#include "srs2/guarded.hpp"
#include "srs2/split.hpp"

namespace srs2 {

using std::string;
using std::vector;

string guarded::compile(const string& sendhost, const string& senduser)
{
    string tag = boost::to_upper_copy(senduser.substr(0,SRS1TAG.length()));

    // senduser, starting past tag+separator
    string user = senduser.substr(SRS1TAG.length());

    if (tag == SRS1TAG) {
        // we could do a sanity check here.  It might *not* be an SRS address,
        // unlikely though that is.  However, since we do not need to interpret
        // it, we dont' really care if its not an SRS address or not.
        // Malicious users get the garbage back that they sent

        // hash, srshost, srsuer
        vector<string> data = split(user, SRSSEP.front(), 3);

        string hash = hash_create(data.begin() + 1, data.end());

        vector<string> parts {
            (string(SRS1TAG) + string(1, m_separator)),
            hash,
            data[1], // srshost
            data[2]  // srsuser
        };

        return boost::algorithm::join(parts, SRSSEP);
    }
    else if (tag == SRS0TAG) {
        vector<string> data { string(sendhost), user };
        string hash = hash_create(data);

        return SRS1TAG + string(1, m_separator) + hash
            + SRSSEP + string(sendhost)
            + SRSSEP + user;
    }

    return super::compile(sendhost, senduser);
}

address guarded::parse(const string& srsuser)
{
    string tag = boost::to_upper_copy(srsuser.substr(0,SRS1TAG.length()));

    // XXX need to account for valid tag separator. probably need something in
    // base to match tags and remove them

    // srsuser after tag+separator
    string user = srsuser.substr(SRS1TAG.length()+1);

    if (tag == SRS1TAG) {
        // hash, host, user, ...
        vector<string> parts = split(user, SRSSEP.front(), 3);

        if (parts.size() != 3)
            throw std::runtime_error("Invalid SRS1 address");

        string hash = parts[0];
        string host = parts[1];
        string user = parts[2];

        if (!verify_hash(hash, vector<string>(parts.begin() + 1, parts.end())))
            throw std::runtime_error("Invalid hash");

        if (host.empty() || parts.empty())
            throw std::runtime_error("Invalid SRS1 address");

        return address(host, SRS0TAG + user);
    }

    return super::parse(srsuser);
}

};
