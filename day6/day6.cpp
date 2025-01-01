#include <iostream>
#include <common/common.h>
#include <common/grid.h>


std::pair<int64_t, int64_t> findInitialPos(const Grid<char>& map)
{
    for (int64_t i = 0; i < map.nRows(); ++i)
    {
        for (int64_t j = 0; j < map.nCols(); ++j)
        {
            if (map.get(i, j) == '^')
                return { i ,j };
        }
    }
    return{ -1,-1 };
}

std::unordered_set<int64_t> walkInLine(const Grid<char>& map, int64_t& pRow, int64_t& pCol, dir& d)
{
    std::unordered_set<int64_t> visited{ };
    
    auto line = map.getLine(pRow, pCol, d);
    auto lineItr = line.begin();
    auto lineEnd = line.end();
    while (lineItr != lineEnd && *lineItr != '#')
    {
        pRow = lineItr.getRow();
        pCol = lineItr.getCol();
        if (lineItr != lineEnd)
        {
            visited.insert(pRow + pCol * map.nRows());
            ++lineItr;
        }
    }

    if (lineItr == lineEnd)
    {
        pRow = lineItr.getRow();
        pCol = lineItr.getCol();
    }
    else
    {
        d = rotateRight(d);
    }   
    
    return visited;
}

bool loops(const Grid<char>& map, int64_t pRow, int64_t pCol, dir d)
{

    std::set<std::tuple<int64_t, int64_t, dir>> visited{ {pRow, pCol, d} };

    while (map.inBounds(pRow, pCol))
    {
        walkInLine(map, pRow, pCol, d);
        if (visited.contains({ pRow, pCol, d }))
            return true;
        visited.insert({ pRow, pCol, d });
    }
    return false;
}




int64_t prob1(std::string inputFile)
{
    std::ifstream file(inputFile);
    auto content = readFile(file);
    auto map = Grid<char>(content | std::views::transform([](auto& str) {return std::span(str); }));
    auto [pRow, pCol] = findInitialPos(map);
    dir d = dir::N;
   std::unordered_set<int64_t> visited{};
    
    while (map.inBounds(pRow, pCol))
    {
        visited.merge(walkInLine(map, pRow, pCol, d));
    }
    return visited.size();
}


int64_t prob2(std::string inputFile)
{
    std::ifstream file(inputFile);
    auto content = readFile(file);

    auto map = Grid<char>(content | std::views::transform([](auto& str) {return std::span(str); }) | std::ranges::to<std::vector>());
    auto [pRow, pCol] = findInitialPos(map);
    dir d = dir::N;
    std::unordered_set<int64_t> visited{};
    
    int64_t count = 0;

    for (int64_t i = 0; i < map.nRows(); ++i)
    {
        for (int64_t j = 0; j < map.nCols(); ++j)
        {
            if (map.get(i, j) != '^' && map.get(i, j) != '#')
            {
                map.get(i, j) = '#';
                if (loops(map, pRow, pCol, d))
                    ++count;
                map.get(i, j) = '.';
            }
        }
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
