#pragma once

#include <cassert>
#include <string>
#include <sstream>
#include <vector>

namespace json {

template <class T>
std::istream& operator>>(std::istream& stream, std::vector<T>& vec);

template <class T>
T convert(const std::string& val) {
  T ret;
  std::stringstream stream(val);
  stream >> ret;
  return ret;
}

template <>
std::string inline convert(const std::string& val) {
  if (val.size() > 1 && (val[0] == '\"' && val.back() == '\"'))
    return val.substr(1, val.size() - 2);
  else
    throw(std::logic_error("Not a string"));
}

template <>
bool inline convert(const std::string& val) {
  if (val.size() >= 4 && val.substr(0, 4) == "true")
    return true;
  else if (val.size() >= 5 && val.substr(0, 5) == "false")
    return false;
  else
    return std::stoi(val);
}

template <class T>
std::ostream& operator<<(std::ostream& stream, const std::vector<T>& vec) {
  stream << "[";
  for (std::size_t i = 0; i < vec.size(); ++i) {
    if constexpr (std::is_same_v<T, std::string>) {
      stream << "\"" + vec[i] + "\"";
    }
    else {
      stream << vec[i];
    }
    if (i < vec.size() - 1)
      stream << ", ";
  }
  stream << "]";
  return stream;
}

template <class T>
std::istream& operator>>(std::istream& stream, std::vector<T>& vec) {
  vec.clear();

  char c;
  stream.read(&c, 1);
  if (c != '[') {
    stream.seekg(-1, stream.cur);
    throw(std::logic_error("invalid vector format"));
  }

  std::string value;
  bool quote = false;
  int parentheses = 0;

  while (stream.read(&c, 1)) {
    if (!quote) {
      switch (c) {
        case ']':
          vec.push_back(convert<T>(value));
          return stream;
        case '(':
          ++parentheses;
          value.push_back(c);
          break;
        case ')':
          --parentheses;
          value.push_back(c);
          break;
        case ',':
          if (parentheses == 0) {
            vec.push_back(convert<T>(value));
            value.clear();
          }
          else {
            value.push_back(c);
          }
          break;
        case '\"':
          quote = true;
          value.push_back(c);
          break;
        case ' ':
          break;
        default:
          value.push_back(c);
      }
    }
    else {
      if (c == '\"') {
        quote = false;
      }
      value.push_back(c);
    }
  }

  return stream;
}

template <class T>
std::istream& operator>>(std::istream& stream, std::vector<std::vector<T>>& vec) {
  vec.clear();

  char c;
  stream.read(&c, 1);
  if (c != '[') {
    stream.seekg(-1, stream.cur);
    throw(std::logic_error("invalid vector format"));
  }

  while (true) {
    vec.emplace_back();
    stream >> vec.back();

    // trim whitespaces.
    while (stream.peek() == ' ')
      stream.read(&c, 1);

    stream.read(&c, 1);
    if (c == ']')
      break;

    assert(c == ',');
    while (stream.peek() == ' ')
      stream.read(&c, 1);
  }

  return stream;
}

}  // namespace json
