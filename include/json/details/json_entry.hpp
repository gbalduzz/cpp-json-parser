#pragma once

#include <cassert>
#include <sstream>
#include <string>
#include <vector>

#include "json/details/convert.hpp"
#include "json/details/json_object.hpp"

namespace json::details {

class JSONEntry : public JSONObject {
public:
  JSONEntry() = default;

  template <class T>
  JSONEntry(const T& val) {
    std::stringstream ss;
    ss << val;
    data_ = ss.str();
  }

  ~JSONEntry() = default;

  JSONEntry(const std::string& val) {
    data_ = '\"' + val + '\"';
  }
  JSONEntry(const char* val) {
    data_ = '\"' + std::string(val) + '\"';
  }

  void write(std::ostream& stream, int /*ident*/) const override {
    stream << data_;
  }

  template <class T>
  void write(T& obj) const {
    Convert<T>::execute(data_, obj);
  }

  // Return true if this is the last element of a group.
  bool read(std::istream& inp) override;

private:
  std::string data_;
};
}  // namespace json::details
