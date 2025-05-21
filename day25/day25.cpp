#include <common/common.h>



int64_t prob1(std::string inputFile)
{
    std::ifstream file(inputFile);
    auto content = readFile(file);

    std::map<std::tuple<int, int, int, int, int>, int> locks{};
    std::map<std::tuple<int, int, int, int, int>, int> keys{};


    for (const auto& dataChunk : content | std::views::chunk_by([](const auto& l, const auto& r) {return !l.empty();}))
    {
        char typeChar = dataChunk[0][0];
        std::tuple<int, int, int, int, int> heights {0,0,0,0,0};

        for (const auto& line: dataChunk | std::views::drop(1))
        {
            if (line.size() < 5)
                continue;
            int i = 0;
            std::apply([&i, &typeChar, &line](auto& ... h) {((h += line[i++] == typeChar), ...); }, heights);
        }

        if (typeChar == '#')
        {
            ++locks[heights];
        }
        else
        {
            int i = 0;
            std::apply([&i](auto& ... h) {((h = 5-h), ...); }, heights);
            ++keys[heights];
        }
    }

    int64_t tot = 0;

    for (const auto& [key, countK] : keys)
    {
        auto itEnd = locks.upper_bound(key);

        for (const auto& [lock, countL] : locks)
        {
            if( std::get<0>(key) + std::get<0>(lock) <= 5 &&
                std::get<1>(key) + std::get<1>(lock) <= 5 &&
                std::get<2>(key) + std::get<2>(lock) <= 5 &&
                std::get<3>(key) + std::get<3>(lock) <= 5 &&
                std::get<4>(key) + std::get<4>(lock) <= 5
                )
                tot += countK * countL;
        }
    }


    return tot;
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
