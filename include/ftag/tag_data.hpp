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
  std::string name = "";
  std::vector<std::string> aliases;
  std::vector<Tag> tags;
};

struct FileInfo {
  std::filesystem::path path = {};
  std::vector<Tag> tags;
  FileState state = FileState::INSERT;
};
