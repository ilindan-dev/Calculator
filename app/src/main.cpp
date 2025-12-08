#include <iostream>
#include <string>
#include <filesystem>
#include <cxxopts.hpp>
#include "calculator/calculator.hpp"

#ifdef _WIN32
#include <windows.h>
#endif

// getAppDir is a function for getting the directory where the executable file is located.
std::filesystem::path getAppDir(const char* argv0) {
    if (const std::filesystem::path path(argv0); path.has_parent_path()) {
        return std::filesystem::absolute(path).parent_path();
    }
    return std::filesystem::current_path();
}

int main(int argc, char* argv[]) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    try {
        cxxopts::Options options("calculator_app", "Calculator with Plugins");

        options.add_options()
            ("h,help", "Show help")
            ("e,expr", "Expression to evaluate", cxxopts::value<std::string>())
            ("p,plugin-dir", "Plugin directory", cxxopts::value<std::string>()->default_value("."));

        const auto result = options.parse(argc, argv);

        if (result.count("help")) {
            std::cout << options.help() << std::endl;
            return 0;
        }

        if (!result.count("expr")) {
            std::cerr << "Error: The expression is required (-e \"...\")" << std::endl;
            std::cout << options.help() << std::endl;
            return 1;
        }

        const std::string expression = result["expr"].as<std::string>();
        const std::string pluginDirName = result["plugin-dir"].as<std::string>();

        const std::filesystem::path appDir = getAppDir(argv[0]);
        std::filesystem::path fullPluginDir = appDir / pluginDirName;

        fullPluginDir = std::filesystem::weakly_canonical(fullPluginDir);

        const Calculator calc;

        std::cout << "Loading plugins from: " << fullPluginDir << std::endl;

        calc.loadPlugins(fullPluginDir.string());

        const double calcResult = calc.evaluate(expression);

        std::cout << "Expression: " << expression << std::endl;
        std::cout << "Result: " << calcResult << std::endl;

    } catch (const cxxopts::exceptions::exception& e) {
        std::cerr << "Argument parsing error: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}