#include <iostream>
#include <common/common.h>
#include <common/cachedFunction.h>


int64_t isValid(int64_t targetVal, const std::ranges::range auto& nums, bool allowConcat = false)
{

    std::function<bool(int64_t, int64_t)> contConfig
        = [&contConfig, &nums, &allowConcat](int64_t target, int64_t pos)->int64_t
        {
            if (pos == 1)
                return target == nums[pos - 1];

            bool out = false;
            if (target % nums[pos - 1] == 0 && target / nums[pos - 1] > 0)
                out = out || contConfig(target / nums[pos - 1], pos - 1);
            
            if (out)
                return true;
            
            if (target - nums[pos - 1] > 0)
                out = out || contConfig(target - nums[pos - 1], pos - 1);
            
            if (out)
                return true;

            if (allowConcat)
            {
                int64_t temp = nums[pos - 1];
                int64_t modTarget = target;
                while (temp > 0)
                {
                    if (temp % 10 != modTarget % 10)
                        return out;
                    temp /= 10;
                    modTarget /= 10;
                }

                out = out || contConfig(modTarget, pos - 1);

            }

            return out;
        };

    contConfig = CashedFunction<bool, int64_t, int64_t>(contConfig);
    
    return contConfig(targetVal, nums.size());

}




int64_t prob1(std::string inputFile)
{
    std::ifstream file(inputFile);
    auto content = readFile(file);

    int64_t tot = 0;

    for (const auto& v : std::views::transform(content, readNumsl))
    {
        tot += isValid(v[0], std::views::drop(v, 1)) ? v[0] : 0;
    }


    return tot;
}


int64_t prob2(std::string inputFile)
{
    std::ifstream file(inputFile);
    auto content = readFile(file);

    int64_t tot = 0;

    for (const auto& v : std::views::transform(content, readNumsl))
    {
        tot += isValid(v[0], std::views::drop(v, 1), true) ? v[0] : 0;
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
