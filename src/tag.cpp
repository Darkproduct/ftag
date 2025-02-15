#include "ftag/tag.hpp"

#include <iostream>

#include "ftag/database.hpp"
#include "ftag/database_statement.hpp"

namespace ftag {

void addTag(const AddTagOptions& options, std::string_view tag,
            const std::vector<std::string>& aliases) {
  Database db(options.db_path);
  std::string add_tag_query = "INSERT INTO tags (name) VALUES (?)";
  Statement add_tag(db, add_tag_query);

  add_tag.bind(1, tag);
  add_tag.executeStep();
  add_tag.reset();

  // TODO: add all aliases
}

void removeTag(const RemoveTagOptions& options, std::string_view name) {
  std::cerr << "TODO" << std::endl;
  std::abort();
}

void editTag(const EditTagOptions& options, std::string_view name) {
  std::cerr << "TODO" << std::endl;
  std::abort();
}

void findTag(const FindTagOptions& options, std::string_view tag) {
  Database db(options.db_path);
  std::string search_tag_query = "SELECT id FROM tags WHERE name = (?)";
  Statement search_tag(db, search_tag_query);
  search_tag.bind(1, tag);
  search_tag.executeStep();

  // TODO:
  // 1. collect return
  // 2. display tag

  std::cerr << "TODO" << std::endl;
  std::abort();
}

}  // namespace ftag
