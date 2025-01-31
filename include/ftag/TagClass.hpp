#pragma once

#include <vector>

#include "ftag/search.hpp"
#include "ftag/tag_data.hpp"

namespace ftag {
class TagClass {
public:
  struct ImportOptions {};
  Tag(const ImportOptions& options);

  // TODO: Different vectors for tags and files names?
  // TODO: At the moment one file and one tag only
  void tag(const std::vector<std::string>& input_data);

private:
  ImportOptions options;
};

}  // namespace ftag
