#include "json/details/util.hpp"

#include <istream>
#include <stdexcept>

namespace json::details {

std::string readQuotedString(std::istream& inp) {
  skipUntil(inp, '\"');
  std::string result;
  std::getline(inp, result, '\"');
  return result;
}

void skipUntil(std::istream& inp, char target) {
  char c;
  while (inp.read(&c, 1)) {
    if (c == target)
      return;
  }

  throw(std::logic_error("Parsing ended while looking for " + target));
}

void trimSpaces(std::istream& inp) {
  char c;
  while (inp.peek() == ' ' || inp.peek() == '\n') {
    inp.read(&c, 1);
  }
}

}  // namespace json::details
