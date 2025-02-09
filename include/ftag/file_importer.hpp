#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include "ftag/database.hpp"

namespace ftag {
class FileImporter {
public:
  struct Options {
    std::filesystem::path db_path;
    bool verbose = false;
    bool reimport = false;
    bool autotag = false;

    bool ignore_hidden = true;
    bool respect_gitignore = true;
  };

public:
  FileImporter(const Options& options);

  void import(const std::vector<std::string>& paths = {}) const;

private:
  std::vector<std::filesystem::path> findFiles(
      const std::filesystem::path& path) const;

private:
  bool isHidden(const std::filesystem::path& path) const;

private:
  void filterFiles(const std::vector<std::filesystem::path>& files) const;

private:
  void extractTags(const std::vector<std::filesystem::path>& files) const;
  void autoTag() const;

private:
  Options options;
  Database database;
};
}  // namespace ftag
