#pragma once

#include <cstdint>
#include <memory>
#include <string_view>
#include <tuple>

struct sqlite3;
struct sqlite3_stmt;

namespace ftag {
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
  void bind(const Args&... args);

  template <typename... Types>
  void bind(const std::tuple<Types...>& tuple);

  template <typename... Types, std::size_t... Indices>
  void bind(const std::tuple<Types...>& tuple, std::index_sequence<Indices...>);

  void executeStep();

private:
  void check(const int ret);

private:
  std::unique_ptr<sqlite3_stmt, Deleter> stmt;
};
}  // namespace ftag
