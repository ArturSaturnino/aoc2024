#include <common/common.h>
#include <common/grid.h>
#include <common/vec.h>

#include <iostream>




auto getAntennas(const Grid<char>& map)
{
    std::unordered_map<char, std::vector<std::pair<int64_t, int64_t>>>
        antennas;

    for (int64_t i = 0; i < map.nRows(); ++i)
    {
        for (int64_t j = 0; j < map.nCols(); ++j)
        {
            if (map.get(i, j) != '.')
                antennas[map.get(i, j)].emplace_back( i, j );
        }
    }

    return antennas;

}





int64_t prob1(std::string inputFile)
{
    std::ifstream file(inputFile);
    auto content = readFile(file);
    auto map = Grid<char>(content);

    auto antennas = getAntennas(map);


    std::set<std::tuple<int64_t, int64_t>> knownAntinodes{};


    for (const auto& [_, pos] : antennas)
    {
        for (int i = 0; i < pos.size(); ++i)
        {
            vec<int64_t, 2> pi(pos[i]);

            for (int j = 0; j < pos.size(); ++j)
            {
                if (i == j)
                    continue;
                vec<int64_t, 2> pj(pos[j]);
                auto antinodePos = pi * 2 - pj;
                if (map.inBounds(antinodePos[0], antinodePos[1]))
                    knownAntinodes.insert({ antinodePos[0], antinodePos[1] });
            }
        }
    }
    return std::size(knownAntinodes);
}


int64_t prob2(std::string inputFile)
{
    std::ifstream file(inputFile);
    auto content = readFile(file);
    auto map = Grid<char>(content);

    auto antennas = getAntennas(map);


    std::set<std::tuple<int64_t, int64_t>> knownAntinodes{};


    for (const auto& [_, pos] : antennas)
    {
        for (int i = 0; i < pos.size(); ++i)
        {
            vec<int64_t, 2> pi(pos[i]);

            for (int j = i+1; j < pos.size(); ++j)
            {
                vec<int64_t, 2> pj(pos[j]);
                auto diff = pi - pj;
                auto gcd = std::gcd(diff[0], diff[1]);
                auto step = vec<int64_t, 2>({ diff[0] / gcd, diff[1] / gcd });

                auto p = pi;
                while(map.inBounds(p[0], p[1]))
                {
                    knownAntinodes.insert({ p[0], p[1] });
                    p += step;
                }

                p = pi - step;
                while (map.inBounds(p[0], p[1]))
                {
                    knownAntinodes.insert({ p[0], p[1] });
                    p -= step;
                }

            }
        }
    }
    return std::size(knownAntinodes);
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
