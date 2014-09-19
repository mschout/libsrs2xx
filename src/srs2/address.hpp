// Copyright (c) 2014, Michael Schout
// Distributed under a 3-clause BSD license. See LICENSE.
#ifndef SRS2_ADDRESS_HPP
#define SRS2_ADDRESS_HPP

#include <string>

namespace srs2 {

class address {
    public:
        explicit address(const std::string& host, const std::string& user) : m_host(host), m_user(user) { }

        std::string user() { return m_user; }

        std::string host() { return m_host; }

    private:
        std::string m_host;
        std::string m_user;

};

};
#endif
