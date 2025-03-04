#include "ftag/database_interface.hpp"

#include <cstdint>
#include <ftag/database.hpp>
#include <ftag/database_interface.hpp>
#include <ftag/database_statement.hpp>
#include <ftag/tag_data.hpp>

namespace ftag {

// Add raw untrakced by directories to the DB and update if found already
void addFilesToDB(std::vector<FileInfo>& files, std::filesystem::path db_path) {
  std::string check_query = "SELECT EXISTS(SELECT 1 FROM files WHERE path = ?)";
  std::string import_files_query =
      "INSERT INTO files (name, path, size, last_modified) VALUES (?, ?, ?, "
      "?);";
  std::string update_entry_query =
      "UPDATE files SET size = ?, last_modified = ? WHERE path = ?";

  Database db(db_path);
  // TODO: Somehow return all files

  for (auto const& file : files) {
    Statement check_existance(db, check_query);
    check_existance.bind(0, file.path.string());

    if (!check_existance.executeStep()) {
      Statement import_file(db, import_files_query);
      import_file.bindMany(file.file_name, file.path.string(),
                           static_cast<uint64_t>(file.file_size),
                           file.last_modified);
      import_file.executeStep();
      import_file.reset();

    } else {
      Statement update_file(db, update_entry_query);
      update_file.bindMany(static_cast<uint64_t>(file.file_size),
                           file.last_modified, file.path.string());
      update_file.executeStep();
      update_file.reset();
    }
    check_existance.reset();
  }
}

}  // namespace ftag
