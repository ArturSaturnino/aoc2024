#pragma once

#include <common/common.h>

class SolutionDay1 : public Solution
{
    int64_t prob1(std::string inputFile);
    int64_t prob2(std::string inputFile);
    std::pair<std::vector<int64_t>, std::vector<int64_t>> parseInput(const std::vector<std::string>& inp);

};


