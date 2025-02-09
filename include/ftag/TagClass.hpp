#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include "ftag/database.hpp"

namespace ftag {

class TagClass {
public:
  struct ImportOptions {
    std::filesystem::path db_path;
    bool verbose = false;
    bool addtag = false;
    bool deletetag = false;
    bool tagfiles = false;
    bool deletefiletags = false;
  };

  TagClass(const ImportOptions& options);

  void start(const std::vector<std::string>& input_files);
  void addTag(const std::vector<std::string>& input_files);

private:
  ImportOptions options;
  Database database;
};
}  // namespace ftag
