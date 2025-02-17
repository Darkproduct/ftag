#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include "ftag/tag_data.hpp"

namespace ftag {
struct ImportOptions {
  std::filesystem::path db_path;
  bool verbose = false;
  bool reimport = false;
  bool autotag = false;

  bool ignore_hidden = true;
  bool respect_gitignore = true;
};

void import(const ImportOptions& options,
            const std::vector<std::string>& paths = {});

std::vector<std::filesystem::path> findFiles(const std::filesystem::path& root,
                                             bool ignore_hidden,
                                             bool respect_gitignore);

void filterFiles(const std::vector<std::filesystem::path>& files);

std::vector<FileInfo> extractTags(
    const std::vector<std::filesystem::path>& files);
void autoTag();

}  // namespace ftag
