#ifndef CALCULATOR_CALCULATOR_HPP
#define CALCULATOR_CALCULATOR_HPP

#include "calculator/i_calculator_registrar.hpp"

#include <string>
#include <memory>

// Calculator class provides functionality to evaluate mathematical expressions.
class Calculator final : public ICalculatorRegistrar {
    public:
    // Constructor
    Calculator();
    // Destructor
    ~Calculator() override;

    // Copy Constructor: The calculator owns unique resources (plug-ins), copying is prohibited.
    Calculator(const Calculator&) = delete;

    // Copy Assignment.
    Calculator& operator=(const Calculator&) = delete;

    // Move Constructor.
    Calculator(Calculator&& other) noexcept;

    // Move Assignment.
    Calculator& operator=(Calculator&& other) noexcept;

    // loadPlugins loads plugins from the specified directory.
    void loadPlugins(const std::string& pluginDir) const;

    // evaluate evaluates the given mathematical expression and returns the result.
    [[nodiscard]] double evaluate(const std::string& expression) const;

    // registerOperation registers a new operation in the calculator.
    void registerOperation(const OperationInfo& operation) override;
private:
    // Impl is the implementation class for the Calculator for implementation hiding (Pimpl idiom).
    class Impl;
    // pImpl is the unique pointer to the implementation.
    std::unique_ptr<Impl> pImpl;
};

#endif //CALCULATOR_CALCULATOR_HPP