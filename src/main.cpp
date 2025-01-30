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
      " - ftag import < list_of_files.txt");

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
    ftag::FileImporter::ImportOptions import_options;
    import_options.verbose = program["--verbose"] == true;
    import_options.autotag = import_command["--autotag"] == true;

    ftag::FileImporter importer(import_options);

    try {
      auto files_argument =
          import_command.get<std::vector<std::string>>("files");
      importer.import(files_argument);
    } catch (std::logic_error) {
      if (isatty(fileno(stdin))) {
        // TODO: Maybe add a question [Y/n] or something if you really want to
        // add the cwd. Can also be set with -y or something
        importer.importFileWalk();
      } else {
        std::vector<std::string> files_piped;
        for (std::string str{}; std::getline(std::cin, str);) {
          files_piped.emplace_back(str);
        }
        importer.import(files_piped);
      }
    }
    return 0;
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
