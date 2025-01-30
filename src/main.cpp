#include <unistd.h>

#include <exception>
#include <iostream>

#include "argparse/argparse.hpp"
#include "ftag/file_importer.hpp"
#include "ftag/search.hpp"

int main(int argc, char* argv[]) {
  argparse::ArgumentParser program("ftag", "0.0.1");
  program.add_argument("--verbose").help("increase output verbosity").flag();

  // ftag import
  argparse::ArgumentParser import_command("import");
  import_command.add_description("Handle import functionality");

  import_command.add_argument("--autotag")
      .help("enable automatic tagging")
      .flag();
  import_command.add_argument("files").help("files to import").remaining();

  import_command.add_epilog(
      "If no files are explicitly given ftag scan all files in the current "
      "working dir.\n\n"
      "The import function also support pipes e.g.\n"
      " - find . | ftag import\n"
      " - ftag import < list_of_files.txt\n\n");

  program.add_subparser(import_command);

  // ftag search
  argparse::ArgumentParser search_command("search");
  search_command.add_description("Search for a tag");

  program.add_subparser(search_command);

  // Parse arguments
  try {
    program.parse_args(argc, argv);
  } catch (const std::exception& err) {
    std::cerr << err.what() << std::endl;
    std::cerr << program << std::endl;
    std::exit(1);
  }

  // Call import
  if (program.is_subcommand_used("import")) {
    ftag::FileImporter importer(program["--verbose"] == true,
                                import_command["--autotag"] == true);

    // TODO: Check if remaining args has values and if yes use explicit import
    try {
      return importer.import(program.get<std::vector<std::string>>("files"));
    } catch (std::logic_error) {
      return importer.import();
    }
  }

  // Call search
  if (program.is_subcommand_used("search")) {
    // TODO
    std::cout << "Search Command Executed" << std::endl;
    // return ftag::Search::search(program);
    return 0;
  }

  std::cerr << program << std::endl;
  return 0;
}
