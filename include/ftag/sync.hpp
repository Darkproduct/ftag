#pragma once

#include <filesystem>

namespace ftag {

struct SyncOptions {
  std::filesystem::path db_path;
};

void syncDatabase(const SyncOptions& options);

}  // namespace ftag
