#include <iostream>
#include <vector>
#include <fstream>
#include "stack.cpp"

void parse_args(int argc, char** argv, std::string& filename) {
    filename = "";
    if (argc > 1) {
        filename = argv[1];
    }
}

Level to_level(std::string s) {
    if (s == "INFO") {
        return L_INFO;
    } else if (s == "ALL") {
        return L_ALL;
    } else if (s == "NO") {
        return L_NO;
    } else if (s == "ERR") {
        return L_ERR;
    }
    return L_DEBUG;
}

template <class T, class... Args>
void execute(Stack<T, Args...>& stack) {
    std::string command;
    T value;
    while (true) {
        std::cin >> command;
        if (command == "push") {
            std::cin >> value;
            stack.push_back(value);
        } else if (command == "pop") {
            stack.pop_back();
        } else if (command == "size") {
            std::cout << stack.get_size() << std::endl;
        } else if (command == "end") {
            return;
        } else if (command == "crash0") {
            stack.arr_[stack.size_ - 1] = 228;
        }
    }
}

int main(int argc, char** argv) {
    std::cout << "Ввведите уровень подробности вывода (INFO, NO, ALL, ERR, DEBUG)\n";
    std::string debug_level = "";
    std::cin >> debug_level;
    std::string filename = "";
    parse_args(argc, argv, filename);

    if (filename != "") {
        std::ofstream log_stream;
        log_stream.open(filename);
        Stack<int, std::ofstream> stack(log_stream,
                                        to_level(debug_level));
        execute(stack);
    } else {
        Stack<int, std::ostream> stack(std::cout,
                                       to_level(debug_level));
        execute(stack);
    }
    return 0;
}