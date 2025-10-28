#ifndef CALCULATOR_CALCULATOR_HPP
#define CALCULATOR_CALCULATOR_HPP

#include "calculator/i_calculator_registrar.hpp"

#include <string>
#include <vector>
#include <memory>

class Calculator : public ICalculatorRegistrar {
    public:
    Calculator();

    ~Calculator();

    void loadPlugins(const std::string& pluginDir);
    double evaluate(const std::string& expression);

    void registerOperation(const OperationInfo& operation) override;
private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

#endif //CALCULATOR_CALCULATOR_HPP