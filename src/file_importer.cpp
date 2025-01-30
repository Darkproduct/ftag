#include "ftag/file_importer.hpp"

#include <iostream>
#include <stdexcept>

namespace ftag {
FileImporter::FileImporter(const ImportOptions& options) : options(options) {
  std::cerr << "import command: verbose=" << options.verbose
            << ", autotag=" << options.autotag << std::endl;
}

void FileImporter::importFileWalk() const {
  std::vector<std::filesystem::path> paths;

  for (auto const& dir_entry :
       std::filesystem::recursive_directory_iterator(".")) {
    paths.emplace_back(dir_entry.path());
  }

  return filterFiles(paths);
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

  return filterFiles(paths);
}

void FileImporter::filterFiles(
    const std::vector<std::filesystem::path>& paths) const {
  std::cerr << "Found " << paths.size() << " files to import" << std::endl;

  // TODO:
  // 1. Filter files
  //   - ignore hidden files
  //   - ignore some files types? well see
  //   - maybe only add some files (images, videos, pdfs)
  // 2. Add tags
  //   - add default tags (file creation data, file size, more?)
  //   - for images and other media files extract them from file
  //   - if auto tagging, use AI to determine image content?
  // 3. Add files and tags to database
}
}  // namespace ftag
