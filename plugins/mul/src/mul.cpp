#include "calculator/plugin.hpp"
namespace {
    double mul_impl(const double* args, int argCount) {
        return args[0] * args[1];
    }
}

extern "C" PLUGIN_API void registerPlugin(ICalculatorRegistrar* registrar) {
    registrar->registerOperation(OperationInfo{
        "mul", 
        &mul_impl, 
        2, 
        OperationType::Infix, 
        "*", 
        3, 
        Associativity::Left
    });
}