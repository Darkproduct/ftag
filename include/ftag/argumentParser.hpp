

#include "argparse/argparse.hpp"

class argumentParserWrapper {
public:
  argumentParserWrapper();
  void parseArguments(int argc, char* argv[]);

private:
  argparse::ArgumentParser program;
  argparse::ArgumentParser importParser;

  std::unordered_map<std::string, std::function<void()>> commandHandlers;
  void InitializeCommands();
  void handleImportCommand();
};
