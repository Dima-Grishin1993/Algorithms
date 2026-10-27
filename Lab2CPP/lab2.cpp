#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "stack.h"

using namespace std;

// Тег значения в стеке: обычное число или адрес возврата
enum Tag
{
    TAG_NUMBER,
    TAG_ADDRESS
};

struct Instruction
{
    string name;
    Data arg;
};

struct Machine
{
    Stack *values;
    Stack *tags; // тег для каждого значения из values
    Data vars[4];
};

// Защита от бесконечного цикла в скрипте
const size_t MAX_STEPS = 1000000;

static void push(Machine &m, Data value, Data tag)
{
    stack_push(m.values, value);
    stack_push(m.tags, tag);
}

static bool pop(Machine &m, Data &value, Data &tag)
{
    if (stack_empty(m.values))
        return false;
    value = stack_get(m.values);
    tag = stack_get(m.tags);
    stack_pop(m.values);
    stack_pop(m.tags);
    return true;
}

static bool is_math_op(const string &name)
{
    return name == "imul" || name == "iand" || name == "ior"
        || name == "ixor" || name == "iadd" || name == "isub";
}

// a — нижний операнд, b — верхний
static Data compute(const string &name, Data a, Data b)
{
    if (name == "imul") return a * b;
    if (name == "iand") return a & b;
    if (name == "ior")  return a | b;
    if (name == "ixor") return a ^ b;
    if (name == "iadd") return a + b;
    return a - b;
}

static bool is_var_op(const string &name, const string &prefix)
{
    return name.size() == prefix.size() + 1
        && name.compare(0, prefix.size(), prefix) == 0
        && name.back() >= '0' && name.back() <= '3';
}

static string parse(istream &in, vector<Instruction> &program)
{
    string line;
    while (getline(in, line))
    {
        istringstream ls(line);
        Instruction ins;
        ins.arg = 0;
        if (!(ls >> ins.name))
            continue;

        bool needs_arg = ins.name == "bipush" || ins.name == "invokestatic";
        bool known = needs_arg || is_math_op(ins.name)
            || ins.name == "pop" || ins.name == "swap" || ins.name == "return"
            || is_var_op(ins.name, "iload_") || is_var_op(ins.name, "istore_");
        if (!known)
            return "unknown command '" + ins.name + "'";
        if (needs_arg && !(ls >> ins.arg))
            return "missing argument for " + ins.name;

        string extra;
        if (ls >> extra)
            return "unexpected '" + extra + "' after " + ins.name;

        program.push_back(ins);
    }
    return "";
}

static string run(Machine &m, const vector<Instruction> &program)
{
    size_t pc = 0;
    size_t steps = 0;
    while (pc < program.size())
    {
        if (++steps > MAX_STEPS)
            return "too many steps (infinite loop?)";

        const Instruction &ins = program[pc++];
        const string &op = ins.name;
        Data a = 0, b = 0, ta = 0, tb = 0;

        if (op == "bipush")
            push(m, ins.arg, TAG_NUMBER);
        else if (op == "pop")
        {
            if (!pop(m, a, ta))
                return "pop from empty stack";
            if (ta == TAG_ADDRESS)
                return "pop of return address";
        }
        else if (is_math_op(op))
        {
            if (!pop(m, b, tb) || !pop(m, a, ta))
                return "not enough operands for " + op;
            if (ta == TAG_ADDRESS || tb == TAG_ADDRESS)
                return op + " with return address";
            push(m, compute(op, a, b), TAG_NUMBER);
        }
        else if (is_var_op(op, "iload_"))
            push(m, m.vars[op.back() - '0'], TAG_NUMBER);
        else if (is_var_op(op, "istore_"))
        {
            if (!pop(m, a, ta))
                return op + " from empty stack";
            if (ta == TAG_ADDRESS)
                return op + " of return address";
            m.vars[op.back() - '0'] = a;
        }
        else if (op == "swap")
        {
            if (!pop(m, b, tb) || !pop(m, a, ta))
                return "not enough values for swap";
            push(m, b, tb);
            push(m, a, ta);
        }
        else if (op == "invokestatic")
        {
            if (ins.arg < 0 || (size_t)ins.arg >= program.size())
                return "invalid function address " + to_string(ins.arg);
            cout << "call " << ins.arg << "\n";
            push(m, (Data)pc, TAG_ADDRESS);
            pc = (size_t)ins.arg;
        }
        else if (op == "return")
        {
            if (!pop(m, a, ta))
                return "return with empty stack";
            if (ta != TAG_ADDRESS)
                return "return with a number instead of return address";
            pc = (size_t)a;
        }
    }
    return "";
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        cout << "usage: " << argv[0] << " <input file>\n";
        return 1;
    }
    ifstream in(argv[1]);
    if (!in)
    {
        cout << "error: cannot open " << argv[1] << "\n";
        return 1;
    }

    Machine m;
    m.values = stack_create();
    m.tags = stack_create();
    for (Data &v : m.vars)
        v = 0;

    vector<Instruction> program;
    string error = parse(in, program);
    if (error.empty())
        error = run(m, program);

    if (!error.empty())
        cout << "error: " << error << "\n";
    else
    {
        cout << "stack:\n";
        while (!stack_empty(m.values))
        {
            cout << stack_get(m.values) << "\n";
            stack_pop(m.values);
        }
        cout << "vars:\n";
        for (Data v : m.vars)
            cout << v << "\n";
    }

    stack_delete(m.values);
    stack_delete(m.tags);
    return 0;
}