#include "sqwhiff/preprocessor/macro_manager.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using sqwhiff::MacroManager;
using sqwhiff::SourceChar;
using sqwhiff::SourceString;
using ::testing::NiceMock;
using ::testing::Return;

class MockSourceConsumer : public sqwhiff::source_consumer {
 public:
  MOCK_METHOD(SourceChar, advance, (), (override));
  MOCK_METHOD(SourceChar, peek, (), (override));
  MOCK_METHOD(SourceChar, at, (), (override));
};

TEST(MacroManager, OutputsNormalWord) {
  NiceMock<MockSourceConsumer> consumer;

  EXPECT_CALL(consumer, at()).Times(1).WillOnce(Return(SourceChar('a')));
  EXPECT_CALL(consumer, advance())
      .Times(8)
      .WillOnce(Return(SourceChar('l')))
      .WillOnce(Return(SourceChar('l')))
      .WillOnce(Return(SourceChar('U')))
      .WillOnce(Return(SourceChar('n')))
      .WillOnce(Return(SourceChar('i')))
      .WillOnce(Return(SourceChar('t')))
      .WillOnce(Return(SourceChar('s')))
      .WillOnce(Return(SourceChar('\0')));

  MacroManager manager;

  SourceString output = manager.processWord(consumer);

  EXPECT_EQ("allUnits", (std::string)output)
      << "allUnits hasn't been defined as a macro and should return unscathed";
}
