#include "variable.h"

#include <iostream>

Variable::Variable(const Variable &var)
        : name(var.name), value(var.value), values(var.values) {
    for (int i=0; i<values; i++)
        valueSet[i] = var.valueSet[i];
}

Variable::Variable(const shared_ptr<Variable> var) : Variable(*var) {}

bool Variable::operator==(const Variable &v) {
    return v.name == name;
}