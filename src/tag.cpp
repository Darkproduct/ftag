#include "ftag/TagClass.hpp"
#include "ftag/tag_data.hpp"

namespace ftag {

TagClass::TagClass(const Options& options)
    : options(options), database(options.db_path) {}

void TagClass::addTag(const std::vector<std::string>& input_files) {
  Tag tag_data;
  tag_data.name =
      input_files[0];  // I just assume that the first entry is the tag
}

}  // namespace ftag
