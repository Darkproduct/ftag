#include <iostream>

#include "ftag/TagClass.hpp"
#include "ftag/database_statement.hpp"
#include "ftag/tag_data.hpp"

namespace ftag {

TagClass::TagClass(const Options& options)
    : options(options), database(options.db_path) {}

void TagClass::addTag(const std::vector<std::string>& input_files) {
  std::string addTagQuery = "INSERT INTO files (path) VALUES (?)";
  Statement add_Tag(database, addTagQuery);
  add_Tag.bind(1, input_files[0]);
  add_Tag.executeStep();

  std::cout << "I GOT HERE: tag " << std::endl;
}

void TagClass::searchTag(const std::vector<std::string>& input_files) {
  std::string searchTagQuery = "SELECT * FROM files";
  Statement search_Tag(database, searchTagQuery);

  std::cout << "I GOT HERE: search 2" << std::endl;

  // search_Tag.bind(1, input_files[0]);

  std::cout << "I GOT HERE: search 1" << std::endl;

  search_Tag.executeStep();

  std::cout << "I GOT HERE: search " << std::endl;
}

}  // namespace ftag
