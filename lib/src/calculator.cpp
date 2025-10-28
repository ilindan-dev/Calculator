#include "calculator/calculator.hpp"
#include "calculator/plugin.hpp"

#include <stdexcept>
#include <filesystem>
#include <iostream>
#include <map>
#include <vector>
#include <functional>
#include <cmath>

#include "tokenizer.hpp"

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

// Impl is the implementation class for the Calculator for implementation hiding (Pimpl idiom).
class Calculator::Impl final : public ICalculatorRegistrar {
public:

    // functions is the map of registered functions, the key is CanonicalName.
    // <CanonicalName, Function>
    std::map<std::string, std::function<double(const std::vector<double>&)>> functions;

    // numArgs is the map of number of arguments for each operation, the key is CanonicalName.
    // <CanonicalName, ArgCount>
    std::map<std::string, int> numArgs;

    // precedences is the map of precedences for each operation, the key is CanonicalName.
    // <CanonicalName, Precedence>
    std::map<std::string, int> precedences;

    // associativities is the map of associativities for each operation, the key is CanonicalName.
    // <CanonicalName, Associativity>
    std::map<std::string, Associativity> associativities;

    // --- Search map for parser ---

    // function_operations is the map of function operations, the key is Symbol, the value is CanonicalName.
    std::map<std::string, std::string> function_operations;

    // infix_operations is the map of infix operations, the key is Symbol, the value is CanonicalName.
    std::map<std::string, std::string> infix_operations;

    // prefix_operations is the map of prefix operations, the key is Symbol, the value is CanonicalName.
    std::map<std::string, std::string> prefix_operations;

    // postfix_operations is the map of postfix operations, the key is Symbol, the value is CanonicalName.
    std::map<std::string, std::string> postfix_operations;

    // --- Dynamic Tokenizer ---
    Tokenizer tokenizer;

    // --- Plugin handles ---
#ifdef _WIN32
    std::vector<HMODULE> pluginHandles;
#else
    std::vector<void*> pluginHandles;
#endif

    // Constructor
    Impl() {
        Impl::registerOperation({
            "add",
            [](const auto& args) { return args[0] + args[1]; }, 2,
            OperationType::Infix, "+", 2, Associativity::Left
        });
        Impl::registerOperation({
            "sub",
            [](const auto& args) { return args[0] - args[1]; }, 2,
            OperationType::Infix, "-", 2, Associativity::Left
        });
        Impl::registerOperation({
            "mul",
            [](const auto& args) { return args[0] * args[1]; }, 2,
            OperationType::Infix, "*", 3, Associativity::Left
        });
        Impl::registerOperation({
            "div",
            [](const auto& args) {
                if (args[1] == 0) throw std::invalid_argument("Divide by zero");
                return args[0] / args[1];
            }, 2,
            OperationType::Infix, "/", 3, Associativity::Left
        });
        Impl::registerOperation({
            "pow",
            [](const auto& args) { return std::pow(args[0], args[1]); }, 2,
            OperationType::Infix, "^", 4, Associativity::Right
        });
        Impl::registerOperation({
            "negate",
            [](const auto& args) { return -args[0]; }, 1,
            OperationType::Prefix, "-", 5
        });
    }

    // Destructor
    ~Impl() override {
        for (auto handle : pluginHandles) {
#ifdef _WIN32
            FreeLibrary(handle);
#else
            dlclose(handle);
#endif
        }
    }

    // registerOperation registers a new operation in the calculator.
    void registerOperation(const OperationInfo& info) override {
        if (functions.contains(info.canonicalName)) {
            std::cerr << "Warning: Operation " << info.canonicalName << " already registered. Skipping." << std::endl;
            return;
        }

        functions[info.canonicalName] = info.function;
        numArgs[info.canonicalName] = info.numArguments;

        switch (info.type) {
            case OperationType::Function:
                function_operations[info.symbol] = info.canonicalName;
                tokenizer.addSymbol(info.symbol);
                break;
            case OperationType::Infix:
                infix_operations[info.symbol] = info.canonicalName;
                precedences[info.canonicalName] = info.precedence;
                associativities[info.canonicalName] = info.associativity;
                tokenizer.addSymbol(info.symbol);
                break;
            case OperationType::Prefix:
                prefix_operations[info.symbol] = info.canonicalName;
                precedences[info.canonicalName] = info.precedence;
                tokenizer.addSymbol(info.symbol);
                break;
            case OperationType::Postfix:
                postfix_operations[info.symbol] = info.canonicalName;
                precedences[info.canonicalName] = info.precedence;
                tokenizer.addSymbol(info.symbol);
                break;
        }
    }

    // loadPlugin loads a single plugin from the specified path.
    void loadPlugin(const std::filesystem::path& pluginPath) {
        using RegisterPluginFunc = void(*)(ICalculatorRegistrar*);

        try {
#ifdef _WIN32
            HMODULE handle = LoadLibraryA(pluginPath.string().c_str());
            if (!handle) {
                throw std::runtime_error("Failed to load plugin: " + pluginPath.string());
            }
            pluginHandles.push_back(handle);

            auto registerPlugin = reinterpret_cast<RegisterPluginFunc>(GetProcAddress(handle, "registerPlugin"));

#else
            void* handle = dlopen(pluginPath.string().c_str(), RTLD_NOW);
            if (!handle) {
                throw std::runtime_error("Failed to load plugin: " + pluginPath.string() + " Error: " + dlerror());
            }
            pluginHandles.push_back(handle);
            const auto registerPlugin = reinterpret_cast<RegisterPluginFunc>(dlsym(handle, "registerPlugin"));
#endif
            if (!registerPlugin) {
                throw std::runtime_error("Failed to find registerPlugin function in: " + pluginPath.string());
            }
            registerPlugin(this);

            std::cout << "Registered plugin: " << pluginPath.filename().string() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Failed to load plugin: " << pluginPath.string() << ": " << e.what() << std::endl;
        }
    }
};

// --- Calculator realize ---

// Constructor
Calculator::Calculator() : pImpl(std::make_unique<Impl>()) {}

// Destructor
Calculator::~Calculator() = default;

// registerOperation registers a new operation in the calculator.
void Calculator::registerOperation(const OperationInfo& operation) {
    pImpl->registerOperation(operation);
}

// loadPlugins loads plugins from the specified directory.
void Calculator::loadPlugins(const std::string& pluginDir) const {
    if (!std::filesystem::exists(pluginDir) || !std::filesystem::is_directory(pluginDir)) {
        std::cout << "Failed to load plugins: " << pluginDir << std::endl;
        return;
    }
    const std::string extension = PLUGIN_EXTENSION;
    for (const auto& entry : std::filesystem::directory_iterator(pluginDir)) {
        if (entry.is_regular_file() && entry.path().extension() == extension) {
            pImpl->loadPlugin(entry.path());
        }
    }
}

// evaluate evaluates the given mathematical expression and returns the result.
double Calculator::evaluate(const std::string& expression) {
    // TODO: Implement the expression evaluation logic using the registered operations.
    throw std::runtime_error("Not implemented: evaluate");
}