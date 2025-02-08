#pragma once

#include <sqlite3.h>
#include <unistd.h>

#include <algorithm>
#include <array>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "ftag/tag_data.hpp"

// https://stackoverflow.com/a/75619411/6411540
template <unsigned... Len>
constexpr auto concat(const char (&... strings)[Len]) {
  constexpr unsigned N = (... + Len) - sizeof...(Len);
  std::array<char, N + 1> result = {};
  result[N] = '\0';

  auto it = result.begin();
  (void)((it = std::copy_n(strings, Len - 1, it), 0), ...);
  return result;
}

namespace ftag {
class Database {
private:
  enum class QueryStatements {
    InsertFiles,
    InsertTags,
  };

  struct SqlStmtDeleter {
    void operator()(sqlite3_stmt* stmt) const {
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
  void createDatabase() const;

  sqlite_stmt_ptr prepareStatement(std::string_view query) const;
  void prepareStatements();

  void bind(QueryStatements stmt, const std::string args, ...);

  // Use only for create tables and so on. Not for possible user input
  void execute_query(std::string_view query) const;

private:
  sqlite3* db;

  constexpr static char query_create_files_table[] =
      "CREATE TABLE files (id INTEGER PRIMARY KEY, path TEXT)";
  constexpr static char query_create_tags_table[] =
      "CREATE TABLE tags (id INTEGER PRIMARY KEY, name TEXT)";
  constexpr static char query_create_tag_map_table[] =
      "CREATE TABLE tagmap (id INTEGER, file_id INTEGER, tag_id INTEGER, "
      "FOREIGN KEY(file_id) REFERENCES files(id), FOREIGN KEY(tag_id) "
      "REFERENCES tags(id))";

  constexpr static char query_insert_files[] =
      "INSERT INTO files (path) VALUES (?)";
  constexpr static char query_insert_tags[] =
      "INSERT INTO tags (name) VALUES (?)";

  std::unordered_map<QueryStatements, sqlite_stmt_ptr> statements;

  std::vector<std::string> file_paths;
};
}  // namespace ftag
