#include "ftag/database.hpp"

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <ostream>
#include <string_view>

namespace ftag {

Database::Database() {
  std::string home_dir = std::filesystem::path(std::getenv("HOME"));
  std::filesystem::path data_dir =
      home_dir / std::filesystem::path(".local/state/ftag");

  constexpr char database_filename[] = "ftag.db";
  std::filesystem::path database_path = data_dir / database_filename;

  if (!std::filesystem::exists(data_dir)) {
    if (!std::filesystem::create_directories(data_dir)) {
      std::cerr << "Couldn't create dir" << data_dir << std::endl;
      std::abort();
    }
  }

  bool is_new_database = !std::filesystem::exists(database_path);
  sqlite3* db_ptr;
  if (sqlite3_open_v2(database_path.c_str(), &db_ptr,
                      SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr)) {
    std::cerr << "Can't create database: " << sqlite3_errmsg(db_ptr)
              << std::endl;
    sqlite3_close(db_ptr);
    std::abort();
  } else {
    db = sqlite_uptr(db_ptr);
  }

  if (is_new_database) {
    createDatabase();
  }

  prepareStatements();
}

std::vector<FileInfo> Database::search(/* TODO */) { return {}; }

void Database::addTags(const Tag& tag_data) {
  std::string query =
      "INSERT INTO tags (name) VALUES ('" + tag_data.name + "');";
  execute_query(query);
}

sqlite_stmt_ptr Database::prepareStatement(std::string_view query) const {
  sqlite3_stmt* ppStmt;
  if (auto err = sqlite3_prepare_v2(db.get(), query.data(), query.size(),
                                    &ppStmt, nullptr);
      err != SQLITE_OK) {
    std::cerr << "Error preparing sql statement '" << query << "' code: " << err
              << std::endl;
    std::abort();
  }
  return sqlite_stmt_ptr(ppStmt);
}

void Database::prepareStatements() {
  statements[QueryStatements::InsertFiles] =
      prepareStatement(query_insert_files);
  statements[QueryStatements::InsertTags] = prepareStatement(query_insert_tags);
}

void Database::bind(QueryStatements stmt, const std::string& args, ...) {
  // TODO:
}

void Database::createDatabase() const {
  execute_query(query_create_files_table);
  execute_query(query_create_tags_table);
  execute_query(query_create_tag_map_table);
}

static int callback(void* NotUsed, int argc, char** argv, char** azColName) {
  std::cerr << "Callback of sql statement" << std::endl;
  return 0;
}

void Database::execute_query(std::string_view query) const {
  char** errmsg = nullptr;
  if (auto ret =
          sqlite3_exec(db.get(), query.data(), callback, nullptr, errmsg);
      ret) {
    if (errmsg != nullptr) {
      std::cerr << "sqlite3_exec error with message: " << *errmsg << std::endl;
      sqlite3_free(errmsg);
    } else {
      std::cerr << "sqlite3_exec error without error message. Return num "
                << ret << std::endl;
    }
    std::abort();
  }
}
}  // namespace ftag
