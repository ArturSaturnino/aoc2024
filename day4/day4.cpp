#include <iostream>
#include <common/common.h>



int64_t prob1(std::string inputFile)
{
    std::ifstream file(inputFile);
    auto content = readFile(file);

    auto puzzle = Grid<const char>(content | std::views::transform([](const auto& str) {return std::span(str); }) | std::ranges::to<std::vector>() );

    std::string targetStr = "XMAS";
    int64_t count = 0;

    for (int i = 0; i < puzzle.nRows(); ++i)
    {
        for (int j = 0; j < puzzle.nCols(); ++j)
        {
            for (const auto& d : dirs)
            {
                auto line = puzzle.getLine(i, j, d);
                auto itr = line.begin();
                int64_t c = 0;
                auto end = line.end();
                while (itr != line.end() && c < targetStr.size() && (*itr) == targetStr[c])
                {
                    ++c;
                    ++itr;
                }
                if (c == targetStr.size())
                {
                    ++count;
                    //std::cout << i << ", " << j << " " << (int)d << std::endl;
                }
                    

            }
        }
    }
        


    return count;
}


int64_t prob2(std::string inputFile)
{
    std::ifstream file(inputFile);
    auto content = readFile(file);

    auto puzzle = Grid<const char>(content | std::views::transform([](const auto& str) {return std::span(str); }) | std::ranges::to<std::vector>());

    int64_t count = 0;

    for (int i = 1; i < puzzle.nRows()-1; ++i)
    {
        for (int j = 1; j < puzzle.nCols()-1; ++j)
        {
            if (puzzle.get(i,j) == 'A')
            {
                auto neDiag = puzzle.getLine(i - 1, j - 1, dir::NE);
                std::string neStr{"***"};
                std::copy_n(neDiag.begin(),3, neStr.begin());
                if (neStr == "MAS" || neStr == "SAM")
                {
                    auto swDiag = puzzle.getLine(i - 1, j + 1, dir::NW);
                    std::string swStr{ "***" };
                    std::copy_n(swDiag.begin(), 3, swStr.begin());
                    if (swStr == "MAS" || swStr == "SAM")
                    {
                        ++count;
                    }   
                }
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
