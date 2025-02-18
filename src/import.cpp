#include "ftag/import.hpp"

#include <ctime>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include "ftag/database.hpp"
#include "ftag/database_statement.hpp"
#include "ftag/tag_data.hpp"

namespace ftag {

void import(const ImportOptions& options,
            const std::vector<std::string>& paths) {
  if (paths.size() == 0) {
    // TODO: Maybe add a question [Y/n] or something if you really want to
    // add the cwd. Can also be set with -y or something
    filterFiles(
        findFiles(".", options.ignore_hidden, options.respect_gitignore));
  } else {
    std::vector<std::filesystem::path> files(paths.size());

    for (const auto& p : paths) {
      if (!std::filesystem::exists(p)) {
        std::cerr << "provided path '" << p << "' does not exist" << std::endl;
        std::exit(1);
      }

      if (std::filesystem::is_directory(p)) {
        auto f = findFiles(p, options.ignore_hidden, options.respect_gitignore);
        files.insert(files.end(), f.begin(), f.end());
      } else if (std::filesystem::is_regular_file(p)) {
        files.emplace_back(p);
      }
    }

    filterFiles(files);
    auto tagged_files = extractTags(files);
    addFilesToDB(options, tagged_files);
  }
}

std::vector<std::filesystem::path> findFiles(const std::filesystem::path& root,
                                             bool ignore_hidden,
                                             bool respect_gitignore) {
  std::vector<std::filesystem::path> files;

  auto it = std::filesystem::recursive_directory_iterator(root);
  for (const auto& dir_entry : it) {
    bool ignore =
        ignore_hidden || dir_entry.path().filename().string().starts_with(".");

    if (dir_entry.is_directory() && ignore) {
      it.disable_recursion_pending();
    } else if (dir_entry.is_directory() && respect_gitignore) {
      // TODO:
      // 1. Find .gitignore
      //    We need to check on every directory we recurse into if it
      //    contains a gitignore file. This would be the only way to ensure no
      //    files are already commited to the files vector
      // 2. read gitignore and apply it
    } else if (dir_entry.is_regular_file() && !ignore) {
      files.emplace_back(dir_entry.path());
    }
  }

  return files;
}

void filterFiles(const std::vector<std::filesystem::path>& files) {
  std::cerr << "Found " << files.size() << " files to import" << std::endl;

  // TODO:
  // 1. Filter files
  //   - ignore some file types? (TBD)
  //   - maybe only add some files (images, videos, PDFs)
}

std::vector<FileInfo> extractTags(
    const std::vector<std::filesystem::path>& files) {
  std::vector<FileInfo>& filesToImport;

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
  }

  autoTagMetaData(filesToImport);
  return filesToImport;
}

void autoTagMetaData(std::vector<FileInfo>& files) {
  // TODO: Which  library to use?

  return;
}

void addFilesToDB(const ImportOptions& options, std::vector<FileInfo>& files) {
  std::string check_query = "SELECT EXISTS(SELECT 1 FROM files WHERE path = ?)";
  std::string import_files_query =
      "INSERT INTO files (name, path, size, last_modified) VALUES (?, ?, ?, "
      "?);";
  std::string update_entry_query =
      "UPDATE files SET name = ?, size = ?, last_modified = ? WHERE path = ?";

  Database db(options.db_path);

  for (auto const& file : files) {
    Statement check_existance(db, check_query);
    check_existance.bind(file.path);

    if (!check_existance.executeStep()) {
      Statement import_file(db, import_files_query);
      import_file.bindMany(file.file_name, file.path, file.file_size,
                           file.last_modified);
      import_file.executeStep();
      import_file.reset();

    } else {
      Statement update_file(db, update_entry_query);
      update_file.bindMany(file.file_name, file.file_size, file.last_modified,
                           file.path);
      update_file.executeStep();
      update_file.reset();
    }
    check_existance.reset();
  }
}

// TODO: Pass Databse to a update enrty function or two seeprate

void UpdateDB(const ImportOptions& options) {
  Database db(options.db_path);

  std::string update_entry_query =
      "UPDATE files SET name = ?, size = ?, last_modified = ? WHERE path = ?";

  std::string delete_entry_query = "DELETE FROM files WHERE path = ? ";

  std::string get_files_column = "SELECT * FROM files";
  Statement get_files(db, get_files_column);

  get_files.executeStep();
  // TODO: retruns amount of colums emplace possible add them to the vector
  std::vector<std::string> paths_from_db;

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
      delete_entry.bind(path);
      delete_entry.executeStep();
      delete_entry.reset();
    }
  }
}

}  // namespace ftag
