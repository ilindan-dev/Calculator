#include <gtest/gtest.h>
#include "calculator/calculator.hpp"
#include <stdexcept>

TEST(CalculatorTest, BasicArithmetic) {
    const Calculator calculator;
    EXPECT_DOUBLE_EQ(calculator.evaluate("2 + 2"), 4.0);
    EXPECT_DOUBLE_EQ(calculator.evaluate("5 - 3"), 2.0);
    EXPECT_DOUBLE_EQ(calculator.evaluate("3 * 4"), 12.0);
    EXPECT_DOUBLE_EQ(calculator.evaluate("10 / 2"), 5.0);
}

TEST(CalculatorTest, PrecedenceAndParentheses) {
    const Calculator calculator;
    EXPECT_DOUBLE_EQ(calculator.evaluate("2 + 3 * 4"), 14.0);
    EXPECT_DOUBLE_EQ(calculator.evaluate("(2 + 3) * 4"), 20.0);
}

TEST(CalculatorTest, UnaryMinus) {
    const Calculator calculator;
    EXPECT_DOUBLE_EQ(calculator.evaluate("-5"), -5.0);
    EXPECT_DOUBLE_EQ(calculator.evaluate("10 * -2"), -20.0);
    EXPECT_DOUBLE_EQ(calculator.evaluate("10 + -2"), 8.0);
    EXPECT_DOUBLE_EQ(calculator.evaluate("- ( 1 + 2 )"), -3.0);
}

TEST(CalculatorTest, Associativity) {
    const Calculator calculator;
    EXPECT_DOUBLE_EQ(calculator.evaluate("2 ^ 3 ^ 2"), 512.0);
    EXPECT_DOUBLE_EQ(calculator.evaluate("10 - 5 - 2"), 3.0);
}

TEST(CalculatorTest, Exceptions) {
    const Calculator calculator;
    EXPECT_THROW((void)calculator.evaluate("5 / 0"), std::runtime_error);
    EXPECT_THROW((void)calculator.evaluate("( 2 + 3"), std::runtime_error);
    EXPECT_THROW((void)calculator.evaluate("2 + 3 )"), std::runtime_error);
    EXPECT_THROW((void)calculator.evaluate("2 +"), std::runtime_error);
}