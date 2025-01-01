#include <iostream>
#include <common/common.h>


int64_t prob1(std::string inputFile)
{
    std::ifstream file(inputFile);
    auto content = readFile(file);

    auto relationsView = std::views::take_while(content, [](const auto& v) {return !v.empty(); });
    auto updatesView = std::views::drop_while(content, [](const auto& v) {return !v.empty(); }) | std::views::drop(1);

    std::unordered_map<int64_t, std::unordered_set<int64_t>> beforePages;

    for (const auto& r : relationsView)
    {
        auto nums = readNumsl(r);
        beforePages[nums[1]].insert(nums[0]);
    }

    int64_t count = 0;

    for (const auto& update : updatesView)
    {
        auto seq = readNumsl(update);
        bool isValid = true;
        int i = 0;
        while (i < seq.size() && isValid)
        {
            for (int j = 0; j < i; ++j)
            {
                if (beforePages[seq[j]].contains(seq[i]))
                {
                    isValid = false;
                    break;
                }
            }
            ++i;
        }

        count += isValid*seq[seq.size()/2];

    }
    return count;
}


int64_t prob2(std::string inputFile)
{
    std::ifstream file(inputFile);
    auto content = readFile(file);

    auto relationsView = std::views::take_while(content, [](const auto& v) {return !v.empty(); });
    auto updatesView = std::views::drop_while(content, [](const auto& v) {return !v.empty(); }) | std::views::drop(1);

    std::unordered_map<int64_t, std::unordered_set<int64_t>> beforePages;

    for (const auto& r : relationsView)
    {
        auto nums = readNumsl(r);
        beforePages[nums[1]].insert(nums[0]);
    }

    int64_t count = 0;

    for (const auto& update : updatesView)
    {
        auto seq = readNumsl(update);
        int i = 0;

        auto orgSeq = seq;
        std::sort(seq.begin(), seq.end(), [&beforePages](auto lhs, auto rhs) {return beforePages[rhs].contains(lhs); });

        if (!std::ranges::all_of(std::views::zip( orgSeq, seq), [](auto pair)->bool {return std::get<0>(pair) == std::get<1>(pair); } )  )
            count += seq[seq.size() / 2];

    }
    return count;
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
