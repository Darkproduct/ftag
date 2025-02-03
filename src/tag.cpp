#include "ftag/TagClass.hpp"

namespace ftag {

TagClass::TagClass(const ImportOptions& options, const Database& database)
    : options(options), database(database) {}

void TagClass::addtag(const std::vector<std::string>& input_files) {
  // Implementation of addtag function
}

void TagClass::start(const std::vector<std::string>& input_files) {
  if (options.addtag == true) {
    TagClass::addtag(input_files);
  }
}
}  // namespace ftag
