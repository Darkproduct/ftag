#pragma once

#include <filesystem>

namespace ftag {

namespace ftag {
struct SyncTagOptions {
  std::filesystem::path db_path;
  bool tracked;
};

void syncDatabase(const SyncTagOptions& options);

}  // namespace ftag
}  // namespace ftag
