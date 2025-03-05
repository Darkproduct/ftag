#pragma once

#include <algorithm>
#include <array>
#include <filesystem>
#include <memory>
#include <vector>

#include "ftag/database_statement.hpp"

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

// Forward declarations to avoid inclusion of <sqlite3.h> in a header
struct sqlite3;
struct sqlite3_context;

namespace ftag {
class Database {
  friend class Statement;

private:
  enum class QueryStatements {
    InsertFiles,
    InsertTags,
  };

public:
  Database(const std::filesystem::path& db_path);

  Database(Database&) = delete;
  Database& operator=(const Database&) = delete;

  Database(Database&& db) = default;
  Database& operator=(Database&& db) = default;

  ~Database() = default;

  struct Deleter {
    void operator()(sqlite3* db) const;
  };

public:
  void exec(std::string_view queries) const;

private:
  void createDatabase() const;

private:
  std::unique_ptr<sqlite3, Deleter> db;

  constexpr static char query_create_files_table[] =
      "CREATE TABLE files ("
      "id INTEGER PRIMARY KEY,"
      "name TEXT,"
      "path TEXT,"
      "size INTEGER,"
      "last_modified TEXT)";
  constexpr static char query_create_tags_table[] =
      "CREATE TABLE tags ("
      "id INTEGER PRIMARY KEY,"
      "name TEXT)";
  constexpr static char query_create_tag_map_table[] =
      "CREATE TABLE tagmap ("
      "id INTEGER PRIMARY KEY,"
      "file_id INTEGER,"
      "tag_id INTEGER,"
      "FOREIGN KEY(file_id) REFERENCES files(id),"
      "FOREIGN KEY(tag_id) REFERENCES tags(id))";

  constexpr static char query_create_directories[] =
      "CREATE TABLE directories ("
      "id INTEGER PRIMARY KEY,"
      "path TEXT)";
  constexpr static char query_create_dir_map_table[] =
      "CREATE TABLE filemap ("
      "id INTEGER PRIMARY KEY,"
      "file_id INTEGER UNIQUE,"
      "dir_id INTEGER,"
      "FOREIGN KEY(file_id) REFERENCES files(id),"
      "FOREIGN KEY(dir_id) REFERENCES directories(id))";

  std::vector<std::string> file_paths;
};
}  // namespace ftag
