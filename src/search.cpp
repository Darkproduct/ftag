#include "ftag/search.hpp"

#include <iostream>
#include <vector>

#include "ftag/database.hpp"
#include "ftag/tag_data.hpp"

namespace ftag {

Search::Search(const ImportOptions& options, const Database& database) 
 : options(options), database(){}

void Search::search(const std::vector<std::string>& input_files) {
  for (std::vector<std::string>::const_iterator it = input_files.begin();
       it != input_files.end(); ++it) {
    std::cout << *it << " ";
  }
}

}  // namespace ftag
