#include <gtest/gtest.h>
#include "calculator/calculator.hpp"
#include <filesystem>
#include <cmath>
#include <iostream>

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

class PluginTest : public ::testing::Test {
protected:
    Calculator calc;
    void SetUp() override {
#ifdef PLUGIN_DIR
        const std::string pluginPath = PLUGIN_DIR;
        std::cout << "Attempting to load plugins from: " << pluginPath << std::endl;

        ASSERT_TRUE(std::filesystem::exists(pluginPath))
            << "Plugin directory does not exist: " << pluginPath;
        
        ASSERT_TRUE(std::filesystem::is_directory(pluginPath))
            << "Plugin path is not a directory: " << pluginPath;

        const std::string extension = PLUGIN_EXTENSION;
        bool foundPlugins = false;
        for (const auto& entry : std::filesystem::recursive_directory_iterator(pluginPath)) {
            if (entry.is_regular_file() && entry.path().extension() == extension) {
                foundPlugins = true;
                std::cout << "Found plugin: " << entry.path().filename().string() << std::endl;
            }
        }

        ASSERT_TRUE(foundPlugins)
            << "No plugins (.so/.dll) found in directory: " << pluginPath;

        calc.loadPlugins(pluginPath);
#else
        FAIL() << "PLUGIN_DIR macro is not defined. CMake configuration is incorrect.";
#endif
    }
};

TEST_F(PluginTest, FunctionPlugins) {
    EXPECT_NEAR(calc.evaluate("sin(0)"), 0.0, 1e-9);
    EXPECT_NEAR(calc.evaluate("sin(90)"), 1.0, 1e-9);
    EXPECT_NEAR(calc.evaluate("sin(30)"), 0.5, 1e-9);
    EXPECT_NEAR(calc.evaluate("sin(45) * sin(45)"), 0.5, 1e-9);
}

TEST_F(PluginTest, OperatorPlugins) {
    EXPECT_DOUBLE_EQ(calc.evaluate("4!"), 24.0);
    EXPECT_DOUBLE_EQ(calc.evaluate("3 + 4!"), 27.0);
    EXPECT_DOUBLE_EQ(calc.evaluate("(3 + 1)!"), 24.0);

    EXPECT_DOUBLE_EQ(calc.evaluate("!4"), 9.0);
    EXPECT_DOUBLE_EQ(calc.evaluate("!4 * 2"), 18.0);

    EXPECT_DOUBLE_EQ(calc.evaluate("!4!"), 362880.0);
}