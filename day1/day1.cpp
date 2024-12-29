#include <iostream>
#include <common/common.h>
#include "day1.h"



int64_t SolutionDay1::prob1(std::string inputFile)
{
    std::ifstream file(inputFile);
    auto content = readFile(file);
    auto [l1, l2] = parseInput(content);
    std::ranges::sort(l1);
    std::ranges::sort(l2);

    auto range = std::ranges::zip_view(l1, l2); 
    auto n =  std::ranges::fold_left(
            range,
            int64_t{ 0 },
            [](int64_t l, const auto& r)-> int64_t { return l + std::abs(std::get<0>(r) - std::get<1>(r)); }
        );


    return n;
}


int64_t SolutionDay1::prob2(std::string inputFile)
{
    std::ifstream file(inputFile);
    auto content = readFile(file);
    auto [l1, l2] = parseInput(content);
    std::unordered_map<int64_t, int64_t> rightCount{};
    for (auto n : l2) ++rightCount[n];
    auto n = std::ranges::fold_left(
        l1,
        int64_t{ 0 },
        [&rightCount](int64_t l, const auto& r)->int64_t { return l + rightCount[r] * r; }
    );

    return n;
}

std::pair<std::vector<int64_t>, std::vector<int64_t>> SolutionDay1::parseInput(const std::vector<std::string>& inp)
{
    std::vector<int64_t> l1(inp.size());
    std::vector<int64_t> l2(inp.size());
    int64_t i = 0;
    for (const auto& nums : std::ranges::transform_view(inp, [](const auto& l) {return readNumsl(l);}) )
    {
        l1[i] = nums[0];
        l2[i] = nums[1];
        ++i;
    }

    return std::make_pair(l1, l2);
}

int main()
{
    std::string test = "test.txt";
    std::string input = "input.txt";

    std::unique_ptr<Solution> sol = std::make_unique<SolutionDay1>();

    std::cout << "Test P1:" << std::endl << sol->prob1(test) << std::endl;
    std::cout << "Input P1:" << std::endl << sol->prob1(input) << std::endl << std::endl;

    std::cout << "Test P2:" << std::endl << sol->prob2(test) << std::endl;
    std::cout << "Input P2:" << std::endl << sol->prob2(input) << std::endl;
}
