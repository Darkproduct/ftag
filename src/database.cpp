#include "database.hpp"

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <ostream>

ftag::Database::Database() {
  std::string home_dir = std::filesystem::path(std::getenv("HOME"));
  std::filesystem::path data_dir =
      home_dir / std::filesystem::path(".local/state/ftag");

  std::string_view database_filename = "ftag.db"sv;
  std::filesystem::path database_path = data_dir / database_filename;

  if (!std::filesystem::exists(data_dir)) {
    if (std::filesystem::create_directories(data_dir)) {
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
}

ftag::Database::~Database() { sqlite3_close(db); }

void ftag::Database::prepareStatements() {
  /*sqlite3_stmt **ps_create_table;*/
  /*char **pzTail;*/
  /*sqlite3_prepare_v2(db, query_create_files_table,*/
  /*                   query_create_files_table.size(), ps_create_table,*/
  /*                   pzTail);*/
  /**/
  /*sqlite3_step(ps_create_table);*/
}

void ftag::Database::createTables() {
  execute_query(query_create_files_table);
  execute_query(query_create_tags_table);
  execute_query(query_create_tag_map_table);
}

static int callback(void* NotUsed, int argc, char** argv, char** azColName) {
  std::cerr << "Callback of sql statement" << std::endl;
  return 0;
}

void ftag::Database::execute_query(std::string_view query) {
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
    sqlite3_close(db);
    std::abort();
  }
}

void ftag::Database::getFiles() {}
