#pragma once

#include <array>
#include <cassert>
#include <string>
#include <sstream>
#include <vector>

#include "json/details/util.hpp"

namespace json {

template <class T>
struct Convert {
  static void execute(const std::string& inp, T& val) {
    std::stringstream stream(inp);
    stream >> val;
  }
};

template <>
struct Convert<std::string> {
  static void execute(std::string_view inp, std::string& val) {
    inp = details::trimSpaces(inp);
    if (inp.size() >= 2 && inp[0] == '\"' && inp.back() == '\"') {
      val = inp.substr(1, inp.size() - 2);
    }
    else {
      throw(std::logic_error("Not a string"));
    }
  }
};

template <>
struct Convert<bool> {
  static bool execute(std::string_view inp, bool& val) {
    inp = details::trimSpaces(inp);

    if (inp == "true")
      val = true;
    else if (inp == "false")
      val = false;
    else
      val = std::atoi(inp.data());

    return true;
  }
};

template <class T1, class T2>
struct Convert<std::pair<T1, T2>> {
  static void execute(const std::string& inp, std::pair<T1, T2>& p) {
    static_assert(std::is_scalar_v<T1> && std::is_scalar_v<T2>,
                  "composite pair members are not supported");

    const auto split = inp.find(',');
    if ((inp.at(0) != '(' && inp.back() != ')') || split == std::string::npos) {
      throw(std::logic_error("Not a pair"));
    }

    Convert<T1>::execute(inp.substr(1, split - 1), p.first);
    Convert<T2>::execute(inp.substr(split + 1, inp.size() - split - 2), p.second);
  }
};

template <class T, std::size_t n>
struct Convert<std::array<T, n>> {
  static void execute(const std::string& inp, std::array<T, n>& arr) {
    std::vector<T> v;
    v.reserve(n);

    // Expansive but easy: delegate to vector conversion.
    Convert<std::vector<T>>::execute(inp, v);
    if (v.size() != n)
      throw(std::logic_error("Array has wrong size " + std::to_string(v.size())));

    std::copy(v.begin(), v.end(), arr.begin());
  }
};

template <class T>
struct Convert<std::vector<T>> {
  static void execute(std::string_view inp, std::vector<T>& vec) {
    inp = details::trimSpaces(inp);

    if (inp.size() < 2 || inp[0] != '[' || inp.back() != ']') {
      throw(std::logic_error("Not a vector"));
    }

    std::vector<T> result;  // move result into vec after it has been successfully read.

    bool quote = false;
    int parentheses = 0;
    std::size_t pos = 1;
    std::string entry;

    auto add_element = [&]() {
      if (entry != "") {
        result.emplace_back();
        Convert<T>::execute(entry, result.back());
        entry.clear();
      }
    };

    while (pos < inp.size() - 1) {  // skip first and last square bracket.
      const char c = inp[pos++];

      if (!quote) {
        switch (c) {
          case '[':
          case '(':
            ++parentheses;
            entry.push_back(c);
            break;
          case ')':
          case ']':
            --parentheses;
            if (parentheses < 0) {  // imbalanced parentheses.
              throw(std::logic_error("Imbalanced parentheses."));
            }
            entry.push_back(c);
            break;
          case ',':
            if (parentheses == 0) {
              add_element();
            }
            else {
              entry.push_back(c);
            }
            break;
          case '\"':
            quote = true;
            entry.push_back(c);
            break;
          default:
            entry.push_back(c);
        }
      }
      else {
        if (c == '\"')
          quote = false;
        entry.push_back(c);
      }
    }

    // Process last element.
    add_element();

    vec = std::move(result);
  }
};

template <class T1, class T2>
std::ostream& operator<<(std::ostream& stream, const std::pair<T1, T2>& p) {
  return stream << '(' << p.first << ", " << p.second << ')';
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

template <class T, std::size_t n>
std::ostream& operator<<(std::ostream& stream, const std::array<T, n>& arr) {
  return stream << std::vector<T>(arr.begin(), arr.end());
}

}  // namespace json
