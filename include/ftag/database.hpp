#pragma once

#include <sqlite3.h>
#include <unistd.h>

#include <cassert>
#include <string_view>
#include <vector>

#include "ftag/tag_data.hpp"

namespace ftag {
using namespace std::literals::string_view_literals;

class Database {
public:
  Database();
  ~Database();

  void updateDatabase(const std::vector<FileInfo>& data);
  void updateDatabase(const std::vector<Tag>& tags);
  std::vector<FileInfo> search(/* TODO */);

private:
  void prepareStatements();
  void createTables();

  // Use only for create tables and so on. Not for possible user input
  void execute_query(std::string_view query);

private:
  sqlite3* db;

  constexpr static std::string_view query_create_files_table =
      "CREATE TABLE files (id INTEGER PRIMARY KEY, path TEXT)\0"sv;
  constexpr static std::string_view query_create_tags_table =
      "CREATE TABLE tags (id INTEGER PRIMARY KEY, name TEXT)\0"sv;
  constexpr static std::string_view query_create_tag_map_table =
      "CREATE TABLE tagmap (id INTEGER, file_id INTEGER, tag_id INTEGER, FOREIGN KEY(file_id) REFERENCES files(id), FOREIGN KEY(tag_id) REFERENCES tags(id))\0"sv;

  std::vector<std::string> file_paths;
};
}  // namespace ftag
