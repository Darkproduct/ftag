#include "ftag/argumentParserWrapper.hpp"

#include <unistd.h>

#include <filesystem>
#include <iostream>

#include "argparse/argparse.hpp"

argumentParserWrapper::argumentParserWrapper() : program("ftag", "0.0.1") {
  InitializeCommands();
}

void argumentParserWrapper::parseArguments(int argc, char const* argv[]) {
  try {
    program.parse_args(argc, argv);

    // Dispatch to the correct handler based on the subcommand
    for (const auto& [command, handler] : commandHandlers) {
      if (program.is_subcommand_used(command)) {
        handler();  // Call the corresponding handler function
        return;
      }
    }

    // If no subcommand was provided
    std::cerr << "No valid command provided.\n";
    std::cerr << program << std::endl;
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    std::cerr << program;
  }
}

void argumentParserWrapper::InitializeCommands() {
  // set up import command, add configs to importParser, add arguments, add to
  // program and update map
  argparse::ArgumentParser importParser("import",
                                        "Handle import functionality");

  importParser.add_argument("--autotag")
      .help("Enable automatic tagging")
      .default_value(false)
      .implicit_value(true);

  program.add_subparser(importParser);
  // Maybe make the function static?
  commandHandlers["import"] = [this]() { handleImportCommand(); };
}

void argumentParserWrapper::handleImportCommand() {
  // returns true when inout is from terminal
  if (isatty(fileno(stdin))) {
    // When input is from a terminal
    std::cout << "Processing directory contents recursively:\n";
    for (auto const& dir_entry :
         std::filesystem::recursive_directory_iterator(".")) {
      std::cout << dir_entry.path().string() << std::endl;
    }
  } else {
    // When input is from a pipe or file
    std::cout << "Reading from standard input:\n";
    for (std::string str{}; std::getline(std::cin, str);) {
      std::cout << str << std::endl;
    }
  }
}
