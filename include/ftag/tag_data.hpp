#pragma once

#include <filesystem>
#include <string>
#include <vector>

enum class FileState {
  INSERT,
  DELETE,
  UPDATE,
};

struct Tag {
  std::string name = ""; // Default to empty string
  std::vector<std::string> aliases; // Automatically empty
  std::vector<Tag> tags; // Automatically empty
};

struct FileInfo {
  std::filesystem::path path = {}; // Default to empty path
  std::vector<Tag> tags; // Automatically empty
  FileState state = FileState::INSERT; // Default to INSERT
};
