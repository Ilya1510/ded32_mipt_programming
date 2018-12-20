#include <iostream>
#include <fstream>
#include "operation.h"
#include "parser.h"

int main(int argc, char** argv) {
    std::ifstream in(argv[1]);
    std::string input_expression;
    std::getline(in, input_expression);

    Parser parser;
    auto operation = parser.parse(input_expression);
    auto derivative = operation->derivative("x");

    std::ofstream out("output.txt");
    out << derivative->to_string() << std::endl;
    return 0;
}