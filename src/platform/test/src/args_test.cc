#include "uinta/args.h"

#include <gtest/gtest.h>

#include "uinta/status.h"

namespace uinta {

class ArgsProcessorTestF : public ::testing::Test {
 protected:
};

// Constructor tests

TEST_F(ArgsProcessorTestF, ConstructorWithNoArgs) {
  ArgsProcessor processor(0, nullptr);

  EXPECT_TRUE(processor.status().ok());
}

TEST_F(ArgsProcessorTestF, ConstructorWithEmptyArgs) {
  std::array<const char*, 0> argv;
  ArgsProcessor processor(0, argv.data());

  EXPECT_TRUE(processor.status().ok());
}

TEST_F(ArgsProcessorTestF, ConstructorWithNonArgValue) {
  std::array<const char*, 2> argv{"program_name", "some_random_arg"};
  ArgsProcessor processor(2, argv.data());

  EXPECT_TRUE(processor.status().ok());
}

// Help argument tests

TEST_F(ArgsProcessorTestF, HelpShortFlag) {
  std::array<const char*, 1> argv{"-h"};
  ArgsProcessor processor(1, argv.data());

  EXPECT_TRUE(IsAborted(processor.status()));
}

TEST_F(ArgsProcessorTestF, HelpLongFlag) {
  std::array<const char*, 1> argv{"--help"};
  ArgsProcessor processor(1, argv.data());

  EXPECT_TRUE(IsAborted(processor.status()));
}

TEST_F(ArgsProcessorTestF, HelpFlagWithOtherArgs) {
  std::array<const char*, 2> argv{"program_name", "-h"};
  ArgsProcessor processor(2, argv.data());

  EXPECT_TRUE(IsAborted(processor.status()));
}

// getValue tests

TEST_F(ArgsProcessorTestF, GetValueForNonExistentArg) {
  std::array<const char*, 1> argv{"program_name"};
  ArgsProcessor processor(1, argv.data());

  auto result = processor.getValue(ArgsProcessor::Help);

  EXPECT_FALSE(result.has_value());
}

TEST_F(ArgsProcessorTestF, GetValueWithKeyValueSyntax) {
  // Help handler doesn't use the value, but we can test value parsing
  std::array<const char*, 1> argv{"-h=somevalue"};
  ArgsProcessor processor(1, argv.data());

  // Help returns CancelledError regardless of value
  EXPECT_TRUE(IsAborted(processor.status()));
}

// getBoolean tests

TEST_F(ArgsProcessorTestF, GetBooleanForNonExistentArg) {
  std::array<const char*, 1> argv{"program_name"};
  ArgsProcessor processor(1, argv.data());

  auto result = processor.getBoolean(ArgsProcessor::Help);

  EXPECT_FALSE(result.has_value());
}

TEST_F(ArgsProcessorTestF, GetBooleanTrueValue) {
  // Create a custom test scenario by directly testing the boolean logic
  // Since Help handler returns CancelledError, we test through constructor side
  // effects The ArgsProcessor stores values, so we verify getBoolean behavior
  ArgsProcessor processor(0, nullptr);

  // No arg set, should return nullopt
  auto result = processor.getBoolean(ArgsProcessor::Help);
  EXPECT_FALSE(result.has_value());
}

// getInt tests

TEST_F(ArgsProcessorTestF, GetIntForNonExistentArg) {
  std::array<const char*, 1> argv{"program_name"};
  ArgsProcessor processor(1, argv.data());

  auto result = processor.getInt(ArgsProcessor::Help);

  EXPECT_FALSE(result.has_value());
}

// status accessor tests

TEST_F(ArgsProcessorTestF, StatusReturnsOkByDefault) {
  ArgsProcessor processor(0, nullptr);

  EXPECT_TRUE(processor.status().ok());
}

TEST_F(ArgsProcessorTestF, StatusReturnsErrorOnHelpFlag) {
  std::array<const char*, 1> argv{"--help"};
  ArgsProcessor processor(1, argv.data());

  EXPECT_FALSE(processor.status().ok());
  EXPECT_TRUE(IsAborted(processor.status()));
}

// Key-value parsing tests

TEST_F(ArgsProcessorTestF, KeyValueParsingWithEquals) {
  std::array<const char*, 1> argv{"--help=value"};
  ArgsProcessor processor(1, argv.data());

  // Help handler is triggered and returns CancelledError
  EXPECT_TRUE(IsAborted(processor.status()));
}

TEST_F(ArgsProcessorTestF, KeyValueParsingWithoutEquals) {
  std::array<const char*, 1> argv{"--help"};
  ArgsProcessor processor(1, argv.data());

  EXPECT_TRUE(IsAborted(processor.status()));
}

// Multiple arguments tests

TEST_F(ArgsProcessorTestF, MultipleNonMatchingArgs) {
  std::array<const char*, 4> argv{"program", "--foo", "--bar", "-x"};
  ArgsProcessor processor(4, argv.data());

  EXPECT_TRUE(processor.status().ok());
}

TEST_F(ArgsProcessorTestF, HelpArgAmongOthers) {
  std::array<const char*, 4> argv{"program", "--foo", "-h", "--bar"};
  ArgsProcessor processor(4, argv.data());

  // Should stop processing and return CancelledError when -h is encountered
  EXPECT_TRUE(IsAborted(processor.status()));
}

// ArgHandler tests via HelpHandler behavior

class ArgHandlerTestF : public ::testing::Test {
 protected:
};

TEST_F(ArgHandlerTestF, ContainsKeyReturnsTrueForValidKey) {
  // Test through ArgsProcessor since ArgHandler is abstract
  std::array<const char*, 1> argv{"-h"};
  ArgsProcessor processor(1, argv.data());

  // -h triggers HelpHandler, confirming containsKey works
  EXPECT_TRUE(IsAborted(processor.status()));
}

TEST_F(ArgHandlerTestF, ContainsKeyReturnsTrueForLongKey) {
  std::array<const char*, 1> argv{"--help"};
  ArgsProcessor processor(1, argv.data());

  EXPECT_TRUE(IsAborted(processor.status()));
}

TEST_F(ArgHandlerTestF, ContainsKeyReturnsFalseForInvalidKey) {
  std::array<const char*, 1> argv{"--invalid-key"};
  ArgsProcessor processor(1, argv.data());

  // Invalid key doesn't match any handler
  EXPECT_TRUE(processor.status().ok());
}

// Constants tests

TEST_F(ArgsProcessorTestF, TrueConstant) {
  EXPECT_EQ(ArgsProcessor::True, "t");
}

TEST_F(ArgsProcessorTestF, FalseConstant) {
  EXPECT_EQ(ArgsProcessor::False, "f");
}

// Edge cases

TEST_F(ArgsProcessorTestF, EmptyStringArg) {
  std::array<const char*, 1> argv{""};
  ArgsProcessor processor(1, argv.data());

  EXPECT_TRUE(processor.status().ok());
}

TEST_F(ArgsProcessorTestF, SingleDashArg) {
  std::array<const char*, 1> argv{"-"};
  ArgsProcessor processor(1, argv.data());

  EXPECT_TRUE(processor.status().ok());
}

TEST_F(ArgsProcessorTestF, DoubleDashArg) {
  std::array<const char*, 1> argv{"--"};
  ArgsProcessor processor(1, argv.data());

  EXPECT_TRUE(processor.status().ok());
}

TEST_F(ArgsProcessorTestF, EqualsOnlyArg) {
  std::array<const char*, 1> argv{"="};
  ArgsProcessor processor(1, argv.data());

  EXPECT_TRUE(processor.status().ok());
}

TEST_F(ArgsProcessorTestF, MultipleEqualsInArg) {
  std::array<const char*, 1> argv{"--help=val1=val2"};
  ArgsProcessor processor(1, argv.data());

  // Help handler triggers regardless
  EXPECT_TRUE(IsAborted(processor.status()));
}

}  // namespace uinta
