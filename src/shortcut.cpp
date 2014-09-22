// Copyright (c) 2014, Michael Schout
// Distributed under a 3-clause BSD license. See LICENSE.
#include <vector>
#include <boost/algorithm/string.hpp>
#include "srs2/split.hpp"
#include "srs2/shortcut.hpp"

namespace srs2 {

using std::string;
using std::vector;

string shortcut::compile(const string& sendhost, const string& senduser)
{
    string host = sendhost;
    string user = senduser;

    string timestamp = create_timestamp();

    vector<string> hashdata;
    hashdata.push_back(timestamp);

    if (is_srs0(user)) {
        remove_tag(user);

        // Duplicates effort of guarded class, but makes this calss work on its own.
        vector<string> parts = split(user, SRSSEP.front(), 4);

        host = parts[2];
        user = parts[3];
    }
    else if (is_srs1(user)) {
        remove_tag(user);
        // This should never be hit in practice.  It would be bad.
        // Introduce compatibility with the guarded format?
        // tag, SRSHOST, hash, timestamp, host, user
        vector<string> parts = split(user, SRSSEP.front(), 6);

        host = parts[3];
        user = parts[4];
    }

    hashdata.push_back(host);
    hashdata.push_back(user);

    string hash = hash_create(hashdata);

    return (SRS0TAG + m_separator) +
        boost::algorithm::join(vector<string> {
            hash,
            timestamp,
            host,
            user
        }, SRSSEP);
}

address shortcut::parse(const string& srsuser)
{
    if (!is_srs0(srsuser))
        throw std::runtime_error(string("Reverse address does not match ") + SRS0TAG);

    string user = srsuser;
    remove_tag(user);

    // hash, timestamp, sendhost, senduser
    // The 4 here matches the number of fields we encoded above. If there are
    // more separators, then they belong in senduser anyway.
    vector<string> data = split(user, SRSSEP.front(), 4);

    if (!verify_hash(data[0], data.begin() + 1, data.end()))
        throw std::runtime_error("Invalid hash x");

    if (!check_timestamp(data[1]))
        throw std::runtime_error("Invalid timestamp");

    return address(data[2],data[3]);
}

};

