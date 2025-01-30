#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace ftag {
class FileImporter {
public:
  struct ImportOptions {
    bool verbose = false;
    bool autotag = false;
  };

public:
  FileImporter(const ImportOptions& options);

  void importFileWalk() const;
  void import(const std::vector<std::string>& files) const;

private:
  void filterFiles(const std::vector<std::filesystem::path>& paths) const;
  void extractTags(const std::vector<std::filesystem::path>& paths) const;
  void autoTag() const;

private:
  const ImportOptions options;
};
}  // namespace ftag
