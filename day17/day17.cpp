#include <common/common.h>
#include <stdexcept>

enum class instructions
{
    adv,
    bxl,
    bst,
    jnz,
    bxc,
    out,
    bdv,
    cdv
};

class CPU
{


private:
    int64_t regA;
    int64_t regB;
    int64_t regC;
    int64_t iPtr;
    std::vector<int64_t> out;

public:

    enum class instructions
    {
        adv,
        bxl,
        bst,
        jnz,
        bxc,
        out,
        bdv,
        cdv
    };

    static instructions getInstruction(int64_t code)
    {
        switch (code)
        {
        case 0: return instructions::adv;
        case 1: return instructions::bxl;
        case 2: return instructions::bst;
        case 3: return instructions::jnz;
        case 4: return instructions::bxc;
        case 5: return instructions::out;
        case 6: return instructions::bdv;
        case 7: return instructions::cdv;
        default:
            throw std::runtime_error("unrecognized instruction");
        }
    }

    const int64_t getOperand(int64_t code)
    {
        if (code < 4)
            return code;
        if (code == 4)
            return regA;
        if (code == 5)
            return regB;
        if (code == 6)
            return regC;

        throw std::runtime_error("unrecognized code");

    }

    void executeInstruction(instructions instruction, int64_t operand)
    {
        switch (instruction)
        {
        case CPU::instructions::adv:
            regA /= static_cast<int64_t>(1) << getOperand(operand);
            break;
        case CPU::instructions::bxl:
            regB ^= operand;
            break;
        case CPU::instructions::bst:
            regB = getOperand(operand) % 8;
            break;
        case CPU::instructions::jnz:
            if (regA != 0)
                iPtr = operand;
            break;
        case CPU::instructions::bxc:
            regB ^= regC;
            break;
        case CPU::instructions::out:
            out.push_back(getOperand(operand) % 8);
            break;
        case CPU::instructions::bdv:
            regB = regA / (static_cast<int64_t>(1) << getOperand(operand));
            break;
        case CPU::instructions::cdv:
            regC = regA / (static_cast<int64_t>(1) << getOperand(operand));
            break;
        default:
            break;
        }

        iPtr += (instruction == instructions::jnz && regA != 0) ? 0 : 2;


    }

    std::vector<int64_t> run(const std::vector<int64_t>& program)
    {
        while (iPtr < program.size() -1)
            executeInstruction(getInstruction(program[iPtr]), program[iPtr + 1]);
        return out;
    }
    
    CPU(int64_t regA, int64_t regB, int64_t regC):
        regA {regA},
        regB {regB},
        regC {regC},
        iPtr {0},
        out  {}
    { }
   


};



int64_t prob1(std::string inputFile)
{
    std::ifstream file(inputFile);
    auto content = readFile(file);

    CPU cpu(readNumsl(content[0])[0], readNumsl(content[1])[0], readNumsl(content[2])[0]);
    auto out = cpu.run(readNumsl(content[4]));

    for (const auto& o : out)
        std::cout << o << ",";
    
    std::cout << std::endl;

    return 0;
}


int64_t prob2(std::string inputFile)
{
    std::ifstream file(inputFile);
    auto content = readFile(file);
    return 0;
}

int main()
{
    std::string test = "test.txt";
    std::string input = "input.txt";

    std::cout << "Test P1:" << std::endl << prob1(test) << std::endl;
    std::cout << "Input P1:" << std::endl << prob1(input) << std::endl << std::endl;

    std::cout << "Test P2:" << std::endl << prob2(test) << std::endl;
    std::cout << "Input P2:" << std::endl << prob2(input) << std::endl;
}
