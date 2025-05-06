#include <common/common.h>
#include <common/grid.h>

#include <queue>

Grid<int> getMemSpace(const std::vector<std::string>& input, int size, size_t nBytes)
{
    Grid<int> grid(size, size, false);
    for (const auto& p : std::views::take(input, nBytes))
    {
        auto pNums = readNums(p);
        grid.get(pNums[1], pNums[0]) = 1;
    }
    return grid;
}

int64_t getShortestPath(const Grid<int>& memSpace)
{
    
    std::map<std::pair<size_t, size_t>, int64_t> visited{};

    auto comp = [](std::tuple<size_t, size_t, int64_t> left, std::tuple<size_t, size_t, int64_t> right) {return std::get<2>(left) > std::get<2>(right); };
    std::priority_queue<std::tuple<size_t, size_t, int64_t>, std::vector<std::tuple<size_t, size_t, int64_t>>, decltype(comp)> queue;
    queue.emplace(0, 0, 0);

    while (queue.size() != 0)
    {
        auto [x,y,dis] = queue.top();
        queue.pop();

        if (visited.contains({ x,y }))
            continue;

        visited.insert({ { x,y }, dis });

        for (const auto& d : cardinal_dirs)
        {
            auto p_d = moveInDir(d, x, y);

            if (memSpace.inBounds(p_d) &&  memSpace.get(p_d) == 0)
            {
                queue.emplace(std::get<0>(p_d), std::get<1>(p_d), dis + 1);
            }
        }
    }
    return visited[{memSpace.nRows() - 1, memSpace.nCols() - 1}];
}

int64_t prob1(std::string inputFile)
{
    std::ifstream file(inputFile);
    auto content = readFile(file);
    auto memSpace = getMemSpace(content, 71, 1024);



    return getShortestPath(memSpace);
}


int64_t prob2(std::string inputFile)
{
    std::ifstream file(inputFile);
    auto content = readFile(file);
    return 0;
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
