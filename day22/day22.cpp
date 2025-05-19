#include <common/common.h>

constexpr int64_t hash(int64_t num)
{
    int64_t secret = num << 6;
    secret ^= num;
    secret %= 16777216;

    secret ^= secret >> 5;
    secret %= 16777216;


    secret ^= secret << 11;
    secret %= 16777216;

    return secret;

}


int64_t prob1(std::string inputFile)
{
    std::ifstream file(inputFile);
    auto content = readFile(file);
   
    int64_t tot = 0;
    for (int64_t n : content | std::views::transform([](auto s) { return readNumsl(s)[0]; }))
    {
        int64_t secret = n;
        for (int i = 0; i < 2000; ++i)
        {
            secret = hash(secret);
        }
        tot += secret;
    }


    return tot;
}


int64_t prob2(std::string inputFile)
{
    std::ifstream file(inputFile);
    auto content = readFile(file);

    std::map<std::tuple<int64_t, int64_t, int64_t, int64_t>, int64_t> payoffs;
    int64_t max = 0;

    for (int64_t n : content | std::views::transform([](auto s) { return readNumsl(s)[0]; }))
    {
        int64_t secret = n;
        std::set<std::tuple<int64_t, int64_t, int64_t, int64_t>> seenSequences;
        
        int64_t thisDigit, lastDigit;
        std::tuple<int64_t, int64_t, int64_t, int64_t> sequence;
        
        for (int i = 0; i < 2000; ++i)
        {
            lastDigit = secret % 10;
            secret = hash(secret);
            thisDigit = secret % 10;
            sequence = { std::get<1>(sequence), std::get<2>(sequence), std::get<3>(sequence), thisDigit - lastDigit };
            if (i >= 4 && !seenSequences.contains(sequence))
            {
                seenSequences.insert(sequence);
                payoffs[sequence] += thisDigit;
                max = std::max(max, payoffs[sequence]);
            }
        }

    }


    return max;
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
