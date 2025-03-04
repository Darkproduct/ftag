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

// For a vector extract tags return the fileInfo
std::vector<FileInfo> extractTags(
    const std::vector<std::filesystem::path>& files) {
  std::vector<FileInfo> filesToImport;

  for (const auto& file : files) {
    FileInfo temp;

    temp.path = file;

    // size
    temp.file_size = std::filesystem::file_size(file);

    // name
    temp.file_name = file.filename();

    // Get last modified time
    auto ftime = std::filesystem::last_write_time(file);
    auto sctp = std::chrono::system_clock::time_point(
        std::chrono::duration_cast<std::chrono::system_clock::duration>(
            ftime.time_since_epoch()));
    std::time_t t = std::chrono::system_clock::to_time_t(sctp);

    std::tm tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");

    temp.last_modified = oss.str();
    filesToImport.push_back(temp);
  }

  autoTagMetaData(filesToImport);
  return filesToImport;
}

void autoTagMetaData(std::vector<FileInfo>& files) {
  // TODO: Which  library to use?

  return;
}

}  // namespace ftag
