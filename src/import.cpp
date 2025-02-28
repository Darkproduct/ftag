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

void filterFiles(const std::vector<std::filesystem::path>& files) {
  std::cerr << "Found " << files.size() << " files to import" << std::endl;

  // TODO:
  // 1. Filter files
  //   - ignore some file types? (TBD)
  //   - maybe only add some files (images, videos, PDFs)
}

}  // namespace ftag
