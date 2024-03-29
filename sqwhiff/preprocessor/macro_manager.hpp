#pragma once
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "sqwhiff/interfaces/abstract_consumer.hpp"
#include "sqwhiff/structures/macro_definition.hpp"

namespace sqwhiff {

using source_consumer = AbstractConsumer<SourceChar>;

// Regular map since macros cannot be overloaded
using macro_storage = std::unordered_map<std::string, MacroDefinition>;

// Macro definitions are shared across included files
using shared_macro_storage = std::shared_ptr<macro_storage>;

// A class for the sole purpose of managing and expanding macros recursively
class MacroManager {
  // Holds the current set of macro definitions
  shared_macro_storage defined_;

  // Helpers for recursively processing sections of source
  SourceString getWord(source_consumer&);
  std::vector<SourceString> getArguments(source_consumer&);

  // Helpers for recursively processing sections of extracted source
  SourceString getWord(std::vector<SourceChar>::const_iterator&,
                       const std::vector<SourceChar>::const_iterator&);
  std::vector<SourceString> getArguments(
      std::vector<SourceChar>::const_iterator&,
      const std::vector<SourceChar>::const_iterator&,
      const std::unordered_set<std::string>&);
  SourceString processWord(std::vector<SourceChar>::const_iterator&,
                           const std::vector<SourceChar>::const_iterator&,
                           const std::unordered_set<std::string>&);

  std::vector<SourceString> parameter_replacement(
      const MacroDefinition&, const std::vector<SourceString>&);

  // Recursively expands nested macros, retaining the origin of characters
  SourceString expand(const MacroDefinition&, const std::vector<SourceString>&,
                      const std::unordered_set<std::string>&);

 public:
  MacroManager();

  // Check if the given identifier is a defined macro
  bool isMacro(const std::string&);

  MacroDefinition getDefinition(const std::string&);

  // Remove a macro definition by identifier
  bool undefine(const std::string&);

  // Process a macro definition
  void define(const SourceString&);

  // Reads the next word from source and expands it if a macro is found
  SourceString processWord(source_consumer&);
};

}  // namespace sqwhiff
