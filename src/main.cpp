#include <unistd.h>

#include <filesystem>
#include <iostream>
#include <iterator>

#include "argparse/argparse.hpp"
#include "ftag/argumentParserWrapper.hpp"
#include "ftag/database.hpp"

int main(int argc, char const* argv[]) {
  ftag::Database db;
  argumentParserWrapper wrapper;

  for (auto i = 0; i < argc; i++) {
    std::cout << argv[i] << std::endl;
  }

  wrapper.parseArguments(argc, argv);

  return 0;
}
