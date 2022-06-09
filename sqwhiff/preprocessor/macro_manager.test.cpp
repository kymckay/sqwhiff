#include "sqwhiff/preprocessor/macro_manager.hpp"

#include <gtest/gtest.h>

#include <sstream>

using sqwhiff::MacroManager;
using sqwhiff::SourceChar;
using sqwhiff::SourceConsumer;
using sqwhiff::SourceString;

TEST(MacroManager, OutputsNormalWord) {
  // TODO: Somehow mock the output of consumer for unit testing
  std::stringstream source("allUnits");
  SourceConsumer consumer(source);
  consumer.advance();

  MacroManager manager;

  SourceString output = manager.processWord(consumer);

  EXPECT_EQ("allUnits", (std::string)output)
      << "allUnits hasn't been defined as a macro and should return unscathed";
}
