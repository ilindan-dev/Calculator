#include "calculator/plugin.hpp"
#include <cmath>

namespace {
    double pow_impl(const double* args, int argCount) {
        return std::pow(args[0], args[1]);
    }
}

extern "C" PLUGIN_API void registerPlugin(ICalculatorRegistrar* registrar) {
    registrar->registerOperation(OperationInfo{
        "pow", 
        &pow_impl, 
        2, 
        OperationType::Infix, 
        "^", 
        4, 
        Associativity::Right
    });
}