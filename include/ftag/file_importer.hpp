#pragma once

#include <unistd.h>

#include <filesystem>
#include <string>
#include <vector>

namespace ftag {
class FileImporter {
public:
  FileImporter(bool verbose, bool autotag);

  int import() const;
  int import(const std::vector<std::string>& files) const;

private:
  int filterFiles(const std::vector<std::filesystem::path>& paths) const;
};
}  // namespace ftag
