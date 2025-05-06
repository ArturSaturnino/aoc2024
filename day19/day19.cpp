#include <common/common.h>
#include <common/cachedFunction.h>


std::vector<std::string_view> getAvailableTowels(const std::string& towelList)
{
    std::vector<std::string_view> towels;
    auto posItr = towelList.cbegin();
    while (posItr != towelList.cend())
    {
        auto endItr = posItr;
        while (endItr != towelList.cend() && *endItr != ',')
            ++endItr;
        towels.emplace_back(posItr, endItr);
        posItr = endItr;
        if (endItr != towelList.cend())
        {
            std::advance(posItr, 2);
        }
    }
    return towels;
}


int64_t prob1(std::string inputFile)
{
    std::ifstream file(inputFile);
    auto content = readFile(file);
    auto towels = getAvailableTowels(content[0]);

     int64_t possibleCount = 0;
     for (const auto& target : std::views::drop(content, 2))
     {
         std::function<bool(size_t)> isDoable = [&isDoable, &target, &towels](size_t pos) -> bool
             {
                 if (pos == target.size())
                     return true;

                 for (const auto& t : towels)
                 {
                     bool startMatch = std::string_view(target.begin() + pos, target.end()).starts_with(t);
                     if (startMatch && isDoable(pos + t.size()))
                         return true;
                 }
                 return false;
             };
         
         isDoable = CachedFunction(isDoable);

         if (isDoable(0))
             ++possibleCount;
     }


    return possibleCount;
}

int64_t prob2(std::string inputFile)
{
    std::ifstream file(inputFile);
    auto content = readFile(file);
    auto towels = getAvailableTowels(content[0]);

    int64_t totCount = 0;
    for (const auto& target : std::views::drop(content, 2))
    {
        std::function<int64_t(size_t)> combCount = [&combCount, &target, &towels](size_t pos) -> int64_t
            {
                if (pos == target.size())
                    return 1;
                int64_t count = 0;
                for (const auto& t : towels)
                {
                    bool startMatch = std::string_view(target.begin() + pos, target.end()).starts_with(t);
                    if (startMatch)
                        count += combCount(pos + t.size());
                }
                return count;
            };

        combCount = CachedFunction(combCount);

        totCount += combCount(0);
    }


    return totCount;
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
