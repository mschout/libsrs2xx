// Copyright (c) 2014, Michael Schout
// Distributed under a 3-clause BSD license. See LICENSE.
#include <boost/algorithm/string.hpp>
#include "srs2/guarded.hpp"
#include "srs2/split.hpp"

namespace srs2 {

using std::string;
using std::vector;

string guarded::compile(const string& sendhost, const string& senduser)
{
    string tag = boost::to_upper_copy(senduser.substr(0,SRS1TAG.length()));

    string user = senduser;

    if (is_srs1(user)) {
        remove_tag(user);

        // we could do a sanity check here.  It might *not* be an SRS address,
        // unlikely though that is.  However, since we do not need to interpret
        // it, we dont' really care if its not an SRS address or not.
        // Malicious users get the garbage back that they sent

        // hash, srshost, srsuer
        vector<string> data = split(user, SRSSEP.front(), 3);

        string hash = hash_create(data.begin() + 1, data.end());

        return (SRS1TAG + m_separator) +
            boost::algorithm::join(vector<string> {
                hash,
                data[1], // srshost
                data[2]  // srsuser
            }, SRSSEP);
    }
    else if (is_srs0(user)) {
        // strip SRS0, but keep the separator
        user = user.substr(SRS0TAG.length());

        string hash = hash_create({ sendhost, user });

        return (SRS1TAG + m_separator) +
            boost::algorithm::join(vector<string> {
                hash,
                sendhost,
                user
            }, SRSSEP);
    }

    return super::compile(sendhost, senduser);
}

address guarded::parse(const string& srsuser)
{
    string tag = boost::to_upper_copy(srsuser.substr(0,SRS1TAG.length()));

    string user = srsuser;

    if (is_srs1(user)) {
        remove_tag(user);

        // hash, host, user, ...
        vector<string> parts = split(user, SRSSEP.front(), 3);

        if (parts.size() != 3)
            throw std::runtime_error("Invalid SRS1 address");

        string hash = parts[0];
        string host = parts[1];
        string user = parts[2];

        if (!verify_hash(hash, parts.begin() + 1, parts.end()))
            throw std::runtime_error("Invalid hash");

        if (host.empty() || parts.empty())
            throw std::runtime_error("Invalid SRS1 address");

        return address(host, SRS0TAG + user);
    }

    return super::parse(srsuser);
}

};
