#pragma once

#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

enum class FileState {
  INSERT,
  DELETE,
  UPDATE,
};

struct Tag {
  std::string name;
  std::vector<std::string> aliases;
  std::vector<Tag> tags;
};

struct FileInfo {
  std::filesystem::path path;
  std::string file_name;
  std::uintmax_t file_size;
  std::string last_modified;
  std::vector<Tag> tags;
};
