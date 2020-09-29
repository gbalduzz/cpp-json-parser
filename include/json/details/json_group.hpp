#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "json_entry.hpp"
#include "json_object.hpp"

namespace json::details {

class JSONGroup : public JSONObject {
public:
  JSONGroup() = default;
  ~JSONGroup() = default;

  JSONGroup* addGroup(const std::string& name);
  JSONGroup* getGroup(const std::string& name);

  template <class T>
  void addEntry(const std::string& name, const T& val);

  template <class T>
  void readEntry(const std::string& name, T& val) const;

  void write(std::ostream& stream, int ident) const override;
  bool read(std::istream& inp) override;

  void clear() {
    objects_.clear();
  }

private:
  using Container = std::unordered_map<std::string, std::unique_ptr<JSONObject>>;
  Container objects_;
  std::vector<Container::const_iterator> order_;
};

template <class T>
void JSONGroup::addEntry(const std::string& name, const T& val) {
  const auto it = objects_.find(name);

  if (it == objects_.end()) {
    auto [new_it, _] = objects_.insert({name, std::make_unique<JSONEntry>(val)});
    order_.push_back(new_it);
  }
  else {
    it->second = std::make_unique<JSONEntry>(val);
  }
}

template <class T>
void JSONGroup::readEntry(const std::string& name, T& val) const {
  JSONEntry* entry = nullptr;
  auto it = objects_.find(name);
  if (it != objects_.end()) {
    entry = dynamic_cast<JSONEntry*>(it->second.get());
  }

  if (!entry)
    throw(std::logic_error("Entry " + name + " does not exist in group"));
  entry->write(val);
}

}  // namespace json::details
