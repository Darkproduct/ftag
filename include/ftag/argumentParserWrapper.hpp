

#include "argparse/argparse.hpp"

class argumentParserWrapper {
public:
  argumentParserWrapper();
  void parseArguments(int argc, char const* argv[]);

private:
  argparse::ArgumentParser program;
  argparse::ArgumentParser importParser;
  argparse::ArgumentParser searchParser;

  std::unordered_map<std::string, std::function<void()>> commandHandlers;
  void InitializeCommands();
  void handleImportCommand();
};
