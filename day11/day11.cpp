#include <iostream>
#include <common/common.h>


void applyRules(int64_t rock, std::vector<int64_t>& out)
{
    if (rock == 0)
        out.push_back(1);
    else if (auto nDigits = countDigits(rock); nDigits%2 == 0)
    {
        int64_t order = std::pow<int64_t, int64_t>(10, nDigits/2);
        out.push_back(rock / order);
        out.push_back(rock % order);
    }
    else
        out.push_back(rock * 2024);

}


int64_t countRocks(const std::vector<int64_t>& startRocks, int numBlink)
{
    std::unordered_map<int64_t, int64_t> states;
    for (const auto& x : startRocks)
    {
        ++states[x];
    }

    for (int i = 0; i < numBlink; ++i)
    {
        std::unordered_map<int64_t, int64_t> newStates;
        for (const auto& [state, count] : states)
        {
            std::vector<int64_t> newState;
            newState.reserve(2);
            applyRules(state, newState);
            for (const auto& s : newState)
            {
                newStates[s] += count;
            }
        }
        states = std::move(newStates);
    }

    return std::ranges::fold_left(states, 0, [](int64_t tot, const auto& p) {return std::get<1>(p) + tot; });
}



int64_t prob1(std::string inputFile)
{
    std::ifstream file(inputFile);
    auto content = readFile(file);
    auto state = readNumsl(content[0]);

    return countRocks(readNumsl(content[0]), 25);
}


int64_t prob2(std::string inputFile)
{
    std::ifstream file(inputFile);
    auto content = readFile(file);
    std::unordered_map<int64_t, int64_t> states;

    return countRocks(readNumsl(content[0]), 75);
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
