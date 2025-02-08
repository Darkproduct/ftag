#include "ftag/database.hpp"

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <memory>
#include <ostream>
#include <string_view>

namespace ftag {

Database::Database() {
  std::string home_dir = std::filesystem::path(std::getenv("HOME"));
  std::filesystem::path data_dir =
      home_dir / std::filesystem::path(".local/state/ftag");

  std::string_view database_filename = "ftag.db"sv;
  std::filesystem::path database_path = data_dir / database_filename;

  if (!std::filesystem::exists(data_dir)) {
    if (!std::filesystem::create_directories(data_dir)) {
      std::cerr << "Couldn't create dir" << data_dir << std::endl;
      std::abort();
    }
  }

  bool is_new_database = !std::filesystem::exists(database_path);
  if (sqlite3_open_v2(database_path.c_str(), &db,
                      SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr)) {
    std::cerr << "Can't create database: " << sqlite3_errmsg(db) << std::endl;
    sqlite3_close(db);
    std::abort();
  }

  if (is_new_database) {
    createTables();
  }

  // Test
  std::cerr << "start prepare test" << std::endl;
  {
    auto test = prepareStatements(query_add_tag);
    std::cerr << "prepare finished" << std::endl;
  }
  std::cerr << "after prepare test" << std::endl;
}

Database::~Database() { sqlite3_close(db); }

Database::sqlite_stmt_ptr Database::prepareStatements(std::string_view query) {
  sqlite3_stmt* ppStmt;
  if (auto err = sqlite3_prepare_v2(db, query.data(), -1, &ppStmt, nullptr);
      err != SQLITE_OK) {
    std::cerr << "Error preparing sql statement '" << query << "' code: " << err
              << std::endl;
    std::abort();
  }
  return sqlite_stmt_ptr(ppStmt);
}

void Database::addTags(const Tag& tag_data) {
  std::string query =
      "INSERT INTO tags (name) VALUES ('" + tag_data.name + "');";
  execute_query(query);
}

void Database::createTables() {
  execute_query(query_create_files_table);
  execute_query(query_create_tags_table);
  execute_query(query_create_tag_map_table);
}

static int callback(void* NotUsed, int argc, char** argv, char** azColName) {
  std::cerr << "Callback of sql statement" << std::endl;
  return 0;
}

std::vector<FileInfo> Database::search(/* TODO */) { return {}; }

void Database::execute_query(std::string_view query) {
  char** errmsg = nullptr;
  if (auto ret = sqlite3_exec(db, query.data(), callback, nullptr, errmsg);
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
