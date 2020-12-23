#include <cstdio>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>


enum InstructionType {
    NOP, ACC, JMP
};

struct Instruction {
    InstructionType type_;
    int value_;
};

Instruction parseLine(std::string const& s) {
    std::string instr = s.substr(0, s.find(" "));
    std::string valueStr = s.substr(s.find(" ")+1);
    int value = std::stod(valueStr);

    InstructionType t;
    if (instr == "nop") t = NOP;
    else if (instr == "acc") t = ACC;
    else if (instr == "jmp") t = JMP;
    else {
        fprintf(stderr, "Invalid instruction %s\n", instr.c_str());
        exit(1);
    }

    return { t, value };
}

bool doesTerminate(std::vector<Instruction> const& program) {
    int glob = 0;
    size_t curInstr = 0;
    std::vector<bool> hasRun (program.size(), false);

    while (!hasRun.at(curInstr)) {
        auto &instr = program.at(curInstr);
        hasRun.at(curInstr) = true;

        switch (instr.type_) {
            case NOP: curInstr += 1; break;
            case ACC: glob += instr.value_; curInstr += 1; break;
            case JMP: curInstr += instr.value_; break;
        }

        if (curInstr == program.size()) return true;

    }

    return false;
}

int main() {

    std::ifstream inFile("inputs/dec8.txt");
    std::string line;
    std::vector<Instruction> program;

    while (std::getline(inFile, line)) {
        program.push_back(parseLine(line));
    }

    int glob = 0;
    size_t curInstr = 0;
    std::vector<bool> hasRun (program.size(), false);
    while (!hasRun.at(curInstr)) {
        auto &instr = program.at(curInstr);
        hasRun.at(curInstr) = true;

        switch (instr.type_) {
            case NOP: curInstr += 1; break;
            case ACC: glob += instr.value_; curInstr += 1; break;
            case JMP: curInstr += instr.value_; break;
        }

    }
    printf("accumulator = %d\n", glob);


    for (size_t i = 0; i < program.size(); i++) {
        if (program.at(i).type_ == ACC) continue;

        program.at(i).type_ = (program.at(i).type_ == JMP) ? NOP : JMP;
        if (doesTerminate(program)) {
            break;
        } else {
            program.at(i).type_ = (program.at(i).type_ == JMP) ? NOP : JMP;
        }
    }

    glob = 0;
    curInstr = 0;
    std::fill(std::begin(hasRun), std::end(hasRun), false);
    while (!hasRun.at(curInstr)) {
        auto &instr = program.at(curInstr);
        hasRun.at(curInstr) = true;

        switch (instr.type_) {
            case NOP: curInstr += 1; break;
            case ACC: glob += instr.value_; curInstr += 1; break;
            case JMP: curInstr += instr.value_; break;
        }

        if (curInstr == program.size()) break;

    }
    printf("terminates with = %d\n", glob);


}