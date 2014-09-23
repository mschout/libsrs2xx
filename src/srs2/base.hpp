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
        base() { };

        /**
         * Constructor.
         * @param secret the secret for generating and verifying hashes.
         * @param seapartor the SRS separator.
         */
        base(const std::string& secret, char separator='=') : m_separator(separator) {
            if (!(m_separator == '+' || m_separator == '-' || m_separator == '='))
                throw std::runtime_error(std::string("bad separator: ") + m_separator);

            m_secrets.push_back(secret);
        }

        /**
         * Forward an address.  Returns an SRS address.
         */
        std::string forward(const std::string& sender, const std::string& alias);

        /**
         * Reverse an SRS address.
         */
        std::string reverse(const std::string& address);

        /**
         * add a secret to the list of secrets.
         * Future forward() calls will use this secret, and old secrets remain
         * valid.
         */
        void add_secret(const std::string& secret);

        /**
         * check if an address is an SRS address (either SRS0 or SRS1)
         */
        bool is_srs(const std::string& address);

        /**
         * check if an address is an SRS0 address
         */
        bool is_srs0(const std::string& address);

        /**
         * check if an address is an SRS1 address.
         */
        bool is_srs1(const std::string& address);

        /**
         * removes an SRS1 or SRS0 tag plus following separator from the
         * address in-place.
         */
        void remove_tag(std::string& address);

        /**
         * Get the current secret used for generating hashes
         */
        std::string secret() {
            return m_secrets.back();
        }

        /**
         * get the separator charactor.
         */
        char separator() {
            return m_separator;
        }

    protected:
        /**
         * create an SRS address from an email
         */
        virtual std::string compile(const std::string& sendhost, const std::string& senduser) = 0;

        /**
         * parse an SRS address
         */
        virtual address parse(const std::string& address) = 0;

        std::string hash_create(std::vector<std::string>::const_iterator begin,
                                std::vector<std::string>::const_iterator end,
                                const std::string& secret);

        std::string hash_create(std::vector<std::string>::const_iterator begin,
                                std::vector<std::string>::const_iterator end) {
            return hash_create(begin, end, m_secrets.back());
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
