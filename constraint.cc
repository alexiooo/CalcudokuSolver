#include <iostream>
#include "constraint.h"
#include "cspsolver.h"

void Constraint::addVariable(int v) {
    this->vars.push_back(v);
}

bool AlldiffConstraint::isSatisfied() {
    for (int i=0; i<vars.size(); i++) {
        auto v1 = csp.getVariable(vars[i]);
        if (v1->value >= csp.valueOffset) {
            for (int j = i + 1; j < vars.size(); j++) {
                auto v2 = csp.getVariable(vars[j]);
                if (v2->value >= csp.valueOffset && v2->value == v1->value) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool PlusConstraint::isSatisfied() {
    bool isComplete = true;
    int  value = 0;
    for (int i : vars) {
        auto v = csp.getVariable(i);
        if (v->value < csp.valueOffset) {
            isComplete = false;
        } else {
            value += v->value;
        }
    }
    return isComplete ? value == target : value < target;
}

bool MinusConstraint::isSatisfied() {
    int  value = 0;
    for (int i : vars) {
        auto v1 = csp.getVariable(i);
        if (v1->value >= csp.valueOffset) {
            value = v1->value;
            for (auto j : vars) {
                value -= csp.getVariable(j)->value;
            }
            if (value == target)
                return true;
        } else {
            return true;
        }
    }
    return false;
}

bool MultiplyConstraint::isSatisfied() {
    bool isComplete = true;
    int  value = 1;
    for (int i : vars) {
        auto v = csp.getVariable(i);
        if (v->value < csp.valueOffset)
            isComplete = false;
        else if (v->value == 0)
            return target == 0;
        else {
            value *= v->value;
            if (target % value != 0)
                return false;
        }
    }
    return isComplete ? value == target : value <= target;
}

bool DivideConstraint::isSatisfied() {
    bool hasZero = false;
    bool unsatisfiable;
    int  value;
    for (int i : vars) {
        auto v1 = csp.getVariable(i);
        if (v1->value < csp.valueOffset) {
            return true; // incomplete
        } else if (v1->value == 0) {
            if (target != 0 || hasZero)
                return false;
            hasZero = true;
        } else {
            value = v1->value;
            unsatisfiable = false;
            for (auto j : vars) {
                auto v2 = csp.getVariable(j);
                if (v2->value < csp.valueOffset)
                    return true;
                else if ( (target/value) % v2->value != 0) {
                    unsatisfiable = true;
                    break;
                } else
                    value /= v2->value;
            }
            if (value == target && !unsatisfiable)
                return true;
        }
    }
    return false;
}

bool ModuloConstraint::isSatisfied() {
    bool hasZero = false;
    int  value;
    for (int i : vars) {
        auto v1 = csp.getVariable(i);
        if (v1->value < csp.valueOffset) {
            return true; // incomplete
        } else if (v1->value == 0) {
            if (target != 0 || hasZero)
                return false;
            hasZero = true;
        } else {
            value = v1->value;
            for (auto j : vars) {
                auto v2 = csp.getVariable(j);
                if (v2->value < csp.valueOffset)
                    return true;
                value %= v2->value;
            }
            if (value == target)
                return true;
        }
    }
    return false;
}