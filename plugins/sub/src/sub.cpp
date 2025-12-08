#include "calculator/plugin.hpp"
#include <cmath>

namespace {
    double sub_impl(const double* args, int argCount) {
        return args[0] - args[1];
    }
    
    double negate_impl(const double* args, int argCount) {
        return -args[0];
    }
}

extern "C" PLUGIN_API void registerPlugin(ICalculatorRegistrar* registrar) {
    registrar->registerOperation(OperationInfo{
        "sub", 
        &sub_impl, 
        2, 
        OperationType::Infix, 
        "-", 
        2, 
        Associativity::Left
    });

    registrar->registerOperation(OperationInfo{
        "negate", 
        &negate_impl, 
        1, 
        OperationType::Prefix, 
        "-", 
        5, 
        Associativity::Right
    });
}