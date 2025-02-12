#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include "ftag/database.hpp"

namespace ftag {

class TagClass {
public:
  struct Options {
    std::filesystem::path db_path;
    bool verbose = false;
  };

  TagClass(const Options& options);

  void start(const std::vector<std::string>& input_files);
  void addTag(const std::vector<std::string>& input_files);
  void searchTag(const std::vector<std::string>& input_files);

private:
  Options options;
  Database database;
};
}  // namespace ftag
