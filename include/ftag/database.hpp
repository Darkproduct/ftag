#pragma once

#include <sqlite3.h>
#include <unistd.h>

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string_view>
#include <vector>

#include "ftag/tag_data.hpp"

namespace ftag {
using namespace std::literals::string_view_literals;

class Database {
public:
  struct SqlStmtDeleter {
    void operator()(sqlite3_stmt* stmt) {
      std::cerr << "Call deleter of stmt" << std::endl;
      if (auto err = sqlite3_finalize(stmt); err != SQLITE_OK) {
        std::cerr << "error: sqlite finalize finished with error " << err
                  << std::endl;
        std::abort();
      }
    }
  };

  using sqlite_stmt_ptr = std::unique_ptr<sqlite3_stmt, SqlStmtDeleter>;

public:
  Database();
  ~Database();

  std::vector<FileInfo> search(/* TODO */);
  void addTags(const Tag& tag_data);

private:
  sqlite_stmt_ptr prepareStatements(std::string_view query);
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

  constexpr static std::string_view query_add_tag =
      "INSERT INTO tags (name) VALUES (?)\0"sv;

  std::vector<std::string> file_paths;
};
}  // namespace ftag
