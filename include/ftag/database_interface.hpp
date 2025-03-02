#pragma once

#include <filesystem>
#include <ftag/database.hpp>
#include <ftag/database_interface.hpp>
#include <ftag/database_statement.hpp>
#include <ftag/tag_data.hpp>
#include <vector>

namespace ftag {

void addFilesToDB(std::vector<FileInfo>& files);
std::vector<std::filesystem::path> findFiles(const std::filesystem::path& root,
                                             bool ignore_hidden,
                                             bool respect_gitignore);
std::vector<FileInfo> extractTags(
    const std::vector<std::filesystem::path>& files);
void autoTagMetaData(std::vector<FileInfo>& files);

}  // namespace ftag
