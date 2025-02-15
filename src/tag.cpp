#include <iostream>

#include "ftag/TagClass.hpp"
#include "ftag/database_statement.hpp"
#include "ftag/tag_data.hpp"

namespace ftag {

TagClass::TagClass(const Options& options)
    : options(options), database(options.db_path) {}

void TagClass::addTag(const std::vector<std::string>& input_files) {
  std::string add_tag_query = "INSERT INTO tags (name) VALUES (?)";
  Statement add_Tag(database, add_tag_query);
  add_Tag.bind(1, input_files[0]);
  add_Tag.executeStep();
}

}  // namespace ftag
