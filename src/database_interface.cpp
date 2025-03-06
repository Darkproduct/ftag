#include "ftag/database_interface.hpp"

#include <cstdint>
#include <ftag/database.hpp>
#include <ftag/database_interface.hpp>
#include <ftag/database_statement.hpp>
#include <ftag/tag_data.hpp>
#include <iostream>

namespace ftag {

void addFilesToDB(const std::vector<FileInfo>& file_infos,
                  const std::filesystem::path& db_path) {
  constexpr static char import_or_update_files_query[] =
      "INSERT INTO files (name, path, size, last_modified)"
      "VALUES (:name, :path, :size, :last_mod)"
      "ON CONFLICT(path) DO UPDATE SET "
      "name=:name,"
      "size=:size,"
      "last_modified=:last_mod "
      "RETURNING id";
  constexpr static char insert_tags_query[] =
      "INSERT INTO tags (name)"
      "VALUES (:name)"
      "ON CONFLICT(name) DO UPDATE SET id=id "
      "RETURNING id";
  constexpr static char tag_files_query[] =
      "INSERT OR IGNORE INTO tagmap (file_id, tag_id)"
      "VALUES (:file_id, :tag_id)";

  Database db(db_path);
  Statement import_file(db, import_or_update_files_query);
  Statement import_tag(db, insert_tags_query);
  Statement tag_file(db, tag_files_query);

  for (const auto& file_info : file_infos) {
    std::string path_storeage = file_info.path.string();
    import_file.bindMany(file_info.file_name, path_storeage,
                         static_cast<uint64_t>(file_info.file_size),
                         file_info.last_modified);
    auto file_id_opt = import_file.executeStep<uint64_t>();
    auto file_id = std::get<0>(*file_id_opt);
    import_file.reset();

    // TODO: Not tested...
    for (const auto& tag : file_info.tags) {
      std::cerr << "add tag: " << tag.name << std::endl;

      import_tag.bindMany(tag.name);
      auto tag_id_opt = import_tag.executeStep<uint64_t>();
      auto tag_id = std::get<0>(*tag_id_opt);
      import_tag.reset();

      tag_file.bindMany(file_id, tag_id);
      tag_file.executeStep();
      tag_file.reset();
    }
  }
}

}  // namespace ftag
