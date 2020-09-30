#pragma once

#include <istream>
#include <string>

namespace json::details {

std::string readQuotedString(std::istream& inp);

void skipUntil(std::istream& inp, char target);
void trimUntil(std::istream& inp, char target);

void trimSpaces(std::istream& inp);
std::string_view trimSpaces(std::string_view s);

std::string findLine(std::istream& inp);
std::string findLine(std::istream& inp, const std::streampos& pos);

}  // namespace json::details
