#pragma once

#include <filesystem>
#include <ftag/database.hpp>
#include <ftag/database_interface.hpp>
#include <ftag/database_statement.hpp>
#include <ftag/tag_data.hpp>
#include <vector>

namespace ftag {

void addFilesToDB(const std::vector<FileInfo>& files,
                  const std::filesystem::path& db_path);

}  // namespace ftag
