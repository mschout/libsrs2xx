// Copyright (c) 2014, Michael Schout
// Distributed under a 3-clause BSD license. See LICENSE.
#ifndef SRS2_GUARDED_HPP
#define SRS2_GUARDED_HPP

#include "srs2/shortcut.hpp"

namespace srs2 {

class guarded : public shortcut {
    public:
        using shortcut::shortcut;

    protected:
        typedef shortcut super;

        virtual std::string compile(const std::string& sendhost, const std::string& senduser) override;
        virtual address parse(const std::string& srsuser) override;
};

};
#endif
