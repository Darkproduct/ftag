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

void Search::searchTag(const std::vector<std::string>& input_tags) {
  std::string search_tag_query = "SELECT id FROM tags WHERE name = (?)";
  Statement search_tag(database, search_tag_query);
  search_tag.bind(1, input_tags[0]);
  search_tag.executeStep();
}

}  // namespace ftag
