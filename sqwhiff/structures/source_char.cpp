#include "sqwhiff/structures/source_char.hpp"

namespace sqwhiff {

SourceChar::operator char() { return character; }
bool SourceChar::operator==(char c) const { return character == c; }
void SourceChar::operator=(char c) { character = c; }

}  // namespace sqwhiff
