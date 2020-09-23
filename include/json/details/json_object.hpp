#pragma once

#include <ostream>

namespace json::details {

class JSONObject {
public:
  JSONObject() = default;
  virtual ~JSONObject() = default;

  virtual void write(std::ostream& stream, int ident) const = 0;
  virtual bool read(std::istream& stream) = 0;
};

}  // namespace json::details
