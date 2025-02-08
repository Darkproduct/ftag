#pragma once

#include <string>
#include <vector>

#include "ftag/database.hpp"
#include "ftag/tag_data.hpp"

namespace ftag {

class Search {
public:
  struct ImportOptions {};
  Search(const ImportOptions& options);

  // TODO: Now just search for a single tagadd search for file names and other
  // stuff in the db
  void search(const std::vector<std::string>& input_files);

private:
  ImportOptions options;
  Database database;
};
}  // namespace ftag
//
