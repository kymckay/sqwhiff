#pragma once

#include <algorithm>

#include "sqwhiff/errors/preprocessing_error.hpp"

#define CONCAT_INSERT -1

namespace sqwhiff {

template <class Consumer>
MacroManager<Consumer>::MacroManager() {
  defined_ = std::make_shared<macro_storage>();
}

template <class Consumer>
bool MacroManager<Consumer>::isMacro(const std::string& id) {
  return defined_->find(id) != defined_->end();
}

template <class Consumer>
MacroDefinition MacroManager<Consumer>::getDefinition(const std::string& id) {
  return defined_->at(id);
}

template <class Consumer>
bool MacroManager<Consumer>::undefine(const std::string& id) {
  return defined_->erase(id) > 0;
}

template <class Consumer>
void MacroManager<Consumer>::define(const SourceString& definition) {
  // Will consume the definition procedurally
  auto at = definition.chars.begin();
  auto limit = definition.chars.end();

  // Every macro has an identifier
  SourceString id;
  if (*at == '_' || std::isalpha(*at)) {
    id = getWord(at, limit);
  }

  if (id.chars.empty()) {
    throw PreprocessingError(
        *at, "Macro ID must start with an alpha character or _, found '" +
                 std::string(1, *at) + "'");
  }

  // Some macros are function-like (have parameters)
  std::vector<std::string> parameters;
  if (*at == '(') {
    at++;

    std::string param;
    while (at != limit && *at != ')') {
      if (*at == ',') {
        parameters.push_back(param);
        param.clear();
      } else if (!std::isspace(*at)) {
        // TODO: Test definition with two words seperated by space as parameter

        // Horizontal whitespace around parameters is ignored
        if (param.empty() && *at != '_' && !std::isalpha(*at)) {
          throw PreprocessingError(
              *at,
              "Macro parameter ID must start with an alpha "
              "character or _, found '" +
                  std::string(1, *at) + "'");
        }

        param.push_back(*at);
      }

      at++;
    }

    if (at == limit) {
      // TODO: Unclosed macro parameters
    }

    // Dangling comma is allowed so param could be empty and ignored
    if (!param.empty()) {
      parameters.push_back(param);
    }

    // Skip over the closing brace
    at++;
  }

  // Space characters (not whitespace) following the head are ignored
  while (at != limit && *at == ' ') {
    at++;
  }

  // The remaining line is the definition body
  // Split where parameters are found for easy insertion later
  std::vector<SourceString> body;
  std::vector<int> insertions;
  SourceString section;
  SourceString word;
  SourceChar just_stringized;
  just_stringized = '\0';
  while (at != limit) {
    // Words need to be collected to check for parameters
    if (*at == '_' || std::isalpha(*at)) {
      word = getWord(at, limit);
      continue;
    }

    // Once past a word, if it's a parameter the body is split around it
    if (!word.chars.empty()) {
      auto it =
          std::find(parameters.begin(), parameters.end(), (std::string)word);

      if (it != parameters.end()) {
        body.push_back(section);
        insertions.push_back(it - parameters.begin());

        section.chars.clear();
      } else {
        section.append(word);
      }

      if (just_stringized == '"') {
        section.chars.push_back(just_stringized);
        just_stringized = '\0';
      }

      word.chars.clear();
    }

    // Hash char is special operation within a macro definition
    if (*at == '#') {
      // When stringizing, new quotes should inherit position of #
      SourceChar hash = *at;
      at++;

      if (*at == '#') {
        // Also split body around concatenation for later application
        body.push_back(section);
        insertions.push_back(CONCAT_INSERT);

        section.chars.clear();
        at++;
      } else {
        just_stringized = hash;
        just_stringized = '"';
        section.chars.push_back(just_stringized);
        word = getWord(at, limit);
      }
      continue;
    }

    // Double quoted strings are not subject to parameter replacement
    // TODO: confirm
    bool in_string = *at == '"';

    // Other characters are added to the current section of the body
    section.chars.push_back(*at);
    at++;

    if (in_string) {
      while (at != limit && *at != '"') {
        section.chars.push_back(*at);
        at++;
      }

      if (at != limit) {
        section.chars.push_back(*at);
        at++;
      }
    }
  }

  // A parameter may be at the end of the body
  // TODO: Tidy up repeated logic
  if (!word.chars.empty()) {
    auto it =
        std::find(parameters.begin(), parameters.end(), (std::string)word);

    if (it != parameters.end()) {
      body.push_back(section);
      insertions.push_back(it - parameters.begin());
      section.chars.clear();
    } else {
      section.append(word);
    }

    if (just_stringized == '"') {
      section.chars.push_back(just_stringized);
    }
  }

  body.push_back(section);

  MacroDefinition result;
  result.body = std::move(body);
  result.insertions = std::move(insertions);
  (*defined_)[id] = std::move(result);
}

template <class Consumer>
std::vector<SourceString> MacroManager<Consumer>::parameter_replacement(
    const MacroDefinition& macro, const std::vector<SourceString>& arguments) {
  std::vector<SourceString> result;

  auto body_it = macro.body.begin();
  auto body_end = macro.body.end();
  auto replace_it = macro.insertions.begin();
  auto replace_end = macro.insertions.end();

  SourceString section;
  while (body_it != body_end) {
    section.append(*body_it);
    body_it++;

    if (replace_it != replace_end) {
      // Concatenation splits are preserved for future use
      if (*replace_it == CONCAT_INSERT) {
        result.push_back(section);
        section.chars.clear();
        replace_it++;
        continue;
      }

      section.append(arguments[*replace_it]);
      replace_it++;
    }
  }

  result.push_back(section);

  return result;
};

template <class Consumer>
SourceString MacroManager<Consumer>::expand(
    const MacroDefinition& macro, const std::vector<SourceString>& arguments,
    const std::unordered_set<std::string>& ignores) {
  SourceString expanded;

  // First perform parameter replacement
  std::vector<SourceString> replaced = parameter_replacement(macro, arguments);

  for (const auto& section : replaced) {
    // Next expand any nested macros (not present in ignore set)
    auto at = section.chars.begin();
    auto end = section.chars.end();

    while (at != end) {
      // Words need to be collected to check for macros
      if (*at == '_' || std::isalpha(*at)) {
        expanded.append(processWord(at, end, ignores));
      } else {
        expanded.chars.push_back(*at);
        at++;
      }
    }
  }

  return expanded;
}

template <class Consumer>
SourceString MacroManager<Consumer>::processWord(Consumer& source) {
  SourceString word = getWord(source);

  if (isMacro(word)) {
    std::vector<SourceString> arguments = getArguments(source);
    std::unordered_set<std::string> ignores = {word};
    return expand(getDefinition(word), arguments, ignores);
  }

  return word;
}

template <class Consumer>
SourceString MacroManager<Consumer>::processWord(
    std::vector<SourceChar>::const_iterator& at,
    const std::vector<SourceChar>::const_iterator& end,
    const std::unordered_set<std::string>& ignores) {
  SourceString word = getWord(at, end);

  if (isMacro(word) && ignores.find(word) == ignores.end()) {
    std::vector<SourceString> arguments = getArguments(at, end, ignores);
    std::unordered_set<std::string> new_ignores = ignores;
    new_ignores.insert(word);
    return expand(getDefinition(word), arguments, new_ignores);
  }

  return word;
}

template <class Consumer>
SourceString MacroManager<Consumer>::getWord(Consumer& source) {
  SourceString word;

  // Assume caller checked that the initial wasn't a digit
  for (SourceChar at = source.at(); std::isalnum(at) || at == '_';
       at = source.advance()) {
    word.chars.push_back(at);
  }

  return word;
}

template <class Consumer>
SourceString MacroManager<Consumer>::getWord(
    std::vector<SourceChar>::const_iterator& at,
    const std::vector<SourceChar>::const_iterator& end) {
  SourceString word;

  // Assume caller checked that the initial wasn't a digit
  while (at != end && (std::isalnum(*at) || *at == '_')) {
    word.chars.push_back(*at);
    at++;
  }

  return word;
}

template <class Consumer>
std::vector<SourceString> MacroManager<Consumer>::getArguments(
    Consumer& source) {
  std::vector<SourceString> arguments = {};

  SourceChar start = source.at();

  if (start == '(') {
    source.advance();

    SourceString argument;

    int open_p = 1;
    while (source.at() != '\0' && open_p != 0) {
      // Arguments are expanded before used in the macro
      if (std::isalpha(source.at()) || source.at() == '_') {
        SourceString expanded = processWord(source);
        argument.chars.insert(argument.chars.end(), expanded.chars.begin(),
                              expanded.chars.end());
        continue;
      }

      // Braces can be used inside arguments as long as the pairing is
      // balanced
      if (source.at() == '(') {
        open_p++;
      } else if (source.at() == ')') {
        open_p--;
      }

      // Commas don't split arguments within nested braces
      if (source.at() == ',' && open_p == 1) {
        arguments.push_back(argument);
        source.advance();

        // Reset for reuse
        argument.chars.clear();
      } else if (open_p != 0) {
        argument.chars.push_back(source.at());
        source.advance();
      }
    }

    if (open_p != 0) {
      // TODO: Give macro name in error
      throw PreprocessingError(start, "Unclosed macro arguments");
    } else {
      // Final argument ends on closing brace
      arguments.push_back(argument);
      source.advance();
    }
  }

  return arguments;
}

template <class Consumer>
std::vector<SourceString> MacroManager<Consumer>::getArguments(
    std::vector<SourceChar>::const_iterator& at,
    const std::vector<SourceChar>::const_iterator& end,
    const std::unordered_set<std::string>& ignores) {
  std::vector<SourceString> arguments;

  if (at == end) {
    return arguments;
  }

  SourceChar start = *at;

  if (start == '(') {
    at++;

    SourceString argument;

    int open_p = 1;
    while (at != end && open_p != 0) {
      // Arguments are expanded before used in the macro
      if (std::isalpha(*at) || *at == '_') {
        SourceString expanded = processWord(at, end, ignores);
        argument.chars.insert(argument.chars.end(), expanded.chars.begin(),
                              expanded.chars.end());
        continue;
      }

      // Braces can be used inside arguments as long as the pairing is
      // balanced
      if (*at == '(') {
        open_p++;
      } else if (*at == ')') {
        open_p--;
      }

      // Commas don't split arguments within nested braces
      if (*at == ',' && open_p == 1) {
        arguments.push_back(argument);
        at++;

        // Reset for reuse
        argument.chars.clear();
      } else if (open_p != 0) {
        argument.chars.push_back(*at);
        at++;
      }
    }

    if (open_p != 0) {
      // TODO: Give macro name in error
      throw PreprocessingError(start, "Unclosed macro arguments");
    } else {
      // Final argument ends on closing brace
      arguments.push_back(argument);
      at++;
    }
  }

  return arguments;
}

}  // namespace sqwhiff
