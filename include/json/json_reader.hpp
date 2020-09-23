#pragma once

#include <stack>
#include <string>

#include "json/details/json_group.hpp"

namespace json {

class JSONReader {
public:
  JSONReader();

  ~JSONReader() = default;

  void open_file(const std::string& filename);

  void close_file();

  void open_group(const std::string& name);

  void close_group();

  template <class T>
  void execute(const std::string& name, T& obj) const;

private:
  std::stack<details::JSONGroup*> open_groups_;
  details::JSONGroup root_;
};

template <class T>
void JSONReader::execute(const std::string& name, T& obj) const {
  open_groups_.top()->readEntry(name, obj);
}

}  // namespace json
