#pragma once
#include <string>
#include <vector>

// Character with an associated file position (may not correspond to physical
// position because of macros)
struct PosChar {
  char c = '\0';
  int line = 1;
  int column = 1;
  std::string file = "";

  // Convenience conversion operator
  operator char() const { return c; }
};

// Using a typedef prevents naming clashes in global name space
typedef PosChar PosChar;
typedef std::vector<PosChar> PosStr;
