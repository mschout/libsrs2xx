#ifndef SRS2_SPLIT_HPP
#define SRS2_SPLIT_HPP

#include <vector>
#include <string>

namespace srs2 {

std::vector<std::string> split(const std::string& value, const char separator, int limit=0);

};

#endif
