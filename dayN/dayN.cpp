#include <iostream>
#include <common/common.h>
#include "dayN.h"



int64_t SolutionDayN::prob1(std::string inputFile)
{
    std::ifstream file(inputFile);
    auto content = readFile(file);
    return 0;
}


int64_t SolutionDayN::prob2(std::string inputFile)
{
    std::ifstream file(inputFile);
    auto content = readFile(file);
    return 0;
}

int main()
{
    std::string test = "test.txt";
    std::string input = "input.txt";

    std::unique_ptr<Solution> sol = std::make_unique<SolutionDayN>();

    std::cout << "Test P1:" << std::endl << sol->prob1(test) << std::endl;
    std::cout << "Input P1:" << std::endl << sol->prob1(input) << std::endl << std::endl;

    std::cout << "Test P2:" << std::endl << sol->prob2(test) << std::endl;
    std::cout << "Input P2:" << std::endl << sol->prob2(input) << std::endl;
}
