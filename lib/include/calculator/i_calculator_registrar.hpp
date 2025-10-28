#ifndef CALCULATOR_I_CALCULATOR_REGISTRAR_HPP
#define CALCULATOR_I_CALCULATOR_REGISTRAR_HPP

// Forward declaration of OperationInfo struct.
struct OperationInfo;

// ICalculatorRegistrar is an interface for registering operations in the calculator.
class ICalculatorRegistrar {
public:
    virtual ~ICalculatorRegistrar() = default;

    // registerOperation registers a new operation in the calculator.
    // Plugins call this method to register a single operation.
    virtual void registerOperation(const OperationInfo& operation) = 0;
};

#endif //CALCULATOR_I_CALCULATOR_REGISTRAR_HPP