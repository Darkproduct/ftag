#include "ftag/search.hpp"

#include <iostream>
#include <vector>

#include "ftag/tag_data.hpp"

namespace ftag {

Search::Search(const Search::ImportOptions& options) : options(options) {}

void Search::search(const std::vector<std::string>& input_files) {
  for (std::vector<std::string>::const_iterator it = input_files.begin();
       it != input_files.end(); ++it) {
    std::cout << *it << " ";
  }
}

}  // namespace ftag
