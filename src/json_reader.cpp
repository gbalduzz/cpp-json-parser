#include "json/json_reader.hpp"

#include <fstream>

namespace json {

JSONReader::JSONReader() {
  open_groups_.push(&root_);
}

void JSONReader::open_file(const std::string& filename) {
  std::ifstream inp(filename);

  if (!inp) {
    throw(std::runtime_error("Can not open file " + filename));
  }

  std::stringstream stream;
  stream << inp.rdbuf();
  inp.close();

  try {
    root_.read(stream);
  }
  catch (const std::logic_error& err) {
    throw(std::logic_error("File " + filename + ":\n" + err.what()));
  }
}

void JSONReader::close_file() {
  root_.clear();
  while (!open_groups_.empty())
    open_groups_.pop();
}

void JSONReader::open_group(const std::string& name) {
  auto new_group = open_groups_.top()->getGroup(name);
  if (!new_group)
    throw(std::logic_error("Group " + name + " does not exist"));
  open_groups_.push(new_group);
}

void JSONReader::close_group() {
  if (open_groups_.size() == 1) {
    throw(std::logic_error("Can't close root group."));
  }

  open_groups_.pop();
}

}  // namespace json
