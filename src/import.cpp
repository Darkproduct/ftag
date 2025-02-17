#include "ftag/import.hpp"

#include <filesystem>
#include <iostream>

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

  extractTags(files);
}

void extractTags(const std::vector<std::filesystem::path>& files) {
  std::vector<FileInfo> filesToImpot;
  // TODO:
  // 2. Add tags
  //   - add default tags (should all be obtainable from std::filesystem)
  //     - file name (without extension)
  //       - Maybe we should do some cleanup here (e.g. replace "_" with " ")
  //     - file extension
  //     - file creation date
  //     - file size
  //   - for images and other media, extract metadata
  //   - if auto-tagging is enabled, use AI to determine content?
  // 3. Add files and tags to database
}

}  // namespace ftag
