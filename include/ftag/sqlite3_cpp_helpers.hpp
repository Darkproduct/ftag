#pragma once

#include <sqlite3.h>
#include <unistd.h>

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <memory>

namespace ftag {

struct SqlDbDeleter {
  void operator()(sqlite3* db) const {
    if (auto err = sqlite3_close_v2(db); err != SQLITE_OK) {
      std::cerr << "error: sqlite db couldn't be closed with error " << err
                << std::endl;
      std::abort();
    }
  }
};

typedef std::unique_ptr<sqlite3, SqlDbDeleter> sqlite_uptr;

struct SqlStmtDeleter {
  void operator()(sqlite3_stmt* stmt) const {
    if (auto err = sqlite3_finalize(stmt); err != SQLITE_OK) {
      std::cerr << "error: sqlite finalize finished with error " << err
                << std::endl;
      std::abort();
    }
  }
};

typedef std::unique_ptr<sqlite3_stmt, SqlStmtDeleter> sqlite_stmt_ptr;
}  // namespace ftag
