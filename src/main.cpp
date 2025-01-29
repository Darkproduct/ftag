#include <unistd.h>

#include <iostream>

#include "argparse/argparse.hpp"
#include "ftag/file_importer.hpp"

int main(int argc, const char* argv[]) {
  argparse::ArgumentParser program("ftag", "0.0.1");

  ftag::FileImporter::AddImportParser(program);

  program.parse_args(argc, argv);

  if (program.is_subcommand_used("import")) {
    return ftag::FileImporter::import(program);
  } else {
    std::cerr << program << std::endl;
  }

  return 0;
}
