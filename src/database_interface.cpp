#include "ftag/database_interface.hpp"

#include <cstdint>
#include <ftag/database.hpp>
#include <ftag/database_interface.hpp>
#include <ftag/database_statement.hpp>
#include <ftag/tag_data.hpp>
#include <iostream>

namespace ftag {

void addFilesToDB(const std::vector<FileInfo>& files,
                  const std::filesystem::path& db_path) {
  constexpr static char import_files_query[] =
      "INSERT INTO files (name, path, size, last_modified)"
      "VALUES (:name, :path, :size, :last_mod)"
      "ON DUPLICATE KEY "
      "UPDATE "
      "name = VALUES(:name),"
      "path = VALUES(:path),"
      "size = VALUES(:size),"
      "last_modified = VALUES(:last_mod);";

  // TODO: Fix insert/update query
  std::cerr << import_files_query << std::endl;

  Database db(db_path);
  Statement import_file(db, import_files_query);

  for (const auto& file : files) {
    std::string path_storeage = file.path.string();
    import_file.bindMany(file.file_name, path_storeage,
                         static_cast<uint64_t>(file.file_size),
                         file.last_modified);
    import_file.executeStep();
    import_file.reset();
  }
}

}  // namespace ftag
