#include <unistd.h>

#include <iostream>

#include "ftag/argumentParserWrapper.hpp"
#include "ftag/database.hpp"

int main(int argc, char const* argv[]) {
  ftag::Database db;
  ftag::ArgumentParserWrapper wrapper;

  for (auto i = 0; i < argc; i++) {
    std::cout << argv[i] << std::endl;
  }

  wrapper.parseArguments(argc, argv);

  return 0;
}
