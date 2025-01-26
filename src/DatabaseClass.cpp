#include "DatabaseClass.h"

#include <stdio.h>

#include <filesystem>

DatabaseClass::DatabaseClass() {
  createDirectory();
  createDatbase();
}

DatabaseClass::~DatabaseClass() { sqlite3_close(db); }

void DatabaseClass::createDirectory() {
  if (!std::filesystem::exists(this->data_dir)) {
    if (std::filesystem::create_directories(this->data_dir)) {
      std::cerr << "Couldn't create dir" << data_dir << std::endl;
    }
  }
}

void DatabaseClass::createDatabase() {
  if (!std::filesystem::exists(this->database_filename)) {
    int rc =
        sqlite3_open_v2(database_path.c_str(), &this->db,
                        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (rc) {
      std::cerr << "Can't create database: " << sqlite3_errmsg(this->db)
                << std::endl;
      sqlite3_close(this->db);
      return 1;
    }

    // TODO use R values
    char** errmsg;
    std::string query_create_files_table =
        std::string("CREATE TABLE files (id INTEGER PRIMARY KEY, path TEXT)");
    if (sqlite3_exec(db, query_create_files_table.c_str(), callback, nullptr,
                     errmsg)) {
      std::cerr << "Error creating table. Error: " << *errmsg << std::endl;
      sqlite3_close(db);
      return 1;
    }

    auto query_create_tags_table =
        std::string("CREATE TABLE tags (id INTEGER PRIMARY KEY, name TEXT)");
    if (sqlite3_exec(db, query_create_tags_table.c_str(), callback, nullptr,
                     errmsg)) {
      std::cerr << "Error creating table. Error: " << *errmsg << std::endl;
      sqlite3_close(db);
      return 1;
    }

    auto query_create_tag_map_table = std::string(
        "CREATE TABLE tagmap (id INTEGER, file_id INTEGER, "
        "tag_id INTEGER, FOREIGN KEY(file_id) REFERENCES "
        "files(id), FOREIGN KEY(tag_id) REFERENCES tags(id))");
    if (sqlite3_exec(db, query_create_tag_map_table.c_str(), callback, nullptr,
                     errmsg)) {
      std::cerr << "Error creating table. Error: " << *errmsg << std::endl;
      sqlite3_close(db);
      return 1;
    }

    /*sqlite3_stmt **ps_create_table;*/
    /*char **pzTail;*/
    /*sqlite3_prepare_v2(db, query_create_files_table,*/
    /*                   query_create_files_table.size(), ps_create_table,*/
    /*                   pzTail);*/
    /**/
    /*sqlite3_step(ps_create_table);*/
  } else {
    // TODO use read only flag if the arguments don't need to edit the database
    int rc = sqlite3_open_v2(database_path.c_str(), &db, SQLITE_OPEN_READWRITE,
                             nullptr);
    if (rc) {
      std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
      sqlite3_close(db);
      return 1;
    }
  }
}

void getFiles() {
  try {
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"),
                                                  pclose);
    if (!pipe) {
      throw std::runtime_error("Failed to open pipe");
    }

    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe.get()) != nullptr) {
      std::string line(buffer);
      if (!line.empty() && line.back() == '\n') {
        line.pop_back();
      }
      result.push_back(line);
    }

  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
}
