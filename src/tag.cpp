#include "ftag/TagClass.hpp"
#include "ftag/tag_data.hpp"

namespace ftag {

TagClass::TagClass(const Options& options)
    : options(options), database(options.db_path) {}

void TagClass::addTag(const std::vector<std::string>& input_files) {



}

}  // namespace ftag
