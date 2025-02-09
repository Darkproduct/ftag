#include "ftag/database.hpp"

#include <sqlite3.h>

#include <iostream>
#include <ostream>
#include <string_view>

namespace ftag {

Database::Database(const std::filesystem::path& db_path) {
  bool is_new_database = !std::filesystem::exists(db_path);
  sqlite3* db_ptr;
  if (sqlite3_open_v2(reinterpret_cast<const char*>(db_path.u8string().c_str()),
                      &db_ptr, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
                      nullptr)) {
    std::cerr << "Can't create database: " << sqlite3_errmsg(db_ptr)
              << std::endl;
    sqlite3_close(db_ptr);
    std::abort();
  } else {
    db.reset(db_ptr);
  }

  if (is_new_database) {
    createDatabase();
  }
}

void Database::Deleter::operator()(sqlite3* db) const {
  if (auto err = sqlite3_close_v2(db); err != SQLITE_OK) {
    std::cerr << "error: sqlite db couldn't be closed with error " << err
              << std::endl;
    std::abort();
  }
}

void Database::exec(std::string_view query) const {
  char** errmsg = nullptr;
  if (auto ret = sqlite3_exec(db.get(), query.data(), nullptr, nullptr, errmsg);
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

void Database::createDatabase() const {
  exec(query_create_files_table);
  exec(query_create_tags_table);
  exec(query_create_tag_map_table);
}

}  // namespace ftag
