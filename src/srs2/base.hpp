// Copyright (c) 2014, Michael Schout
// Distributed under a 3-clause BSD license. See LICENSE.
#ifndef SRS2_HPP
#define SRS2_HPP

#include <time.h>
#include <vector>
#include <string>
#include "srs2/address.hpp"

namespace srs2 {

const std::string SRS0TAG = "SRS0";
const std::string SRS1TAG = "SRS1";
const std::string SRSSEP  = "=";

class base {
    public:
        base() {};
        base(const std::string& secret) {
            m_secrets.push_back(secret);
        }

        std::string forward(const std::string& sender, const std::string& alias);
        std::string reverse(const std::string& address);

        void add_secret(const std::string& secret);

        std::string secret() {
            return m_secrets.back();
        }

    protected:
        // creste SRS address from an email
        virtual std::string compile(const std::string& sendhost, const std::string& senduser) = 0;

        virtual address parse(const std::string& address) = 0;

        std::string hash_create(std::vector<std::string>::const_iterator begin,
                                std::vector<std::string>::const_iterator end,
                                const std::string& secret);

        std::string hash_create(std::vector<std::string>::const_iterator begin,
                                std::vector<std::string>::const_iterator end) {
            return hash_create(begin, end, m_secrets.front());
        }

        std::string hash_create(const std::vector<std::string>& data) {
            return hash_create(data.begin(),  data.end());
        }

        bool verify_hash(const std::string& hash,
                         std::vector<std::string>::const_iterator begin,
                         std::vector<std::string>::const_iterator end);

        bool verify_hash(const std::string& hash, std::vector<std::string> parts) {
            return verify_hash(hash, parts.begin(), parts.end());
        }

        bool check_timestamp(const std::string& stamp);

        std::string create_timestamp(time_t now);
        std::string create_timestamp() {
            return create_timestamp(time(nullptr));
        }


        std::vector<std::string> m_secrets;
        char m_separator = '=';
        int m_maxage = 21;
        int m_hashlength = 4;
        int m_hashmin = 4;
        bool m_alwaysrewrite = false;

};

}; // namespace srs2

#endif
