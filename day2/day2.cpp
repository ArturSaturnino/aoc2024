#include <iostream>
#include <common/common.h>


int64_t prob1(std::string inputFile)
{
    std::ifstream file(inputFile);
    auto content = readFile(file);
    auto reports = std::ranges::transform_view(content, [](const auto& line) {return readNumsl(line); });

    auto isValid = [](const auto& diffs)
        {
            int64_t dLast = 0;
            for ( auto d : diffs)
            {
                if (d * dLast < 0)
                    return false;
                if (std::abs(d) == 0 || std::abs(d) > 3)
                    return false;
                dLast = d;
            }
            return true;
        };

    auto validView = reports | std::views::transform([&isValid](const auto& report) {
        return isValid( 
            report
            | std::views::pairwise_transform([](int64_t l, int64_t r) {return r - l; })
        )
            ;
        }
    );

    return std::ranges::fold_left(validView, 0, [](int64_t v, bool b) {return v + b; });
}


int64_t prob2(std::string inputFile)
{
    std::ifstream file(inputFile);
    auto content = readFile(file);
    auto reports = std::ranges::transform_view(content, [](const auto& line) {return readNumsl(line); });


    auto isValid = [](const auto& report)
        {
            int64_t dLast = 0;
            for (int64_t d : std::views::pairwise_transform(report, [](const int64_t l, const int64_t r) {return r - l; }))
            {
                if (d * dLast < 0)
                    return false;
                if (std::abs(d) == 0 || std::abs(d) > 3)
                    return false;
                dLast = d;
            }
            return true;
        };

    auto validView = reports | std::views::transform([&isValid](const auto& report) {

        if (isValid(report))
           return true;

        for (size_t i = 0; i < report.size(); ++i)
        {
            std::vector<std::vector<int64_t>> pairViews
            {
            std::views::take(report, i) | std::ranges::to<std::vector>(),
            std::views::drop(report, i + 1) | std::ranges::to<std::vector>()
            };

            auto subReport = std::views::join(pairViews);
            if (isValid(subReport))
                return true;

        }

        return false;

        }
    );

    return  std::ranges::fold_left(validView, 0, [](int64_t v, bool b) {return v + b; });;
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
