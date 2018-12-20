#include <iostream>
#include "processor.h"

int main(int argc, char **argv) {
    std::string path = argv[1];
    std::ifstream my_file (path + "/source.txt");

    Assembler assembler;
    CompiledProgram cp = assembler.assemble(my_file);
    std::ofstream out_assemble(path + "/assemble.txt");
    assembler.writeByteCode(out_assemble, cp);

    std::ofstream out_dis(path + "/disassemble.txt");
    assembler.disassemble(out_dis, cp);

    std::ofstream output(path + "/out.txt");
    Processor processor(output);
    processor.execute(cp);
    return 0;
}