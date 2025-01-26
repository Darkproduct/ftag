#include <sqlite3.h>
#include <unistd.h>

#include <cassert>
#include <cstdlib>
#include <filesystem>
#include <iostream>

static int callback(void* NotUsed, int argc, char** argv, char** azColName) {
  std::cerr << "Callback of sql statement" << std::endl;
  return 0;
}

int main(int argc, char const* argv[]) {
  /*if (isatty(fileno(stdin))) {*/
  /*  std::cerr << "No pipe found" << std::endl;*/
  /*  return 1;*/
  /*}*/

  /*
    auto home_dir = std::filesystem::path(std::getenv("HOME"));
    auto data_dir = home_dir / std::filesystem::path(".local/state/ftag");
    if (!std::filesystem::exists(data_dir)) {
      if (!std::filesystem::create_directory(data_dir)) {
        std::cerr << "Couln't create dir: " << data_dir << std::endl;
        return (1);
      }
    }

    auto database_filename = "ftag.db";
    auto database_path = data_dir / database_filename
    sqlite3* db;
    */

  if (!std::filesystem::exists(database_path)) {
    int rc =
        sqlite3_open_v2(database_path.c_str(), &db,
                        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (rc) {
      std::cerr << "Can't create database: " << sqlite3_errmsg(db) << std::endl;
      sqlite3_close(db);
      return 1;
    }

    // TODO use R values
    //
    char** errmsg;
    auto query_create_files_table =
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

  assert(db);

  // TODO do shit
  /*char *zErrMsg = 0;*/
  /*int err = sqlite3_exec(db, argv[2], callback, 0, &zErrMsg);*/
  /*if (err != SQLITE_OK) {*/
  /*  fprintf(stderr, "SQL error: %s\n", zErrMsg);*/
  /*  sqlite3_free(zErrMsg);*/
  /*}*/

  sqlite3_close(db);
  return 0;
}
