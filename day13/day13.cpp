#include <iostream>
#include <functional>

#include <common/common.h>
#include <common/vec.h>


using IntVec = vec<int64_t, 2>;

struct ClawMachine
{
    IntVec offsetA;
    IntVec offsetB;
    IntVec target;

    int64_t costA = 3;
    int64_t costB = 1;


    ClawMachine(const std::ranges::range auto& inp)
    {
        auto it = inp.begin();
        offsetA = IntVec{ *it };
        offsetB = IntVec{ *(++it) };
        target = IntVec{ *(++it) };
    }

    ClawMachine& correctTarget()
    {
        target[0] += 10000000000000;
        target[1] += 10000000000000;
        return *this;
    }


};

struct ClawState
{
    IntVec pos;
    int64_t cost;

    bool operator<(const ClawState& other) const
    {
        if (cost < other.cost)
            return true;
        if (cost > other.cost)
            return false;
        return pos < other.pos;
    }
};



int64_t calcCost(const ClawMachine& machine)
{
    auto det = machine.offsetA[0] * machine.offsetB[1] - machine.offsetA[1] * machine.offsetB[0];

    auto numA = machine.offsetB[1] * machine.target[0] - machine.offsetB[0] * machine.target[1];
    auto numB = -machine.offsetA[1] * machine.target[0] + machine.offsetA[0] * machine.target[1];

    if (numA % det != 0 || numB % det != 0)
        return 0;
    
    return (numA / det) * machine.costA + (numB / det) * machine.costB;
}



int64_t prob1(std::string inputFile)
{
    std::ifstream file(inputFile);
    auto content = readFile(file);
    auto costs = std::views::transform(content, readNumsl)
        | std::views::chunk(4)
        | std::views::transform([](const auto& v) {return ClawMachine(v); })
        | std::views::transform(calcCost)
        ;

    return std::ranges::fold_left(costs, 0, std::plus{});
}


int64_t prob2(std::string inputFile)
{
    std::ifstream file(inputFile);
    auto content = readFile(file);
    auto costs = std::views::transform(content, readNumsl)
        | std::views::chunk(4)
        | std::views::transform([](const auto& v) {return ClawMachine(v).correctTarget(); })
        | std::views::transform(calcCost)
        ;

    return std::ranges::fold_left(costs, 0, std::plus{});
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
