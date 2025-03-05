#pragma once

#include <iostream>
#include <source_location>
#include <stacktrace>
#include <string_view>

namespace ftag {

[[noreturn]]
inline void abort(
    const std::string_view message,
    const std::source_location location = std::source_location::current()) {
  std::cerr << std::endl
            << message << std::endl
            << "file: " << location.file_name() << '(' << location.line() << ':'
            << location.column() << ") `" << location.function_name()
            << std::endl
            << std::endl
            << std::stacktrace::current() << std::endl;
  std::abort();
}

}  // namespace ftag
