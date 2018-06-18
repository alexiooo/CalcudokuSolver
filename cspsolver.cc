#include <memory>
#include <iostream>
#include "cspsolver.h"

using std::make_unique;
using std::make_shared;
using std::move;

/*
 *  GETTERS
 */

int CspSolver::indexOfVariable(const string &name) {
    for (int i = 0; i < vars.size(); i++)
        if (vars[i]->name == name)
            return i;
    return -1;
}

shared_ptr<Variable> &CspSolver::getVariable(const string &name) {
    return vars[indexOfVariable(name)];
}

shared_ptr<Variable> &CspSolver::getVariable(int index) {
    return vars[index];
}

/*
 *  BUILDING
 */

long CspSolver::addVariable(string name) {
    vars.emplace_back(make_shared<Variable>(move(name), values));
    return vars.size() - 1;
}

void CspSolver::addVariableToConstraint(int constraint, int var) {
    this->constraints[constraint]->addVariable(var);
}

long CspSolver::constraintFactory(string spec) {
    int target = std::stoi(spec);
    auto it = spec.begin();
    while (std::isspace(*it) || *it == '-' || (*it >= '0' && *it <= '9') ) {
        it++;
        if ( it == spec.end() )
            throw std::runtime_error("String ended before operator was found");
    }
    unique_ptr<Constraint> con;
    switch (*it) {
        case '+':   con = make_unique<PlusConstraint>(*this, target);
                    break;
        case '-':   con = make_unique<PlusConstraint>(*this, target);
                    break;
        case '*':   con = make_unique<MultiplyConstraint>(*this, target);
                    break;
        case '/':
        case ':':   con = make_unique<DivideConstraint>(*this, target);
                    break;
        case 'm':   con = make_unique<ModuloConstraint>(*this, target);
                    break;
        default:
            throw std::runtime_error("Unknown operator character");
    }
    this->constraints.emplace_back(move(con));
    return this->constraints.size() - 1;
}

void CspSolver::generateAlldiff(const string &format) {
    char buf[MAX_NAME_CHARS];
    for (int i = 0; i < values; i++) {
        auto colCon = make_unique<AlldiffConstraint>(*this);
        auto rowCon = make_unique<AlldiffConstraint>(*this);
        for (int j = 0; j < values; j++) {
            snprintf(buf, MAX_NAME_CHARS, format.c_str(), i, j);
            colCon->addVariable(indexOfVariable(string(buf)));
            snprintf(buf, MAX_NAME_CHARS, format.c_str(), j, i);
            rowCon->addVariable(indexOfVariable(string(buf)));
        }
        this->constraints.emplace_back(move(colCon));
        this->constraints.emplace_back(move(rowCon));
    }
}

void CspSolver::generateAlldiffBox(const string &format) {
    char buf[MAX_NAME_CHARS];
    for (int bi = 0; bi < values / 3; bi++) {
        for (int bj = 0; bj < values / 3; bj++) {
            auto boxCon = make_unique<AlldiffConstraint>(*this);
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    snprintf(buf, MAX_NAME_CHARS, format.c_str(), bi+i, bj+j);
                    boxCon->addVariable(indexOfVariable(string(buf)));
                }
            }
            this->constraints.emplace_back(move(boxCon));
        }
    }
}

/*
 * SOLVING
 */

bool CspSolver::isAdmissable() {
    for (auto &c : constraints)
        if (!c->isSatisfied())
            return false;
    return true;
}

void deepCopyVars(const VariableVector &src, VariableVector &dest, const int from = 0) {
    if (dest.size() < src.size())
        dest.resize(src.size());
    for (int i = from; i < src.size(); i++)
        dest[i] = make_shared<Variable>(src[i]);
}

void shallowCopyVars(const VariableVector &src, VariableVector &dest, const int from = 0) {
    for (int i = from; i < src.size(); i++)
        dest[i] = src[i];
}

using std::cout; using std::endl;

void CspSolver::print() {
    cout << "#####################" << endl << "\t";
    int cnt = 0;
    for (auto &v : vars) {
        if (v->value > 0)
            cout << v->value << "  ";
        else
            cout << ".  ";
        if (++cnt % values == 0)
            cout << endl << '\t';
    }
    cout << endl
         << "#####################"
         << endl;
}

int recursion = 0;

bool CspSolver::solve() {
    this->nodeConsistency();
    return this->backtrack(0);
}

bool CspSolver::backtrack(int offset) {
    if (!isAdmissable())
        return false;
    else if (offset >= vars.size())
        return true;

    auto &var = vars[offset];
    if (var->value <= 0) {
        VariableVector backup;
        deepCopyVars(vars, backup, offset + 1);

        for (int v = 0; v < values; v++) {
            if (var->valueSet[v]) {
                var->value = v + valueOffset;
                if (backtrack(offset + 1))
                    return true;
                shallowCopyVars(backup, vars, offset + 1);
                var = vars[offset];
            }
        }
        var->value = -1;
        return false;
    } else {
        return backtrack(offset + 1);
    }
}

void CspSolver::nodeConsistency() {
    for (auto it = constraints.begin(); it < constraints.end(); ) {
        if ((*it)->vars.size() == 1) {
            auto var = vars[(*it)->vars.front()];
            if (var->value >= valueOffset)
                return;
            for (int v = 0; v < values; v++) {
                if (var->valueSet[v]) {
                    var->value = v + valueOffset;
                    var->valueSet[v] = (*it)->isSatisfied();
                }
            }
            var->value = valueOffset - 1;
            it = constraints.erase(it);
        } else {
            ++it;
        }
    }
}

void CspSolver::arcConsistency() {
    for (auto & con : constraints) {
        for (int i : con->vars) {
            auto & vari = vars[i];
            if (vari->value >= valueOffset)
                continue;
            for (int vi = 0; vi < values; vi++) {
                if ( ! vari->valueSet[vi])
                    continue;
                vari->value = vi + valueOffset;
                if ( ! con->isSatisfied() ) {
                    vari->valueSet[vi] = false;
                    continue;
                }

                for (int j : con->vars) {
                    auto & varj = vars[j];
                    if (j == i || varj->value >= valueOffset)
                        continue;
                }
            }
            vari->value = valueOffset - 1;
        }
    }
}