#pragma once

#include <filesystem>

namespace ftag {

struct AddDirOptions {
  std::filesystem::path db_path;
};

void TrackFolder(const AddDirOptions& options);

struct RemoveDirOptions {
  std::filesystem::path db_path;
};

void RemoveFolder(const RemoveDirOptions& options);

}  // namespace ftag
