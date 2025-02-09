#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include "ftag/database.hpp"

namespace ftag {

class Search {
public:
  struct ImportOptions {
    std::filesystem::path db_path;
    bool verbose = false;
  };

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
