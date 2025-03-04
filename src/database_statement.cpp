#include "ftag/database_statement.hpp"

#include <sqlite3.h>

#include <cstdint>
#include <cstdlib>
#include <format>
#include <iostream>
#include <utility>

#include "ftag/abort.hpp"
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
    abort(std::format("error: sqlite finalize finished with error {}", err));
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

void Statement::bind(const int index, const uint64_t value) {
  const int ret =
      sqlite3_bind_int64(stmt.get(), index, static_cast<int64_t>(value));
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

template <>
int32_t Statement::getColumn<int32_t>(int index) {
  if (const auto t = sqlite3_column_type(stmt.get(), index);
      t != SQLITE_INTEGER) {
    abort(std::format(
        "Reading unexpected type from sqlite row. Expected integer got {}", t));
  }

  return sqlite3_column_int(stmt.get(), index);
}

template <>
uint32_t Statement::getColumn<uint32_t>(int index) {
  if (const auto t = sqlite3_column_type(stmt.get(), index);
      t != SQLITE_INTEGER) {
    abort(std::format(
        "Reading unexpected type from sqlite row. Expected integer got {}", t));
  }

  return sqlite3_column_int64(stmt.get(), index);
}

template <>
int64_t Statement::getColumn<int64_t>(int index) {
  if (const auto t = sqlite3_column_type(stmt.get(), index);
      t != SQLITE_INTEGER) {
    abort(std::format(
        "Reading unexpected type from sqlite row. Expected integer got {}", t));
  }

  return sqlite3_column_int64(stmt.get(), index);
}

template <>
uint64_t Statement::getColumn<uint64_t>(int index) {
  if (const auto t = sqlite3_column_type(stmt.get(), index);
      t != SQLITE_INTEGER) {
    abort(std::format(
        "Reading unexpected type from sqlite row. Expected integer got {}", t));
  }

  return static_cast<uint64_t>(sqlite3_column_int64(stmt.get(), index));
}

template <>
double Statement::getColumn<double>(int index) {
  if (const auto t = sqlite3_column_type(stmt.get(), index);
      t != SQLITE_FLOAT) {
    abort(std::format(
        "Reading unexpected type from sqlite row. Expected float got {}", t));
  }

  return sqlite3_column_double(stmt.get(), index);
}

template <>
std::string Statement::getColumn<std::string>(int index) {
  if (const auto t = sqlite3_column_type(stmt.get(), index); t != SQLITE_TEXT) {
    abort(std::format(
        "Reading unexpected type from sqlite row. Expected text got {}", t));
  }

  return std::string(
      reinterpret_cast<const char*>(sqlite3_column_text(stmt.get(), index)));
}

template <>
std::vector<std::byte> Statement::getColumn<std::vector<std::byte>>(int index) {
  const std::byte* blobData =
      static_cast<const std::byte*>(sqlite3_column_blob(stmt.get(), index));
  int blobSize = sqlite3_column_bytes(stmt.get(), index);
  return std::vector<std::byte>(blobData, blobData + blobSize);
}

template <typename... Types, std::size_t... Indices>
std::tuple<Types...> Statement::getRow(std::index_sequence<Indices...>) {
  return {getColumn<Types>(Indices)...};
}

template <typename... Types>
std::optional<std::tuple<Types...>> Statement::executeStep() {
  int return_code = sqlite3_step(stmt.get());
  if (return_code == SQLITE_DONE) {
    std::cout << "query finished" << std::endl;
    return std::nullopt;
  } else if (return_code == SQLITE_ROW) {
    constexpr std::size_t count = sizeof...(Types);
    int number_of_columns = sqlite3_column_count(stmt.get());
    if (number_of_columns != count) {
      abort(std::format("Reading unexpected column length. Expected {} got {}",
                        count, number_of_columns));
    }

    return getRow<Types...>(std::index_sequence_for<Types...>());
  } else {
    check(return_code);
    abort(std::format("wtf is this? {}", return_code));
  }
}

template <typename... Types>
std::vector<std::tuple<Types...>> Statement::execute() {
  std::vector<std::tuple<Types...>> result{};

  auto row = executeStep<Types...>();
  while (row) {
    result.push_back(*row);
    row = executeStep<Types...>();
  }

  return result;
}

template std::vector<std::tuple<int64_t, std::string>>
Statement::execute<int64_t, std::string>();

std::vector<ftag::SQLiteValue> Statement::executeStepVariant() {
  int return_code = sqlite3_step(stmt.get());
  if (return_code == SQLITE_DONE) {
    std::cout << "query finished" << std::endl;
    return {};
  } else if (return_code == SQLITE_ROW) {
    int number_of_columns = sqlite3_column_count(stmt.get());
    std::vector<SQLiteValue> row;
    row.reserve(number_of_columns);

    for (int i = 0; i < number_of_columns; i++) {
      int type = sqlite3_column_type(stmt.get(), i);
      switch (type) {
        case SQLITE_INTEGER:
          row.emplace_back(sqlite3_column_int(stmt.get(), i));
          break;
        case SQLITE_FLOAT:
          row.emplace_back(sqlite3_column_double(stmt.get(), i));
          break;
        case SQLITE_TEXT:
          row.emplace_back(std::string(reinterpret_cast<const char*>(
              sqlite3_column_text(stmt.get(), i))));
          break;
        case SQLITE_BLOB: {
          const void* blob = sqlite3_column_blob(stmt.get(), i);
          int blobSize = sqlite3_column_bytes(stmt.get(), i);
          const std::uint8_t* blobData = static_cast<const std::uint8_t*>(blob);
          row.emplace_back(
              std::vector<std::uint8_t>(blobData, blobData + blobSize));
          break;
        }
        case SQLITE_NULL:
        default:
          row.push_back(nullptr);
          break;
      }
    }
    // int id = sqlite3_column_int(stmt.get(), 0);
    // const char* tag =
    //    reinterpret_cast<const char*>(sqlite3_column_text(stmt.get(), 1));

    // std::cout << "Output is: " << i
  } else {
    check(return_code);
    executeStep();
  }
  return {};
}

void Statement::check(const int ret) {
  if (ret != SQLITE_OK) {
    abort(std::format("Error in sqlite statement with error code: {}", ret));
  }
}

}  // namespace ftag
