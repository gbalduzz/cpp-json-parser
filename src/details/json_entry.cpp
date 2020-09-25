#include "json/details/json_entry.hpp"
#include "json/details/util.hpp"

namespace json::details {

bool JSONEntry::read(std::istream& inp) {
  int parentheses = 0;
  bool quote = false;
  char c;
  const auto pos = inp.tellg();

  while (inp.read(&c, 1)) {
    if (!quote) {
      switch (c) {
        case '(':
        case '[':
          ++parentheses;
          break;
        case ']':
        case ')':
          --parentheses;
          if (parentheses < 0)
            throw(std::logic_error(" Line " + findLine(inp) + ": imbalanced parenthesis."));
          break;
        case ',':
          if (parentheses == 0)
            return false;
          break;
        case '}':
          if (parentheses != 0) {
            throw(std::logic_error("Line " + findLine(inp) + ": imbalanced parenthesis."));
          }
          inp.seekg(-1, inp.cur);  // brace is part of parent group.
          return true;

        case '{':
        case ':':
          throw(std::logic_error("Line " + findLine(inp, pos) +
                                 ": missing \",\" or \"}\" after JSON entry."));

        case '\"':
          quote = true;
          break;

        // skip spaces and new lines.
        case '\n':
        case ' ':
          continue;
      }
    }
    else {
      if (c == '\"')
        quote = false;
    }

    data_.push_back(c);
  }

  throw(std::logic_error("Line " + findLine(inp, pos) + ": file ended while reading entry"));
}

}  // namespace json::details
