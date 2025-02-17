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
  std::filesystem::path path = {};
  std::uintmax_t file_size;
  // TODO: Fucking SQL Time stuff
  std::vector<Tag> tags;
};
