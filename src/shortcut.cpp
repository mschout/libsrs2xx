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

    // tags are all the same length, so just use SRS0TAG.length()
    string tag = boost::to_upper_copy(senduser.substr(0, SRS0TAG.length()));

    if (tag == SRS0TAG) {
        // Duplicates effort of guarded class, but makes this calss work on its own.
        vector<string> parts = split(senduser, SRSSEP.front(), 5);

        host = parts[3];
        user = parts[4];
    }
    else if (tag == SRS1TAG) {
        // This should never be hit in practice.  It would be bad.
        // Introduce compatibility with the guarded format?
        // tag, SRSHOST, hash, timestamp, host, user
        vector<string> parts = split(senduser, SRSSEP.front(), 7);

        host = parts[4];
        user = parts[5];
    }

    hashdata.push_back(host);
    hashdata.push_back(user);

    string hash = hash_create(hashdata);

    vector<string> address_parts = { SRS0TAG, hash, timestamp, host, user };

    string address = boost::algorithm::join(address_parts, string(1, m_separator));

    return address;
}

address shortcut::parse(const string& srsuser)
{
    string tag = boost::to_upper_copy(srsuser.substr(0, SRS0TAG.length()));

    if (!(tag == SRS0TAG))
        throw std::runtime_error(string("Reverse address does not match ") + SRS0TAG);

    // tag, hash, timestamp, sendhost, senduser
    // if there are more separators, they belong in the senduser, hence split limited to 5
    vector<string> data = split(srsuser, SRSSEP.front(), 5);

    if (!verify_hash(data[1], data.begin() + 2, data.end()))
        throw std::runtime_error("Invalid hash x");

    if (!check_timestamp(data[2]))
        throw std::runtime_error("Invalid timestamp");

    return address(data[3],data[4]);
}

};

