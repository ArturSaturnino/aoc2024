#include <iostream>
#include <common/common.h>
#include <common/cachedFunction.h>
#include <common/grid.h>





int64_t prob1(std::string inputFile)
{
    std::ifstream file(inputFile);
    auto content = readFile(file);
    Grid<int> map(content | std::views::transform([](const std::string_view& str) {
        return str | std::views::transform([](char c) {return static_cast<int>(c) - static_cast<int>('0'); });
        }));

    using pos_type = std::pair<int64_t, int64_t>;

    std::function<std::set<pos_type>(int64_t, int64_t)> reachablePeaks =
        [&reachablePeaks, &map](int64_t row, int64_t col)->std::set<pos_type>
        {

            std::set<pos_type> reachable;

            auto currElevation = map.get(row, col);
            if (currElevation == 9)
            {
                reachable.insert({ row, col });
                return reachable;
            }

            for (const auto& d : cardinal_dirs)
            {
                auto[mRow, mCol] = moveInDir(d, row, col);
                if (map.inBounds(mRow, mCol) && map.get(mRow, mCol) == currElevation + 1)
                    reachable.merge(reachablePeaks(mRow, mCol));
            }
            return reachable;
        };

    reachablePeaks = CachedFunction<std::set<pos_type>, int64_t, int64_t>(reachablePeaks);

    int64_t tot = 0;

    for (int64_t i = 0; i < map.nRows(); ++i)
    {
        for (int64_t j = 0; j < map.nCols(); ++j)
        {
            if (map.get(i, j) == 0)
                tot += std::size(reachablePeaks(i, j));

        }
    }

    return tot;
}


int64_t prob2(std::string inputFile)
{
    std::ifstream file(inputFile);
    auto content = readFile(file);

    Grid<int> map(content | std::views::transform([](const std::string_view& str) {
        return str | std::views::transform([](char c) {return static_cast<int>(c) - static_cast<int>('0'); });
        }));


    std::function<int64_t(int64_t, int64_t)> rating =
        [&rating, &map](int64_t row, int64_t col)->int64_t
        {
            auto currElevation = map.get(row, col);
            if (currElevation == 9)
            {
                return 1;
            }

            int64_t sum = 0;

            for (const auto& d : cardinal_dirs)
            {
                auto [mRow, mCol] = moveInDir(d, row, col);
                if (map.inBounds(mRow, mCol) && map.get(mRow, mCol) == currElevation + 1)
                    sum += rating(mRow, mCol);
            }
            return sum;
        };

    rating = CachedFunction<int64_t, int64_t, int64_t>(rating);

    int64_t tot = 0;

    for (int64_t i = 0; i < map.nRows(); ++i)
    {
        for (int64_t j = 0; j < map.nCols(); ++j)
        {
            if (map.get(i, j) == 0)
                tot += rating(i, j);

        }
    }

    return tot;
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
