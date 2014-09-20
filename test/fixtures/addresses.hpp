// Copyright (c) 2014, Michael Schout
// Distributed under a 3-clause BSD license. See LICENSE.

#ifndef FIXTURES_ADDRESSES_HPP
#define FIXTURES_ADDRESSES_HPP

struct Addresses
{
    std::vector<std::string> aliases;
    std::string source;

    Addresses() : source("user@host.tld")
    {
        for (int i = 0; i <= 5; i++) {
            auto id = std::to_string(i);
            aliases.push_back(std::string("alias") + id + "@host" + id + ".tld" + id);
        }
    }

    ~Addresses() { }
};

#endif
