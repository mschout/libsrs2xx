// Copyright (c) 2014, Michael Schout
// Distributed under a 3-clause BSD license. See LICENSE.

#ifndef SRS2_REVERSIBLE_HPP
#define SRS2_REVERSIBLE_HPP

#include <string>
#include "srs2/shortcut.hpp"

namespace srs2 {

class reversible : public shortcut {
    public:
        using shortcut::shortcut;

        virtual std::string compile(const std::string& sendhost,
                                    const std::string& senduser) override;
};

};
#endif
