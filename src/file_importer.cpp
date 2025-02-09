#include "ftag/file_importer.hpp"

#include <filesystem>
#include <iostream>

namespace ftag {

FileImporter::FileImporter(const Options& options)
    : options(options), database(options.db_path) {}

void FileImporter::import(const std::vector<std::string>& paths) const {
  if (paths.size() == 0) {
    filterFiles(findFiles("."));
  } else {
    std::vector<std::filesystem::path> files(paths.size());

    for (const auto& p : paths) {
      if (!std::filesystem::exists(p)) {
        std::cerr << "provided path '" << p << "' does not exist" << std::endl;
        std::exit(1);
      }

      if (std::filesystem::is_directory(p)) {
        auto f = findFiles(p);
        files.insert(files.end(), f.begin(), f.end());
      } else if (std::filesystem::is_regular_file(p)) {
        files.emplace_back(p);
      }
    }

    filterFiles(files);
  }
}

std::vector<std::filesystem::path> FileImporter::findFiles(
    const std::filesystem::path& path) const {
  std::vector<std::filesystem::path> files;

  auto it = std::filesystem::recursive_directory_iterator(path);
  for (const auto& dir_entry : it) {
    if (dir_entry.is_directory() && isHidden(dir_entry)) {
      it.disable_recursion_pending();
    } else if (dir_entry.is_regular_file() && !isHidden(dir_entry)) {
      files.emplace_back(dir_entry.path());
    } else if (dir_entry.is_regular_file() &&
               dir_entry.path().filename() == ".gitignore") {
      // TODO:
    }
  }

  return files;
}

bool FileImporter::isHidden(const std::filesystem::path& path) const {
  if (!options.ignore_hidden) {
    return false;
  }

  return path.filename().string().starts_with(".");
}

void FileImporter::filterFiles(
    const std::vector<std::filesystem::path>& files) const {
  std::cerr << "Found " << files.size() << " files to import" << std::endl;

  // TODO:
  // 1. Filter files
  //   - ignore hidden files
  //   - ignore some file types? (TBD)
  //   - maybe only add some files (images, videos, PDFs)
  // 2. Add tags
  //   - add default tags (file creation date, file size, etc.)
  //   - for images and other media, extract metadata
  //   - if auto-tagging is enabled, use AI to determine content?
  // 3. Add files and tags to database
}

}  // namespace ftag
