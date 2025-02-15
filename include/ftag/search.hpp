#pragma once

#include <filesystem>

namespace ftag {

struct SearchOptions {
  std::filesystem::path db_path;
  bool verbose = false;
};

void search(const SearchOptions& options, std::string_view pattern);
}  // namespace ftag
