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
  void addEntry(const std::string& name, const T& val) {
    auto [it, inserted] = objects_.insert({name, std::make_unique<JSONEntry>(val)});
    if (inserted) {
      order_.push_back(it);
    }
  }

  template <class T>
  void readEntry(const std::string& name, T& val) const {
    JSONEntry* entry = nullptr;
    auto it = objects_.find(name);
    if (it != objects_.end()) {
      entry = dynamic_cast<JSONEntry*>(it->second.get());
    }

    if (!entry)
      throw(std::logic_error("Entry " + name + " does not exist in group"));
    entry->write(val);
  }

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

}  // namespace json::details
