#pragma once

#include <string>

#include "ftag/tag_data.hpp"

namespace ftag {

class Search {
public:
  Search(/* pass parser options */);

  FileInfo search(const std::string& search) { return {}; }
};
}  // namespace ftag
