#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string_view>
#include <tuple>
#include <variant>
#include <vector>

struct sqlite3;
struct sqlite3_stmt;

namespace ftag {

using SQLiteValue = std::variant<std::nullptr_t, int, double, std::string,
                                 std::vector<std::uint8_t>>;
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
  void bind(const int index, const uint64_t value);
  void bind(const int index, const double value);
  void bind(const int index, const std::string_view value);

  template <class... Args>
  void bindMany(const Args&... args) {
    int pos = 0;
    (void)std::initializer_list<int>{
        ((void)bind(++pos, std::forward<decltype(args)>(args)), 0)...};
  }

  template <typename... Types>
  void bindMany(const std::tuple<Types...>& tuple) {
    bind(tuple, std::index_sequence_for<Types...>());
  }

  template <typename... Types, std::size_t... Indices>
  void bindMany(const std::tuple<Types...>& tuple,
                std::index_sequence<Indices...>) {
    bind(std::get<Indices>(tuple)...);
  }

private:
  template <typename T>
  T getColumn(int index);

  template <typename... Types, std::size_t... Indices>
  std::tuple<Types...> getRow(std::index_sequence<Indices...>);

public:
  template <typename... Types>
  std::optional<std::tuple<Types...>> executeStep();

  template <typename... Types>
  std::vector<std::tuple<Types...>> execute();

  std::vector<SQLiteValue> executeStepVariant();

private:
  void check(const int ret);

private:
  std::unique_ptr<sqlite3_stmt, Deleter> stmt;
};
}  // namespace ftag
