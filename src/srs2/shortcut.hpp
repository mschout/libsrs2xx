// Copyright (c) 2014, Michael Schout
// Distributed under a 3-clause BSD license. See LICENSE.
#ifndef SRS2_SHORTCUT_HPP
#define SRS2_SHORTCUT_HPP

#include "srs2/base.hpp"

namespace srs2 {

class shortcut : public base {
    public:
        using base::base;

    protected:
        virtual std::string compile(const std::string& sendhost, const std::string& senduser) override;
        virtual address parse(const std::string& srsuser) override;
};

};

#endif
