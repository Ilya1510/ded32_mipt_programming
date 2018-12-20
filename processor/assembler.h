//
// Created by ilya on 14.12.18.
//

#ifndef PROCESSOR_ASSEMBLER_H
#define PROCESSOR_ASSEMBLER_H

#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <assert.h>
#include <sstream>

const double UNDEFINED_PLACE = 1e9;
typedef long ul;

enum CommandsCodes {
    CC_PUSH, CC_POP, CC_ADD, CC_SUB, CC_MUL,
    CC_DIV, CC_SIN, CC_LABEL, CC_END, CC_JMP,
    CC_JE, CC_MOVE, CC_RET, CC_BEGIN, CC_PRINT_VALUE,
    CC_JNE,
};

static const std::map<std::string, ul> map_commands = {
        {"push", CC_PUSH}, {"pop", CC_POP}, {"add", CC_ADD}, {"sub", CC_SUB},
        {"mul", CC_MUL}, {"div", CC_DIV}, {"sin", CC_SIN}, {"label", CC_LABEL},
        {"end", CC_END}, {"jmp", CC_JMP}, {"je", CC_JE}, {"move", CC_MOVE},
        {"ret", CC_RET}, {"begin", CC_BEGIN}, {"print_value", CC_PRINT_VALUE},
        {"jne", CC_JNE}
};

enum Registers {
    rax = 0, rbx = 1, rcx = 2, rdx = 3
};

static const std::map<std::string, ul> map_registers = {
        {"rax", rax}, {"rbx", rbx}, {"rcx", rcx}, {"rdx", rdx}
};

static const std::map<double, ul> get_from_registers = {
        {1e8, rax}, {1e8 + 1, rbx}, {1e8 + 2, rcx}, {1e8 + 3, rdx}
};
double register_to_value[] = {1e8, 1e8 + 1, 1e8 + 2, 1e8 + 3};

struct CompiledProgram {
    std::vector<double> commands;
    std::map<std::string, ul> labels;
    CompiledProgram(std::vector<double> new_commands,
        std::map<std::string, ul> new_labels) :
            commands(new_commands), labels(new_labels) {
    }
};

class Assembler {

    double read_value(std::ifstream& in) const {
        std::string input;
        in >> input;
        if (input[0] == '$') {
            assert(map_registers.find((input.substr(1))) != map_registers.end());
            ul reg = map_registers.find(input.substr(1))->second;
            return register_to_value[reg];
        }
        double value = static_cast<double>(std::stoi(input));
        assert(get_from_registers.find(value) == get_from_registers.end());
        return value;
    }

    std::string change_if_register(double value) const {
        static const auto invertRegister =
                getInvertMap<ul, std::string>(map_registers);
        if (get_from_registers.find(value) == get_from_registers.end()) {
            std::ostringstream strs;
            strs << value;
            return strs.str();
        }
        ul reg = get_from_registers.find(value)->second;

        return "$" + invertRegister.find(reg)->second;
    }

public:
    CompiledProgram assemble(std::ifstream& in) {
        std::vector<double> compiled_program;
        std::map<std::string, ul> labels;
        std::map<ul, std::string> unset_jumps;
        std::string label_name;
        std::string command;
        double value = 0;
        while (in >> command) {
            if (!isRegisteredCommand(command)) {
                continue;
            }
            if (command == "label") {
                in >> label_name;
                assert(labels.find(label_name) == labels.end());
                labels[label_name] = compiled_program.size();
                continue;
            }
            compiled_program.push_back(
                    static_cast<double>(map_commands.find(command)->second));
            if (command == "push" || command == "print_value") {
                value = read_value(in);
                compiled_program.push_back(value);
            } else if (command[0] == 'j') { // JUMP COMMAND
                in >> label_name;
                unset_jumps[compiled_program.size()] = label_name;
                compiled_program.push_back(UNDEFINED_PLACE);
                if (command != "jmp") {
                    double first = read_value(in);
                    double second = read_value(in);
                    compiled_program.push_back(first);
                    compiled_program.push_back(second);
                }
            } else if (command == "move") {
                std::string register_name;
                in >> register_name;
                value = read_value(in);
                assert(map_registers.find(register_name) != map_registers.end());
                compiled_program.push_back(
                        static_cast<double>(map_registers.find(register_name)->second));
                compiled_program.push_back(value);
            } else if (command == "pop") {
                std::string register_name;
                in >> register_name;
                assert(map_registers.find(register_name) != map_registers.end());
                compiled_program.push_back(
                        static_cast<double>(map_registers.find(register_name)->second));
            }
        }

        for (auto jump : unset_jumps) {
            compiled_program[jump.first] = labels[jump.second];
        }

        return CompiledProgram(compiled_program, labels);
    }

    template<class T, class U>
    std::map<T, U> getInvertMap(const std::map<U, T>& map_straight) const {
        std::map<T, U> invertMap;
        for (auto elem : map_straight) {
            invertMap[elem.second] = elem.first;
        }
        return invertMap;
    }

    void disassemble(std::ofstream& out, const CompiledProgram& cp) const {
        auto invertMap = getInvertMap<ul, std::string>(map_commands);
        auto invertLabels = getInvertMap<ul, std::string>(cp.labels);
        auto invertRegisters = getInvertMap<ul, std::string>(map_registers);
        auto jt = invertLabels.begin();
        for (ul i = 0; i < cp.commands.size(); ++i) {
            while (jt != invertLabels.end() && jt->first == i) {
                out << "label " << jt->second << std::endl;
                ++jt;
            }
            ul command_code = static_cast<ul>(cp.commands[i]);
            std::string command = invertMap[command_code];
            if (!isRegisteredCommand(command)) {
                continue;
            }
            out << command;
            if (command == "push" || command == "print_value") {
                out << " " << change_if_register(cp.commands[i + 1]);
                ++i;
            } else if (command[0] == 'j') { // JUMP COMMAND
                ul label_code = static_cast<ul>(cp.commands[i + 1]);
                assert(invertLabels.find(label_code) != invertLabels.end());
                out << " " << invertLabels[label_code];
                ++i;
                if (command != "jmp") { //JUMP IF
                    std::string first = change_if_register(cp.commands[i + 1]);
                    std::string second = change_if_register(cp.commands[i + 2]);
                    out << " " << first << " " << second;
                    i += 2;
                }
            } else if (command == "move") {
                ul register_code = static_cast<ul>(cp.commands[i + 1]);
                assert(invertRegisters.find(register_code) != invertRegisters.end());
                out << " " << invertRegisters[register_code] << " " <<
                    change_if_register(cp.commands[i + 2]);
                i += 2;
            } else if (command == "pop") {
                ul register_code = static_cast<ul>(cp.commands[i + 1]);
                assert(invertRegisters.find(register_code) != invertRegisters.end());
                out << " " << invertRegisters[register_code];
                ++i;
            }
            out << std::endl;
        }
    }

    void writeByteCode(std::ofstream& out, const CompiledProgram& cp) const {
        for (auto x : cp.commands) {
            out << x << " ";
        }
    }

    bool isRegisteredCommand(std::string cmd) const {
        return map_commands.find(cmd) != map_commands.end();
    }
};

#endif //PROCESSOR_ASSEMBLER_H
