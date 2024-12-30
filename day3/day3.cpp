#include <iostream>
#include <common/common.h>
#include "day3.h"



int64_t SolutionDay3::prob1(std::string inputFile)
{
    std::ifstream file(inputFile);
    auto content = readFile(file);
    auto allContent = std::views::join(content) | std::ranges::to<std::string>();
    
    std::regex multRegex("mul\\((\\d{1,3}),(\\d{1,3})\\)");
    auto matches = std::sregex_iterator(allContent.begin(), allContent.end(), multRegex);
    
    int64_t tot = 0;
    for (auto it = matches; it != std::sregex_iterator(); ++it)
    {
        tot += std::stoll((*it)[1])* std::stoll((*it)[2]);
    }

    return tot;
}


int64_t SolutionDay3::prob2(std::string inputFile)
{
    std::ifstream file(inputFile);
    auto content = readFile(file);
    auto allContent = std::views::join(content) | std::ranges::to<std::string>();

    std::regex multRegex("(do\\(\\))|(don't\\(\\))|(mul\\((\\d{1,3}),(\\d{1,3})\\))");
    auto matches = std::sregex_iterator(allContent.begin(), allContent.end(), multRegex);

    int64_t tot = 0;
    bool enabled = true;
    for (auto it = matches; it != std::sregex_iterator(); ++it)
    {
        if (enabled && (*it)[3].matched)
            tot += std::stoll((*it)[4]) * std::stoll((*it)[5]);
        else if ((*it)[1].matched)
            enabled = true;
        else
            enabled = false;
    }

    return tot;
}

int main()
{
    std::string test = "test.txt";
    std::string input = "input.txt";

    std::unique_ptr<Solution> sol = std::make_unique<SolutionDay3>();

    std::cout << "Test P1:" << std::endl << sol->prob1(test) << std::endl;
    std::cout << "Input P1:" << std::endl << sol->prob1(input) << std::endl << std::endl;

    std::cout << "Test P2:" << std::endl << sol->prob2(test) << std::endl;
    std::cout << "Input P2:" << std::endl << sol->prob2(input) << std::endl;
}
