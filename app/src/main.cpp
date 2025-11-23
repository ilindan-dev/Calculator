#include <iostream>
#include <string>
#include <filesystem>
#include <boost/program_options.hpp>
#include "calculator/calculator.hpp"

namespace po = boost::program_options;

int main(int argc, char* argv[]) {
    po::options_description desc("Calculator Options");
    desc.add_options()
        ("help,h", "Show this help message and exit.")
        ("expr,e", po::value<std::string>(), "The expression to evaluate.")
        ("plugin-dir,p", po::value<std::string>()->default_value("./plugins"), "The directory to load plugins from.");

    po::variables_map vm;
    try {
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);
    } catch (const std::exception& e) {
        std::cerr << "Argument parsing error: " << e.what() << std::endl;
        std::cout << desc << "\n";
        return 1;
    }

    if (vm.count("help")) {
        std::cout << "Usage: calculator_app -e \"expression\" [options]\n\n";
        std::cout << desc << "\n";
        return 0;
    }

    if (!vm.count("expr")) {
        std::cerr << "Error: The expression is required." << std::endl;
        std::cout << desc << "\n";
        return 1;
    }

    auto expression = vm["expr"].as<std::string>();
    auto pluginDir = vm["plugin-dir"].as<std::string>();

    Calculator calc;

    std::string appPath = argv[0];
    std::string appDir = std::filesystem::path(appPath).parent_path().string();
    std::string fullPluginDir = appDir + "/" + pluginDir;

    std::cout << "Loading plugins from: " << fullPluginDir << std::endl;
    calc.loadPlugins(fullPluginDir);
    std::cout << "Loading complete." << std::endl;
    std::cout << "---" << std::endl;

    try {
        double result = calc.evaluate(expression);
        std::cout << "Expression: " << expression << std::endl;
        std::cout << "Result: " << result << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Evaluation error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

