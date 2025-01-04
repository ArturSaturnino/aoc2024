#include <iostream>
#include <common/common.h>
#include <common/Grid.h>

using Region = std::set<std::pair<int64_t, int64_t>>;
using OrientedPair = std::tuple<dir, int64_t, int64_t>;



Region getContainingRegion(int64_t row, int64_t col, const Grid<char>& field)
{
    Region region;

    char symb = field.get(row, col);

    std::vector<std::pair<int64_t, int64_t>> toExplore{ {row, col} };
    while (!toExplore.empty())
    {
        auto [cRow, cCol] = toExplore.back();
        toExplore.pop_back();
        if (region.contains({ cRow, cCol }))
            continue;

        region.insert({ cRow, cCol });

        for (auto d : cardinal_dirs)
        {
            auto [dRow, dCol] = moveInDir(d, cRow, cCol);
            if (field.inBounds(dRow, dCol) && field.get(dRow, dCol) == symb && !region.contains({dRow, dCol}))
            {
                toExplore.push_back({ dRow, dCol });
            }
        }
    }

    return region;
}


std::vector<Region> getRegions(const Grid<char>& field)
{
    std::vector<Region> regions;

    for (int i = 0; i < field.nCols(); ++i)
    {
        for (int j = 0; j < field.nRows(); ++j)
        {
            auto isKnown = std::ranges::any_of(regions, [i, j](const auto& region) {return region.contains({ i,j }); });
            if (!isKnown)
            {
                regions.push_back(getContainingRegion(i, j, field));
            }
        }
    }

    return regions;
}


int64_t getArea(const Region& region)
{
    return std::size(region);
}

int64_t countOuterBound(int64_t row, int64_t col, const Region& region)
{
    int64_t perim = 0;
    for (auto d : cardinal_dirs)
    {
        auto [dRow, dCol] = moveInDir(d, row, col);
        if (!region.contains({ dRow, dCol }))
            ++perim;
    }
    return perim;
}

int64_t getPerimeter(const Region& region)
{

    int64_t perim = 0;
    for (const auto& p : region)
    {
        perim += countOuterBound(p.first, p.second, region);
    }

    return perim;
}

std::vector<OrientedPair> walkPerimeter(OrientedPair start, const Region& region, bool rotateClockwise)
{
    std::vector<OrientedPair> perim{ };

    auto loc = start;
    while (perim.empty() || loc != perim.front())
    {
        perim.push_back(loc);
        auto [rowAhead, colAhead] = moveInDir(std::get<0>(loc), std::get<1>(loc), std::get<2>(loc));
        if (region.contains({ rowAhead, colAhead }))
        {
            loc = { std::get<0>(loc), rowAhead, colAhead };
            auto turnDir = rotateClockwise ? rotateLeft(std::get<0>(loc)) : rotateRight(std::get<0>(loc));
            auto [rowTurn, colTurn] = moveInDir(turnDir , std::get<1>(loc), std::get<2>(loc));
            if (region.contains({ rowTurn, colTurn }))
                loc = { rotateLeft(std::get<0>(loc)), rowTurn, colTurn };
            continue;
        }

        auto turnDir = rotateClockwise ? rotateRight(std::get<0>(loc)) : rotateLeft(std::get<0>(loc));
        loc = { turnDir, std::get<1>(loc), std::get<2>(loc) };
    }
    return perim;
}


int64_t getSides(const Region& region)
{

    std::vector<std::vector<OrientedPair>> perims{ walkPerimeter(
        {dir::N, region.begin()->first, region.begin()->second} , region, true)
    };

    for (const auto& p : region)
    {
        if (region.contains(moveInDir(dir::E, p.first, p.second)))
            continue;

        if (std::ranges::any_of(perims, [&p, &region](const auto& perim) {
            return std::ranges::find(perim, OrientedPair({ dir::S, p.first, p.second })) != perim.end(); }))
            continue;

        perims.push_back(walkPerimeter({ dir::S, p.first, p.second }, region, true));
    }

    auto countSides = [](int64_t tot, const std::vector<OrientedPair>& perim)->int64_t
        {
            int64_t numSides = 0;
            for (int i = 0; i < perim.size(); ++i)
            {
                if (std::get<0>(perim[i]) != std::get<0>(perim[(i + 1) % perim.size()]))
                    ++numSides;
            }
            return tot + numSides;
        };

    return std::ranges::fold_left(perims, 0, countSides);
}




int64_t prob1(std::string inputFile)
{
    std::ifstream file(inputFile);
    auto content = readFile(file);
    Grid<char> field(content);

    auto regions = getRegions(field);

    return std::ranges::fold_left(regions, 0, 
        [](int64_t s, const auto& region) {return s + getArea(region) * getPerimeter(region); });
}


int64_t prob2(std::string inputFile)
{
    std::ifstream file(inputFile);
    auto content = readFile(file);
    Grid<char> field(content);

    auto regions = getRegions(field);

    return std::ranges::fold_left(regions, 0,
        [](int64_t s, const auto& region) {return s + getArea(region) * getSides(region); });
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
