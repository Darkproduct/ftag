#include "ftag/sync_and_track.hpp"

#include <filesystem>
#include <vector>

#include "ftag/database.hpp"
#include "ftag/database_interface.hpp"
#include "ftag/tag_data.hpp"

namespace ftag {

void syncDatabase(const SyncTagOptions& options,
                  std::vector<std::filesystem::path> directories) {
  Database db(options.db_path);
  std::string update_entry_query =
      "UPDATE files SET name = ?, size = ?, last_modified = ? WHERE path = ?";

  std::string delete_entry_query = "DELETE FROM files WHERE path = ? ";

  std::string get_files_column =
      "SELECT * FROM files";  // TODO:  get only files which are untracked by
                              // folder achange SQL

  if (options.tracked == true) {
    Statement get_files(db, get_files_column);
    std::vector<std::filesystem::path> paths_from_db;
    get_files.executeStep();
    for (auto const& path : paths_from_db) {
      if (std::filesystem::exists(path)) {
        Statement update_file(db, update_entry_query);
        std::vector<FileInfo> file_info_from_path = extractTags({path});
        update_file.bindMany(
            file_info_from_path[0].file_name, file_info_from_path[0].file_size,
            file_info_from_path[0].last_modified, file_info_from_path[0].path);
        update_file.executeStep();
        update_file.reset();
      } else {
        Statement delete_entry(db, delete_entry_query);
        delete_entry.bindMany(path);
        delete_entry.executeStep();
        delete_entry.reset();
      }
    }
  } else {
    // TODO: 2 Things go trough all files that are tracked by diectories and
    //  look fo rhtem in the filesystem
    //  Them go trough all folders that are trakced and check if the file is
    //  tracked if nto add it they are already updated.
  }
}
}  // namespace ftag
