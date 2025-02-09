#include "ftag/file_importer.hpp"

#include <iostream>

namespace ftag {

FileImporter::FileImporter(const Options& options)
    : options(options), database(options.db_path) {}

void FileImporter::importFileWalk() const {
  std::vector<std::filesystem::path> paths;

  for (const auto& dir_entry :
       std::filesystem::recursive_directory_iterator(".")) {
    paths.emplace_back(dir_entry.path());
  }

  filterFiles(paths);
}

void FileImporter::import(const std::vector<std::string>& files) const {
  std::vector<std::filesystem::path> paths;

  for (const auto& f : files) {
    if (!std::filesystem::exists(f)) {
      std::cerr << "provided file '" << f << "' does not exist" << std::endl;
      std::exit(1);
    }
    paths.emplace_back(f);
  }

  filterFiles(paths);
}

void FileImporter::filterFiles(
    const std::vector<std::filesystem::path>& paths) const {
  std::cerr << "Found " << paths.size() << " files to import" << std::endl;

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
