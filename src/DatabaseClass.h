
#include <unistd.h>

#include <cassert>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <vector>

#include "sqlite3.h"

class DatabaseClass {
  sqlite3* db;

  std::string home_dir = std::filesystem::path(std::getenv("HOME"));
  std::filesystem::path data_dir =
      home_dir / std::filesystem::path(".local/state/ftag");

  std::string database_filename = "ftag.db";
  std::filesystem::path database_path = data_dir / database_filename;

  std::vector<std::string> file_paths;

  DatabaseClass();
  ~DatabaseClass();

  void createDirectory();
  void createDatbase();
  void createTables();
  void getFiles();
};
