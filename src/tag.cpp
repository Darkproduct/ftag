#include "ftag/TagClass.hpp"
#include "ftag/tag_data.hpp"



namespace ftag {

TagClass::TagClass(const ImportOptions& options, const Database& database)
    : options(options), database(database) {}

void TagClass::addtag(const std::vector<std::string>& input_files) {
  Tag tag_data;
  tag_data.name = input_files[0]; //I just assume that the first entry is the tag
  database.AddTags(tag_data);
  
}

void TagClass::start(const std::vector<std::string>& input_files) {
  if (options.addtag == true) {
    TagClass::addtag(input_files);
  }
}
}  // namespace ftag
