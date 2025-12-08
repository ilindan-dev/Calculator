#include <gtest/gtest.h>
#include "calculator/calculator.hpp"
#include <string>
#include <cmath>

#ifndef PLUGIN_DIR
#define PLUGIN_DIR "."
#endif

class CalculatorTest : public ::testing::Test {
protected:
    Calculator calc;

    void SetUp() override {
        calc.loadPlugins(PLUGIN_DIR);
    }
};

TEST_F(CalculatorTest, SimpleAddition) {
    // 2 + 2 = 4
    EXPECT_DOUBLE_EQ(calc.evaluate("2 + 2"), 4.0);
}

TEST_F(CalculatorTest, SimpleSubtraction) {
    // 5 - 3 = 2
    EXPECT_DOUBLE_EQ(calc.evaluate("5 - 3"), 2.0);
}

TEST_F(CalculatorTest, SimpleMultiplication) {
    // 3 * 4 = 12
    EXPECT_DOUBLE_EQ(calc.evaluate("3 * 4"), 12.0);
}

TEST_F(CalculatorTest, SimpleDivision) {
    // 10 / 2 = 5
    EXPECT_DOUBLE_EQ(calc.evaluate("10 / 2"), 5.0);
}

TEST_F(CalculatorTest, OrderOfOperations) {
    // 2 + 2 * 2 = 6 (а не 8)
    EXPECT_DOUBLE_EQ(calc.evaluate("2 + 2 * 2"), 6.0);
}

TEST_F(CalculatorTest, Parentheses) {
    // (2 + 2) * 2 = 8
    EXPECT_DOUBLE_EQ(calc.evaluate("(2 + 2) * 2"), 8.0);
}

TEST_F(CalculatorTest, FloatingPoint) {
    // 2.5 + 2.5 = 5.0
    EXPECT_DOUBLE_EQ(calc.evaluate("2.5 + 2.5"), 5.0);
}

TEST_F(CalculatorTest, NegativeNumbers) {
    // -5 + 3 = -2 (Проверка унарного минуса из плагина sub)
    EXPECT_DOUBLE_EQ(calc.evaluate("-5 + 3"), -2.0);
}

TEST_F(CalculatorTest, ComplexExpression) {
    // (4 - 2)^3 + 1 = 2^3 + 1 = 9
    // Plugins sub, pow, add are required
    EXPECT_DOUBLE_EQ(calc.evaluate("(4 - 2) ^ 3 + 1"), 9.0);
}

TEST_F(CalculatorTest, DivisionByZero) {
    const double result = calc.evaluate("1 / 0");
    EXPECT_TRUE(std::isinf(result) || std::isnan(result));
}

TEST_F(CalculatorTest, UnknownOperation) {
    EXPECT_THROW((void)calc.evaluate("2 % 2"), std::runtime_error);
}