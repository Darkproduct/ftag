#include <iostream>
#include <vector>

#include "ftag/TagClass.hpp"
#include "ftag/database.hpp"

namespace ftag {

TagClass::TagClass(const TagClass::ImportOptions& options,
                   const Database& databse)
    : options(options), database(database) {}

void TagClass::addtag(const std::vector<std::string>& input_files) {}

void TagClass::start(const std::vector<std::string>& input_files) {
  if (options.addtag == true) {
    TagClass::addtag(input_files);
  }
}
}  // namespace ftag
