#pragma once

#include <filesystem>
#include <string_view>
#include <vector>

namespace ftag {
struct AddTagsFileOptions {
  std::filesystem::path db_path;
  bool verbose = false;
};

void addTagsFile(const AddTagsFileOptions& options, std::string_view file,
                 const std::vector<std::string>& tags);

struct RemoveTagsFileOptions {
  std::filesystem::path db_path;
  bool verbose = false;
};

void removeTagsFile(const RemoveTagsFileOptions& options, std::string_view file,
                    const std::vector<std::string>& tags);

}  // namespace ftag
