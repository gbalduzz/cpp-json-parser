#include "json/json_writer.hpp"

namespace json {

JSONWriter::JSONWriter() {
  open_groups_.push(&root_);
}

JSONWriter::~JSONWriter() {
  close_file();
}

void JSONWriter::open_file(const std::string& filename) {
  file_.open(filename);
  if (!file_) {
    throw(std::runtime_error("Can not open file " + filename));
  }
}

void JSONWriter::close_file() {
  if (file_) {
    flush();
    file_.close();
  }
}

void JSONWriter::open_group(const std::string& name) {
  auto new_group = open_groups_.top()->addGroup(name);
  if (!new_group)
    throw(std::logic_error("Could not open group " + name));
  open_groups_.push(new_group);
}

void JSONWriter::close_group() {
  if (open_groups_.size() == 1) {
    throw(std::logic_error("Can't close root group."));
  }

  open_groups_.pop();
}

void JSONWriter::flush() {
  file_.seekp(0);
  root_.write(file_, 1);
  file_ << "\n";
}

}  // namespace json
