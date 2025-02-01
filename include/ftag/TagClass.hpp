#pragma once

#include <string>
#include <vector>

#include "ftag/database.hpp"
#include "ftag/tag_data.hpp"

namespace ftag {

class TagClass {
public:
  struct ImportOptions {
    bool verbose = false;
    bool addtag = false;
    bool deletetag = false;
    bool tagfiles = false;
    bool deletefiletags = false;
  };
  TagClass(const ImportOptions& options);

  // TODO: Now just for single file and tag
  void addtag(const std::vector<std::string>& input_files);

private:
  ImportOptions options;
  Database database;
};
}  // namespace ftag
