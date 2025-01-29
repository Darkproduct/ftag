#pragma once

#include <unistd.h>

#include <filesystem>
#include <vector>

#include "argparse/argparse.hpp"

namespace ftag {
class FileImporter {
public:
  FileImporter() = delete;

  static void AddImportParser(argparse::ArgumentParser& program) {
    argparse::ArgumentParser import_command("import");
    import_command.add_description("Handle import functionality");

    import_command.add_argument("--autotag")
        .help("Enable automatic tagging")
        .default_value(false)
        .implicit_value(true);
    program.add_subparser(import_command);
  }

  static int import(const argparse::ArgumentParser& program) {
    std::vector<std::filesystem::path> paths;

    if (isatty(fileno(stdin))) {
      // When input is from a terminal
      for (auto const& dir_entry :
           std::filesystem::recursive_directory_iterator(".")) {
        paths.emplace_back(dir_entry.path());
      }
    } else {
      // When input is from a pipe or file
      for (std::string str{}; std::getline(std::cin, str);) {
        paths.emplace_back(str);
      }
    }

    // TODO:
    // 1. Filter files
    //   - ignore hidden files
    //   - ignore some files types? well see
    //   - maybe only add some files (images, videos, pdfs)
    // 2. Add tags
    //   - add default tags (file creation data, file size, more?)
    //   - for images and other media files extract them from file
    //   - if auto tagging, use AI to determine image content?
    // 3. Add files and tags to database

    return 0;
  }
};
}  // namespace ftag
