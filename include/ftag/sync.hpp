#pragma once

#include <filesystem>

namespace ftag {

struct SyncOptions {
  std::filesystem::path db_path;
};

}  // namespace ftag
