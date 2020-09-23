#pragma once

#include <fstream>
#include <string>
#include <stack>

#include "json/details/json_group.hpp"

namespace json {

class JSONWriter {
public:
  JSONWriter();

  ~JSONWriter();

  void open_file(const std::string& filename);

  void close_file();

  void flush();

  void open_group(const std::string& name);

  void close_group();

  template <class T>
  void execute(const std::string& name, const T& obj);

private:
  std::stack<details::JSONGroup*> open_groups_;
  details::JSONGroup root_;

  std::ofstream file_;
};

template <class T>
void JSONWriter::execute(const std::string& name, const T& obj) {
  open_groups_.top()->addEntry(name, obj);
}

}  // namespace json
