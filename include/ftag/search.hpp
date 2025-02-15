#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include "ftag/database.hpp"

namespace ftag {

class Search {
public:
  struct Options {
    std::filesystem::path db_path;
    bool verbose = false;
  };

  Search(const Options& options);

  // TODO: Now just search for a single tagadd search for file names and other
  // stuff in the db
  void searchTag(const std::vector<std::string>& input_tags);

  void search(const std::vector<std::string>& input_files);

private:
  Options options;
  Database database;
};
}  // namespace ftag
//
