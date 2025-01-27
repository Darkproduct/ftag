#include <unistd.h>

#include <filesystem>
#include <iostream>

#include "database.hpp"

int main(int argc, char const* argv[]) {
  if (isatty(fileno(stdin))) {
    for (auto const& dir_entry :
         std::filesystem::recursive_directory_iterator(".")) {
      std::cout << dir_entry << std::endl;
    }
  } else {
    for (std::string str{}; std::getline(std::cin, str);) {
      std::cout << str << std::endl;
    }
  }

  ftag::Database db;

  return 0;
}
