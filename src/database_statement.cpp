#include "ftag/database_statement.hpp"

#include <sqlite3.h>

#include <initializer_list>
#include <iostream>

#include "ftag/database.hpp"

namespace ftag {
Statement::Statement(const Database& db, const std::string_view query) {
  sqlite3_stmt* ppStmt;
  const int ret = sqlite3_prepare_v2(db.db.get(), query.data(), query.size(),
                                     &ppStmt, nullptr);
  check(ret);

  stmt.reset(ppStmt);
}

void Statement::Deleter::operator()(sqlite3_stmt* stmt) const {
  if (auto err = sqlite3_finalize(stmt); err != SQLITE_OK) {
    std::cerr << "error: sqlite finalize finished with error " << err
              << std::endl;
    std::abort();
  }
}

void Statement::reset() {
  const int ret_clear = sqlite3_clear_bindings(stmt.get());
  check(ret_clear);

  const int ret_reset = sqlite3_reset(stmt.get());
  check(ret_reset);
}

void Statement::unbind(const int index) {
  const int ret = sqlite3_bind_null(stmt.get(), index);
  check(ret);
}

void Statement::bind(const int index, const int32_t value) {
  const int ret = sqlite3_bind_int(stmt.get(), index, value);
  check(ret);
}

void Statement::bind(const int index, const uint32_t value) {
  const int ret = sqlite3_bind_int64(stmt.get(), index, value);
  check(ret);
}

void Statement::bind(const int index, const int64_t value) {
  const int ret = sqlite3_bind_int64(stmt.get(), index, value);
  check(ret);
}

void Statement::bind(const int index, const double value) {
  const int ret = sqlite3_bind_double(stmt.get(), index, value);
  check(ret);
}

void Statement::bind(const int index, const std::string_view value) {
  const int ret =
      sqlite3_bind_text(stmt.get(), index, value.data(),
                        static_cast<int>(value.size()), SQLITE_TRANSIENT);
  check(ret);
}

template <class... Args>
void Statement::bind(const Args&... args) {
  int pos = 0;
  (void)std::initializer_list<int>{
      ((void)bind(++pos, std::forward<decltype(args)>(args)), 0)...};
}

template <typename... Types>
void Statement::bind(const std::tuple<Types...>& tuple) {
  bind(tuple, std::index_sequence_for<Types...>());
}

template <typename... Types, std::size_t... Indices>
void Statement::bind(const std::tuple<Types...>& tuple,
                     std::index_sequence<Indices...>) {
  bind(std::get<Indices>(tuple)...);
}

void Statement::executeStep() {}

void Statement::check(const int ret) {
  if (ret != SQLITE_OK) {
    std::cerr << "Error in sqlite statement with error code: " << ret
              << std::endl;
    std::abort();
  }
}

}  // namespace ftag
