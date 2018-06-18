#ifndef CALCUDOKUSOLVER_VARIABLE_H
#define CALCUDOKUSOLVER_VARIABLE_H

#include <string>
#include <memory>
using std::string;
using std::shared_ptr;
const int MAX_VAL = 20;
const int MAX_NAME_CHARS = 10;

class Variable {
public:
    const string name;
    bool valueSet[MAX_VAL] = {true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true};
    int value;
    int values;

    explicit Variable(const string && name = "", int values = MAX_VAL, int val=-1)
            : name(name), values(values), value(val) {}
    Variable(const Variable & var);
    explicit Variable(shared_ptr<Variable> var);
    bool operator==(const Variable &v);
};

#endif //CALCUDOKUSOLVER_VARIABLE_H
