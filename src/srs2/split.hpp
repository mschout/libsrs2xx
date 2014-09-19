// Copyright (c) 2014, Michael Schout
// Distributed under a 3-clause BSD license. See LICENSE.
#ifndef SRS2_SPLIT_HPP
#define SRS2_SPLIT_HPP

#include <vector>
#include <string>

namespace srs2 {

std::vector<std::string> split(const std::string& value, const char separator, int limit=0);

};

#endif
