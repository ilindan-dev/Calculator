#include <gtest/gtest.h>
#include "calculator/calculator.hpp"
#include <cmath>

#ifndef PLUGIN_DIR
#define PLUGIN_DIR "."
#endif

class PluginTest : public ::testing::Test {
protected:
    Calculator calc;

    void SetUp() override {
        calc.loadPlugins(PLUGIN_DIR);
    }
};

const double TOLERANCE = 1e-9;

TEST_F(PluginTest, SineFunction) {
    EXPECT_NEAR(calc.evaluate("sin(30)"), 0.5, TOLERANCE);

    EXPECT_NEAR(calc.evaluate("sin(90)"), 1.0, TOLERANCE);
}

TEST_F(PluginTest, CosineFunction) {
    EXPECT_NEAR(calc.evaluate("cos(60)"), 0.5, TOLERANCE);

    EXPECT_NEAR(calc.evaluate("cos(0)"), 1.0, TOLERANCE);
}

TEST_F(PluginTest, PowerFunctionRightAssociativity) {
    // 2^3^2 = 2^(3^2) = 2^9 = 512
    // If (2^3)^2 = 8^2 = 64 this is an error
    EXPECT_DOUBLE_EQ(calc.evaluate("2 ^ 3 ^ 2"), 512.0);
}

TEST_F(PluginTest, FactorialPostfix) {
    // 5! = 120
    EXPECT_DOUBLE_EQ(calc.evaluate("5!"), 120.0);
}

TEST_F(PluginTest, SubfactorialPrefix) {
    // !5 = 44
    // !4 = 9
    EXPECT_DOUBLE_EQ(calc.evaluate("!5"), 44.0);
    EXPECT_DOUBLE_EQ(calc.evaluate("!4"), 9.0);
}

TEST_F(PluginTest, FactorialAndSubfactorialConflict) {
    // !3 + 3!
    // !3 = 2
    // 3! = 6
    // Result = 8
    EXPECT_DOUBLE_EQ(calc.evaluate("!3 + 3!"), 8.0);
}

TEST_F(PluginTest, Logarithm) {
    // ln(e) = 1
    // exp(1) = e
    EXPECT_NEAR(calc.evaluate("ln(2.718281828)"), 1.0, 1e-5);
}

TEST_F(PluginTest, DegreesRadians) {
    // rad(180) = PI
    EXPECT_NEAR(calc.evaluate("rad(180)"), 3.1415926535, 1e-5);
    // deg(PI) = 180
    EXPECT_NEAR(calc.evaluate("deg(3.1415926535)"), 180.0, 1e-4);
}