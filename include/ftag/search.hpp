#pragma once

#include <unistd.h>

#include <vector>

#include "argparse/argparse.hpp"
#include "ftag/tag_data.hpp"

namespace ftag {

class Search {
public:
  Search() = delete;

  static void AddSearchParser(argparse::ArgumentParser& program) {
    argparse::ArgumentParser search_command("search");
    search_command.add_description("Search for a tag");
    program.add_subparser(search_command);
  }

  static int search(const argparse::ArgumentParser& program) {
    FileInfo database_struct;
    std::vector<std::string> raw_args =
        program.get<std::vector<std::string>>("raw_args");

    // Print all arguments
    std::cout << "Raw arguments received:" << std::endl;
    for (const auto& arg : raw_args) {
      std::cout << arg << std::endl;
    }
    return 0;
  }
};
}  // namespace ftag
