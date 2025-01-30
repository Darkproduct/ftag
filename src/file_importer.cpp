#include "ftag/file_importer.hpp"

#include <iostream>
#include <stdexcept>

namespace ftag {
FileImporter::FileImporter(bool verbose, bool autotag) {
  std::cerr << "import command: verbose=" << verbose << ", autotag=" << autotag
            << std::endl;
}

int FileImporter::import() const {
  std::vector<std::filesystem::path> paths;

  if (isatty(fileno(stdin))) {
    // When input is from a terminal
    // TODO: Maybe add a question [Y/n] or something if you really want to add
    // the cwd. Can also be set with -y or something
    for (auto const& dir_entry :
         std::filesystem::recursive_directory_iterator(".")) {
      paths.emplace_back(dir_entry.path());
    }
  } else {
    // When input is from a pipe or file
    for (std::string str{}; std::getline(std::cin, str);) {
      paths.emplace_back(str);
    }
  }

  return filterFiles(paths);
}

int FileImporter::import(const std::vector<std::string>& files) const {
  std::vector<std::filesystem::path> paths;
  try {
    for (const auto& f : files) {
      if (!std::filesystem::exists(f)) {
        std::cerr << "provided file '" << f << "' does not exist" << std::endl;
        std::exit(1);
      }
      std::cerr << f << " exists" << std::endl;
      paths.emplace_back(f);
    }
  } catch (std::logic_error) {
  }

  return filterFiles(paths);
}

int FileImporter::filterFiles(
    const std::vector<std::filesystem::path>& paths) const {
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

  return 0;
}
}  // namespace ftag
