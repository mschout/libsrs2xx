// Copyright (c) 2014, Michael Schout
// Distributed under a 3-clause BSD license. See LICENSE.
#include <sstream>
#include <boost/algorithm/string.hpp>
#include "srs2/split.hpp"

namespace srs2 {

using std::vector;
using std::string;

vector<string> split(const string& value, const char sep, int limit)
{
    if (limit <= 0) {
        vector<string> results;
        boost::algorithm::split(results, value, boost::is_any_of(std::to_string(sep)));
        return results;
    }

    // pointless to split if asked for only 1 item
    if (limit == 1)
        return vector<string> { value };

    // use stringstream to pull off the requsted number of tokens.
    std::istringstream ss(value);
    vector<string> results;

    while ((results.size() < (limit-1))) {
        string entry;
        if (std::getline(ss, entry, sep))
            results.push_back(entry);
        else
            break;
    }

    // add the rest if necessary
    string entry;
    if (std::getline(ss, entry))
        results.push_back(entry);

    return results;
}

};
