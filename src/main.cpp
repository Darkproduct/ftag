#include <unistd.h>

#include <chrono>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <ostream>

#include "argparse/argparse.hpp"
#include "ftag/TagClass.hpp"
#include "ftag/file_importer.hpp"
#include "ftag/search.hpp"

std::filesystem::path getDatabasePath() {
  std::string home_dir = std::filesystem::path(std::getenv("HOME"));

  std::filesystem::path data_dir =
      home_dir / std::filesystem::path(".local/state/ftag");

  if (!std::filesystem::exists(data_dir)) {
    if (!std::filesystem::create_directories(data_dir)) {
      std::cerr << "Couldn't create dir" << data_dir << std::endl;
      std::abort();
    }
  }

  constexpr char database_filename[] = "ftag.db";
  return data_dir / database_filename;
}

int main(int argc, char* argv[]) {
  argparse::ArgumentParser program("ftag", "0.0.1");
  program.add_argument("--verbose").help("increase output verbosity").flag();

  // ftag import
  argparse::ArgumentParser import_command("import");
  import_command.add_description("Handle import functionality");

  import_command.add_argument("--reimport")
      .help("update files already known by ftag")
      .flag();
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

  argparse::ArgumentParser search_add_command("tag");
  search_command.add_subparser(search_add_command);

  import_command.add_argument("tags").help("files to import").remaining();

  program.add_subparser(search_command);

  // ftag tag
  argparse::ArgumentParser tag_command("tag");
  tag_command.add_description("Add, edit or remove tags");

  argparse::ArgumentParser tag_add_command("add");
  tag_add_command.add_description("Add new tag");
  tag_command.add_subparser(tag_add_command);

  argparse::ArgumentParser tag_edit_command("edit");
  tag_edit_command.add_description("Edit tag");
  tag_command.add_subparser(tag_edit_command);

  argparse::ArgumentParser tag_delete_command("delete");
  tag_delete_command.add_description("Delete tag");
  tag_command.add_subparser(tag_delete_command);

  program.add_subparser(tag_command);

  // ftag file
  argparse::ArgumentParser file_command("file");
  file_command.add_description("Tag, track or untrack files");

  argparse::ArgumentParser file_tag_command("tag");
  file_tag_command.add_description("Change file tags");

  argparse::ArgumentParser file_tag_add_command("add");
  file_tag_add_command.add_description("Add tag to file");
  file_tag_command.add_subparser(file_tag_add_command);

  argparse::ArgumentParser file_tag_remove_command("remove");
  file_tag_remove_command.add_description("Remove tag from file");
  file_tag_command.add_subparser(file_tag_remove_command);

  file_command.add_subparser(file_tag_command);

  argparse::ArgumentParser file_track_command("track");
  file_track_command.add_description("Add file to ftag");
  file_command.add_subparser(file_track_command);

  argparse::ArgumentParser file_untrack_command("untrack");
  file_untrack_command.add_description("Remove file from ftag");
  file_command.add_subparser(file_untrack_command);

  program.add_subparser(file_command);

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

    ftag::FileImporter::Options import_options;
    import_options.db_path = getDatabasePath();
    import_options.verbose = program["--verbose"] == true;
    import_options.reimport = import_command["--reimport"] == true;
    import_options.autotag = import_command["--autotag"] == true;

    ftag::FileImporter importer(import_options);

    std::vector<std::string> paths;
    try {
      paths = import_command.get<std::vector<std::string>>("files");
    } catch (std::logic_error) {
      if (isatty(fileno(stdin))) {
        // TODO: Maybe add a question [Y/n] or something if you really want to
        // add the cwd. Can also be set with -y or something
        importer.import();
      } else {
        for (std::string str{}; std::getline(std::cin, str);) {
          paths.emplace_back(str);
        }
      }
    }
    importer.import(paths);
  }

  // Call search
  else if (program.is_subcommand_used("search")) {
    ftag::Search::Options search_options;
    search_options.db_path = getDatabasePath();
    search_options.verbose = program["--verbose"] == true;

    ftag::Search seeker(search_options);

    auto input_search_command =
        search_command.get<std::vector<std::string>>("tags");
    if (search_command.is_subcommand_used("tag")) {
      seeker.search_tag(input_search_command);
    }
    std::cerr << "TODO" << std::endl;
    std::abort();
  }

  // Call tag
  else if (program.is_subcommand_used("tag")) {
    ftag::TagClass::Options tag_options;
    tag_options.db_path = getDatabasePath();
    tag_options.verbose = false;

    ftag::TagClass tagger(tag_options);
    if (tag_command.is_subcommand_used("add")) {
      tagger.addTag({"Test"});
    }
    if (tag_command.is_subcommand_used("edit")) {
      std::abort();
    }

  }

  // Call file
  else if (program.is_subcommand_used("file")) {
    std::cerr << "TODO" << std::endl;
    std::abort();
  }

  else {
    std::cerr << "No command given" << std::endl;
    std::cerr << program << std::endl;
  }

  return 0;
}
