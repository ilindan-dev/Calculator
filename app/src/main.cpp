#include <iostream>
#include <string>
#include <filesystem>
#include <cxxopts.hpp>
#include "calculator/calculator.hpp"

std::filesystem::path getAppDir(const char* argv0) {
    std::filesystem::path path(argv0);
    if (path.has_parent_path()) {
        return std::filesystem::absolute(path).parent_path();
    }
    return std::filesystem::current_path();
}

int main(int argc, char* argv[]) {
    try {
        // Настройка опций
        cxxopts::Options options("calculator_app", "C++17 Calculator with Plugins");

        options.add_options()
            ("h,help", "Show help")
            ("e,expr", "Expression to evaluate", cxxopts::value<std::string>())
            ("p,plugin-dir", "Plugin directory", cxxopts::value<std::string>()->default_value("plugins"));

        auto result = options.parse(argc, argv);

        // Обработка --help
        if (result.count("help")) {
            std::cout << options.help() << std::endl;
            return 0;
        }

        // Проверка обязательного аргумента
        if (!result.count("expr")) {
            std::cerr << "Error: The expression is required (-e \"...\")" << std::endl;
            std::cout << options.help() << std::endl;
            return 1;
        }

        std::string expression = result["expr"].as<std::string>();
        std::string pluginDirName = result["plugin-dir"].as<std::string>();

        // Логика путей
        std::filesystem::path appDir = getAppDir(argv[0]);
        std::filesystem::path fullPluginDir = appDir / pluginDirName;

        Calculator calc;

        std::cout << "Loading plugins from: " << fullPluginDir << std::endl;
        // Передаем строку, так как наш интерфейс ожидает string
        calc.loadPlugins(fullPluginDir.string());
        std::cout << "Loading complete." << std::endl;
        std::cout << "---" << std::endl;

        double calcResult = calc.evaluate(expression);
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