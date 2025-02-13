#include "ftag/search.hpp"

#include <iostream>
#include <vector>

#include "ftag/database_statement.hpp"

namespace ftag {

Search::Search(const Options& options)
    : options(options), database(options.db_path) {}

void Search::search(const std::vector<std::string>& input_files) {
  for (std::vector<std::string>::const_iterator it = input_files.begin();
       it != input_files.end(); ++it) {
    std::cout << *it << " ";
  }
}

void Search::search_tag(const std::vector<std::string>& input_files) {
  std::string searchTagQuery = "SELECT id FROM tags WHERE name = (?)";
  Statement search_Tag(database, searchTagQuery);
  search_Tag.bind(1, input_files[0]);
  search_Tag.executeStep();
}

}  // namespace ftag
