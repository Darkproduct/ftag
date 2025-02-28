#pragma once

#include <filesystem>

namespace ftag {
struct SyncTagOptions {
  std::filesystem::path db_path;
  bool tracked;
};

void syncDatabase(const SyncTagOptions& options);

struct AddDirOptions {
  std::filesystem::path db_path;
};

void TrackFolder(const AddDirOptions& options);

struct RemoveDirOptions {
  std::filesystem::path db_path;
};

void RemoveFolder(const RemoveDirOptions& options);

}  // namespace ftag
