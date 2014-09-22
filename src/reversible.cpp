// Copyright (c) 2014, Michael Schout
// Distributed under a 3-clause BSD license. See LICENSE.
#include <vector>
#include <boost/algorithm/string.hpp>
#include "srs2/reversible.hpp"

using std::string;

namespace srs2 {

string reversible::compile(const string& sendhost, const string& senduser)
{
    string timestamp = create_timestamp();

    string hash = hash_create({ timestamp, sendhost, senduser });

    return (SRS0TAG + m_separator) +
        boost::algorithm::join(std::vector<string> {
            hash, timestamp, sendhost, senduser
        }, SRSSEP);
}

};
