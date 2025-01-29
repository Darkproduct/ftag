#include <unistd.h>

#include <iostream>

#include "argparse/argparse.hpp"
#include "ftag/file_importer.hpp"
// #include "ftag/search.hpp"

// Ich find doch char const* besser es ist ein char pointer der auf ein const
// zeigt
int main(int argc, const char* argv[]) {
  argparse::ArgumentParser program("ftag", "0.0.1");

  std::cout << "Search Command" << std::endl;
  ftag::FileImporter::AddImportParser(program);
  // ftag::Search::AddSearchParser(program);
  /*
    program.add_argument("raw_args")
        .remaining()  // Collect all remaining arguments
        .help("All arguments passed to the program");
  */
  program.parse_args(argc, argv);

  std::cout << "Search Command Executed" << std::endl;
  if (program.is_subcommand_used("import")) {
    return ftag::FileImporter::import(program);
  } else if (program.is_subcommand_used("search")) {
    std::cout << "Search Command Executed" << std::endl;
    // return ftag::Search::search(program);
  }

  else {
    std::cerr << program << std::endl;
  }

  return 0;
}
