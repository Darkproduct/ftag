#include "ftag/search.hpp"

#include <iostream>
#include <vector>

namespace ftag {

Search::Search(const Options& options)
    : options(options), database(options.db_path) {}

void Search::search(const std::vector<std::string>& input_files) {
  for (std::vector<std::string>::const_iterator it = input_files.begin();
       it != input_files.end(); ++it) {
    std::cout << *it << " ";
  }
}

}  // namespace ftag
