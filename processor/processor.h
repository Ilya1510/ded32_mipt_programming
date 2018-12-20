//
// Created by ilya on 14.12.18.
//

#ifndef PROCESSOR_PROCESSOR_H
#define PROCESSOR_PROCESSOR_H

#include <iostream>
#include "stack.cpp"
#include "assembler.h"
#include <cmath>

typedef long ul;

class Processor {
    Stack <double, std::ostream> stack;
    std::ofstream& out;

    double registers[4];
public:
    Processor(std::ofstream& output)
            : stack(std::cout, Level::L_NO), out(output) {

    }

    void execute(const CompiledProgram& cp) {
        for (ul i = 0; i < cp.commands.size(); ++i) {
            ul cmd = static_cast<ul>(cp.commands[i]);
            switch (cmd) {
                case CC_PUSH: {
                    double value = get_value(cp.commands[i+1]);
                    push(value); ++i;
                    break;
                }
                case CC_PRINT_VALUE: {
                    double value = get_value(cp.commands[i+1]);
                    out << value << std::endl; ++i;
                    break;
                }
                case CC_JMP: {
                    ul place = static_cast<ul>(cp.commands[i + 1]);
                    ++i;
                    jump_if(place, i, true);
                    break;
                }
                case CC_JNE:
                case CC_JE: {
                    ul place = static_cast<ul>(cp.commands[i + 1]);
                    double first = get_value(cp.commands[i + 2]);
                    double second = get_value(cp.commands[i + 3]);
                    i += 3;
                    bool is_need_jump = is_true_condition(cmd, first, second);
                    jump_if(place, i, is_need_jump);
                    break;
                }
                case CC_MOVE: {
                    ul reg = static_cast<ul>(cp.commands[i + 1]);
                    double value = get_value(cp.commands[i + 2]);
                    move(reg, value);
                    i += 2;
                    break;
                }
                case CC_END:
                    return;
                case CC_POP: {
                    ul reg = static_cast<ul>(cp.commands[i + 1]);
                    move(reg, stack.pop_back());
                    ++i;
                    break;
                }
                case CC_ADD: {
                    add();
                    break;
                }
                case CC_SUB: {
                    sub();
                    break;
                }
                case CC_MUL: {
                    mul();
                    break;
                }
                case CC_DIV: {
                    div();
                    break;
                }
                case CC_SIN: {
                    sin();
                    break;
                }
                default:
                    break;
            }
        }
    }

private:
    void push(double x) {
        stack.push_back(x);
    }

    void jump_if(ul place, ul& it, bool condition) const {
        if (condition) {
            it = place - 1;
        }
    }

    bool is_true_condition(ul cc, double first, double second) const {
        switch (cc) {
            case CC_JE:
                return first == second;
            case CC_JNE:
                return first != second;
            default:
                return true;
        }
    }

    void move(ul reg, double value) {
        registers[reg] = value;
    }

    double get_value(double raw_value) {
        if (get_from_registers.find(raw_value) == get_from_registers.end()) {
            return raw_value;
        }
        ul reg = get_from_registers.find(raw_value)->second;
        return registers[reg];
    }

    double pop() {
        return stack.pop_back();
    }

    void add() {
        stack.push_back(stack.pop_back() + stack.pop_back());
    }

    void sub() {
        double x = stack.pop_back();
        double y = stack.pop_back();
        stack.push_back(x - y);
    }

    void mul() {
        double x = stack.pop_back();
        double y = stack.pop_back();
        stack.push_back(x * y);
    }

    void div() {
        stack.push_back(stack.pop_back() / stack.pop_back());
    }

    void sin() {
        stack.push_back(std::sin(stack.pop_back()));
    }
};

#endif //PROCESSOR_PROCESSOR_H
