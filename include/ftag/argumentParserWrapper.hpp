#pragma once

#include <functional>

#include "argparse/argparse.hpp"

namespace ftag {

class ArgumentParserWrapper {
public:
  ArgumentParserWrapper();
  void parseArguments(int argc, char const* argv[]);
  void setInportCallback(std::function<void()> callback);

private:
  void initializeCommands();
  void handleImportCommand();

private:
  argparse::ArgumentParser program;
  argparse::ArgumentParser importParser;
  argparse::ArgumentParser searchParser;

  std::unordered_map<std::string, std::function<void()>> commandHandlers;
};

}  // namespace ftag
