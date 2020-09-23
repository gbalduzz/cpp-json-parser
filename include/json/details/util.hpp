#pragma once

#include <string>

namespace json::details {

std::string readQuotedString(std::istream& inp);

void skipUntil(std::istream& inp, char target);

void trimSpaces(std::istream& inp);

}
