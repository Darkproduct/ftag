#pragma once

#include <filesystem>
#include <string_view>
#include <vector>

namespace ftag {
struct AddTagOptions {
  std::filesystem::path db_path;
  bool verbose = false;
};

void addTag(const AddTagOptions& options, std::string_view name,
            const std::vector<std::string>& aliases);

struct RemoveTagOptions {
  std::filesystem::path db_path;
  bool verbose = false;
};

void removeTag(const RemoveTagOptions& options, std::string_view name);

struct EditTagOptions {
  std::filesystem::path db_path;
  bool verbose = false;
};

void editTag(const EditTagOptions& options, std::string_view name);

struct FindTagOptions {
  std::filesystem::path db_path;
  bool verbose = false;
};

void findTag(const FindTagOptions& options, std::string_view name);

}  // namespace ftag
