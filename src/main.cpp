#include <unistd.h>

#include <exception>
#include <iostream>
#include <ostream>

#include "argparse/argparse.hpp"
#include "ftag/TagClass.hpp"
#include "ftag/database.hpp"
#include "ftag/file_importer.hpp"
#include "ftag/search.hpp"

int main(int argc, char* argv[]) {
  ftag::Database db;

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

  search_command.add_argument("files").help("files to import").remaining();

  program.add_subparser(search_command);

  // tagging
  argparse::ArgumentParser tagging_command("tag");
  tagging_command.add_description("Halde tagging functionality");

  tagging_command.add_argument("--addtag").help("add tags").flag();
  tagging_command.add_argument("--deletetag").help("delete tags").flag();
  tagging_command.add_argument("--tagfiles").help("Tag files").flag();
  tagging_command.add_argument("--deletefiletags")
      .help("Delte tags from files")
      .flag();
  tagging_command.add_argument("files").help("files to import").remaining();
  program.add_subparser(tagging_command);

  // Parse arguments
  try {
    program.parse_args(argc, argv);
  } catch (const std::exception& err) {
    std::cerr << err.what() << std::endl;
    std::exit(1);
  }

  // Call import
  if (program.is_subcommand_used("import")) {

    std::cout << "Test" << std::endl;

    ftag::FileImporter::ImportOptions import_options;
    import_options.verbose = program["--verbose"] == true;
    import_options.autotag = import_command["--autotag"] == true;

    ftag::FileImporter importer(import_options, db);

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
  
  }

  // Call search
  else if(program.is_subcommand_used("search")) {
    ftag::Search::ImportOptions search_options;
    ftag::Search seeker(search_options, db);

    std::cout << "Search Command Executed" << std::endl;

    try {
      auto files = search_command.get<std::vector<std::string>>("files");
      std::cout << files.size() << " tags provided" << std::endl;
      seeker.search(files);

    } catch (std::logic_error& e) {
      std::cout << "No tags provided" << std::endl;
    }
    
  }

  // Tag Search
  else if(program.is_subcommand_used("tag")) {
    ftag::TagClass::ImportOptions tagging_options;
    tagging_options.verbose = program["--verbose"] == true;
    tagging_options.addtag = tagging_command["--addtag"] == true;
    tagging_options.deletetag = tagging_command["--deletetag"] == true;
    tagging_options.tagfiles = tagging_command["--tagfiles"] == true;
    tagging_options.deletefiletags = tagging_command["--deletefiletags"] == true;

    ftag::TagClass tagger(tagging_options, db);
    try {
      auto input_stream = tagging_command.get<std::vector<std::string>>("files");
      std::cout << input_stream.size() << "input provided" << std::endl;
      tagger.addtag(input_stream);
    } catch (std::logic_error& e) {
      std::cout << "No input provided" << std::endl;
    }
    
  }

  else {
  std::cout << "No command given" << std::endl;

}

  return 0;
}
