#pragma once

#include <cstdint>
#include <memory>
#include <string_view>
#include <tuple>
#include <variant>
#include <vector>
struct sqlite3;
struct sqlite3_stmt;

namespace ftag {

using SQLiteValue = std::variant<std::nullptr_t,  // for SQL NULL values
                                 int,             // for INTEGER
                                 double,          // for FLOAT
                                 std::string,     // for TEXT
                                 std::vector<std::uint8_t>  // for BLOB
                                 >;
class Database;

class Statement {
public:
  Statement(const Database& db, const std::string_view query);

  Statement(Statement&) = delete;
  Statement& operator=(const Statement&) = delete;

  Statement(Statement&& db) = default;
  Statement& operator=(Statement&& db) = default;

  ~Statement() = default;

  struct Deleter {
    void operator()(sqlite3_stmt* stmt) const;
  };

public:
  void reset();

  void unbind(const int index);

  void bind(const int index, const int32_t value);
  void bind(const int index, const uint32_t value);
  void bind(const int index, const int64_t value);
  void bind(const int index, const double value);
  void bind(const int index, const std::string_view value);

  template <class... Args>
  void bindMany(const Args&... args);

  template <typename... Types>
  void bindMany(const std::tuple<Types...>& tuple);

  template <typename... Types, std::size_t... Indices>
  void bindMany(const std::tuple<Types...>& tuple,
                std::index_sequence<Indices...>);

  std::vector<SQLiteValue> executeStep();

private:
  void check(const int ret);

private:
  std::unique_ptr<sqlite3_stmt, Deleter> stmt;
};
}  // namespace ftag
