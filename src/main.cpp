#include <unistd.h>

#include <cstdlib>
#include <exception>
#include <format>
#include <iostream>
#include <ostream>

#include "argparse/argparse.hpp"
#include "ftag/abort.hpp"
#include "ftag/file.hpp"
#include "ftag/import.hpp"
#include "ftag/search.hpp"
#include "ftag/sync.hpp"
#include "ftag/tag.hpp"

std::filesystem::path getDatabasePath() {
  std::string home_dir = std::filesystem::path(std::getenv("HOME"));

  std::filesystem::path data_dir =
      home_dir / std::filesystem::path(".local/state/ftag");

  if (!std::filesystem::exists(data_dir)) {
    if (!std::filesystem::create_directories(data_dir)) {
      ftag::abort(std::format("Couldn't create dir '{}'", data_dir.string()));
    }
  }

  constexpr char database_filename[] = "ftag.db";
  return data_dir / database_filename;
}

int main(int argc, char* argv[]) {
  argparse::ArgumentParser program("ftag", "0.0.1");

  const auto default_db_path = getDatabasePath();

  // ftag import
  ftag::ImportOptions import_options;
  argparse::ArgumentParser import_command("import");
  import_command.add_description("import files and directories");

  import_command.add_argument("-a", "--all")
      .help("do not ignore entries starting with .")
      .store_into(import_options.ignore_hidden);
  import_command.add_argument("--autotag")
      .help("enable automatic tagging")
      .store_into(import_options.autotag);
  import_command.add_argument("--database")
      .help("Path to the database file to use")
      .default_value(default_db_path)
      .store_into(import_options.db_path);
  import_command.add_argument("--no-ignore")
      .help("When set, ignore files such as .gitignore, will not be respected.")
      .store_into(import_options.respect_gitignore);
  import_command.add_argument("--reimport")
      .help("update files already known by ftag")
      .store_into(import_options.reimport);
  import_command.add_argument("--verbose")
      .help("Increase output verbosity")
      .store_into(import_options.verbose);

  import_command.add_argument("paths")
      .help("files and directories to import")
      .remaining();

  import_command.add_epilog(
      "If no files are explicitly given ftag scan all files in the current "
      "working dir.\n\n"
      "The import function also support pipes e.g.\n"
      " - find . | ftag import\n"
      " - ftag import < list_of_files.txt");

  program.add_subparser(import_command);
  // ftag sync
  ftag::SyncOptions sync_options;
  argparse::ArgumentParser sync_command("sync");
  sync_command.add_description("Sync directories which are tracked");

  // ftag search
  ftag::SearchOptions search_options;
  argparse::ArgumentParser search_command("search");
  search_command.add_description("Search for a tag");
  search_command.add_argument("--database")
      .help("Path to the database file to use")
      .default_value(default_db_path)
      .store_into(search_options.db_path);
  search_command.add_argument("--verbose")
      .help("Increase output verbosity")
      .store_into(search_options.verbose);
  search_command.add_argument("pattern").help("search");
  program.add_subparser(search_command);

  // ftag tag
  argparse::ArgumentParser tag_command("tag");
  tag_command.add_description("Add, remove, edit or find tags");

  // ftag track
  argparse::ArgumentParser track_command("track");
  track_command.add_description("Track directories");
  track_command.add_description("directories");

  // ftag tag add
  ftag::AddTagOptions add_tag_options;
  argparse::ArgumentParser tag_add_command("add");
  tag_add_command.add_description("Add new tag");
  tag_add_command.add_argument("--database")
      .help("Path to the database file to use")
      .default_value(default_db_path)
      .store_into(add_tag_options.db_path);
  tag_add_command.add_argument("--verbose")
      .help("Increase output verbosity")
      .store_into(add_tag_options.verbose);
  tag_add_command.add_argument("tag").help("Tag to add");
  tag_add_command.add_argument("aliases")
      .help("Aliases for this tag")
      .remaining();
  tag_command.add_subparser(tag_add_command);

  // ftag tag remove
  ftag::RemoveTagOptions remove_tag_options;
  argparse::ArgumentParser tag_remove_command("remove");
  tag_remove_command.add_description("Remove tag");
  tag_remove_command.add_argument("--database")
      .help("Path to the database file to use")
      .default_value(default_db_path)
      .store_into(remove_tag_options.db_path);
  tag_remove_command.add_argument("--verbose")
      .help("Increase output verbosity")
      .store_into(remove_tag_options.verbose);
  tag_remove_command.add_argument("tag").help("Tag to remove");
  tag_command.add_subparser(tag_remove_command);

  // ftag tag edit
  ftag::EditTagOptions edit_tag_options;
  argparse::ArgumentParser tag_edit_command("edit");
  tag_edit_command.add_description("Edit tag");
  tag_edit_command.add_argument("--database")
      .help("Path to the database file to use")
      .default_value(default_db_path)
      .store_into(edit_tag_options.db_path);
  tag_edit_command.add_argument("--verbose")
      .help("Increase output verbosity")
      .store_into(edit_tag_options.verbose);
  tag_edit_command.add_argument("tag").help("Tag to edit");
  tag_command.add_subparser(tag_edit_command);

  // ftag tag find
  ftag::FindTagOptions find_tag_options;
  argparse::ArgumentParser tag_find_command("find");
  tag_find_command.add_description("Find tag");
  tag_find_command.add_argument("--database")
      .help("Path to the database file to use")
      .default_value(default_db_path)
      .store_into(find_tag_options.db_path);
  tag_find_command.add_argument("--verbose")
      .help("Increase output verbosity")
      .store_into(find_tag_options.verbose);
  tag_find_command.add_argument("tag").help("Tag or alias to find");
  tag_command.add_subparser(tag_find_command);
  program.add_subparser(tag_command);

  // ftag file
  argparse::ArgumentParser file_command("file");
  file_command.add_description("Change file tags");

  // ftag file tag
  argparse::ArgumentParser file_tag_command("tag");
  file_tag_command.add_description("Change file tags");

  // ftag file tag add
  ftag::AddTagsFileOptions add_tags_file_options;
  argparse::ArgumentParser file_tag_add_command("add");
  file_tag_add_command.add_description("Add tag to file");
  file_tag_add_command.add_argument("--database")
      .help("Path to the database file to use")
      .default_value(default_db_path)
      .store_into(add_tags_file_options.db_path);
  file_tag_add_command.add_argument("--verbose")
      .help("Increase output verbosity")
      .store_into(add_tags_file_options.verbose);
  file_tag_add_command.add_argument("file").help("File to add tags to");
  file_tag_add_command.add_argument("tags")
      .help("Tags to add to the file")
      .nargs(argparse::nargs_pattern::at_least_one);
  file_tag_command.add_subparser(file_tag_add_command);

  // ftag file tag remove
  ftag::RemoveTagsFileOptions remove_tags_file_options;
  argparse::ArgumentParser file_tag_remove_command("remove");
  file_tag_remove_command.add_description("Remove tag from file");
  file_tag_remove_command.add_argument("--database")
      .help("Path to the database file to use")
      .default_value(default_db_path)
      .store_into(remove_tags_file_options.db_path);
  file_tag_remove_command.add_argument("--verbose")
      .help("Increase output verbosity")
      .store_into(remove_tags_file_options.verbose);
  file_tag_remove_command.add_argument("file").help("File to remove tags from");
  file_tag_remove_command.add_argument("tags")
      .help("Tags to remove from the file")
      .nargs(argparse::nargs_pattern::at_least_one);
  file_tag_command.add_subparser(file_tag_remove_command);
  file_command.add_subparser(file_tag_command);
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
    std::vector<std::string> paths;
    try {
      paths = import_command.get<std::vector<std::string>>("files");
    } catch (std::logic_error) {
      if (!isatty(fileno(stdin))) {
        for (std::string str{}; std::getline(std::cin, str);) {
          paths.emplace_back(str);
        }
      }
    }
    ftag::import(import_options, paths);
  }

  // Call search
  else if (program.is_subcommand_used("search")) {
    auto pattern = search_command.get<std::string>("pattern");
    ftag::search(search_options, pattern);
  }

  // Call tag
  else if (program.is_subcommand_used("tag")) {
    if (tag_command.is_subcommand_used("add")) {
      auto tag = tag_add_command.get<std::string>("tag");
      auto aliases = tag_add_command.get<std::vector<std::string>>("aliases");
      ftag::addTag(add_tag_options, tag, aliases);
    } else if (tag_command.is_subcommand_used("remove")) {
      auto tag = tag_remove_command.get<std::string>("tag");
      ftag::removeTag(remove_tag_options, tag);
    } else if (tag_command.is_subcommand_used("edit")) {
      auto tag = tag_edit_command.get<std::string>("tag");
      ftag::editTag(edit_tag_options, tag);
    } else if (tag_command.is_subcommand_used("find")) {
      auto tag = tag_find_command.get<std::string>("tag");
      ftag::findTag(find_tag_options, tag);
    } else {
      std::cerr << "No command given" << std::endl;
      std::cerr << tag_command << std::endl;
      return 1;
    }
  }

  // Call file
  else if (program.is_subcommand_used("file")) {
    if (file_command.is_subcommand_used("tag")) {
      if (file_tag_command.is_subcommand_used("add")) {
        auto file = file_tag_add_command.get<std::string>("file");
        auto tags = file_tag_add_command.get<std::vector<std::string>>("tags");
        ftag::addTagsFile(add_tags_file_options, file, tags);
      } else if (file_tag_command.is_subcommand_used("remove")) {
        auto file = file_tag_remove_command.get<std::string>("file");
        auto tags =
            file_tag_remove_command.get<std::vector<std::string>>("tags");
        ftag::removeTagsFile(remove_tags_file_options, file, tags);
      } else {
        std::cerr << "No command given" << std::endl;
        std::cerr << file_tag_command << std::endl;
        return 1;
      }
    } else {
      std::cerr << "No command given" << std::endl;
      std::cerr << file_command << std::endl;
      return 1;
    }
  } else {
    std::cerr << "No command given" << std::endl;
    std::cerr << program << std::endl;
    return 1;
  }

  return 0;
}
