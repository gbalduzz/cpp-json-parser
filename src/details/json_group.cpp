#include "json/details/json_group.hpp"
#include "json/details/util.hpp"

namespace json::details {

JSONGroup* JSONGroup::addGroup(const std::string& name) {
  if (objects_.find(name) == objects_.end()) {
    objects_[name] = std::make_unique<JSONGroup>();
    return static_cast<JSONGroup*>(objects_[name].get());
  }
  else {
    return dynamic_cast<JSONGroup*>(objects_[name].get());
  }
}

JSONGroup* JSONGroup::getGroup(const std::string& name) {
  return dynamic_cast<JSONGroup*>(objects_[name].get());
}

void JSONGroup::write(std::ostream& stream, int ident) const {
  if (objects_.size() == 0) {
    stream << "{}";
    return;
  }

  stream << "{\n";

  auto last = --objects_.end();
  for (auto it = objects_.begin(); it != objects_.end(); ++it) {  // it -> [name, object]
    for (int i = 0; i < ident; ++i)
      stream << "  ";
    stream << "\"" + it->first + "\" : ";
    it->second->write(stream, ident + 1);

    if (it != last) {
      stream << ",\n";
      if (dynamic_cast<JSONGroup*>(it->second.get()))
        stream << "\n";
    }
  }

  stream << "\n";
  for (int i = 0; i < ident - 1; ++i)
    stream << "  ";
  stream << "}";
}

bool JSONGroup::read(std::istream& inp) {
  std::string name;
  char c;
  trimSpaces(inp);
  inp.read(&c, 1);
  if (c != '{')
    throw(std::logic_error("Line " + findLine(inp) + ": invalid group format."));
  trimSpaces(inp);

  bool is_last = false;

  if (inp.peek() == '}') {  // empty group
    is_last = true;
  }

  while (!is_last) {
    name = readQuotedString(inp);
    trimUntil(inp, ':');
    trimSpaces(inp);

    if (inp.peek() == '{') {  // is group
      objects_[name] = std::make_unique<JSONGroup>();
      is_last = objects_[name]->read(inp);
    }
    else {
      objects_[name] = std::make_unique<JSONEntry>();
      is_last = objects_[name]->read(inp);
    }
  }

  skipUntil(inp, '}');
  trimSpaces(inp);

  c = inp.peek();
  if (c == ',') {
    inp.read(&c, 1);
    return false;
  }
  else {
    return true;
  }
}

}  // namespace json::details
