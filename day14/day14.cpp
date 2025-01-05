#include <iostream>
#include <common/common.h>

#include <common/vec.h>


using IntVec = vec<int64_t, 2>;

struct particle
{
    IntVec p;
    IntVec v;
    particle(std::vector<int64_t> inp) :
        p{ {inp[1], inp[0]} },
        v{ {inp[3], inp[2]} }
    {
    };
};

class ToricEngine
{
public:
    ToricEngine(int64_t xLen, int64_t yLen) :
        m_xLen{ xLen },
        m_yLen{ yLen }
    {
    };

    void moveParticle(particle& part, int64_t step)
    {
        part.p += part.v * step;
        part.p[0] %= m_xLen;
        part.p[1] %= m_yLen;

        part.p[0] = (m_xLen + part.p[0]) % m_xLen;
        part.p[1] = (m_yLen + part.p[1]) % m_yLen;
    }

    int getQuadrant(const IntVec& pos)
    {
        if (pos[0] == m_xLen / 2 || pos[1] == m_yLen / 2)
            return 0;
        if (pos[0] < m_xLen / 2 && pos[1] > m_yLen / 2)
            return 1;
        if (pos[0] > m_xLen / 2 && pos[1] > m_yLen / 2)
            return 2;
        if (pos[0] > m_xLen / 2  && pos[1] < m_yLen / 2)
            return 3;
        if (pos[0] < m_xLen / 2 && pos[1] < m_yLen / 2)
            return 4;
        return 0;
    }


private:

    int64_t m_xLen;
    int64_t m_yLen;

};

void printParticles(const std::map<IntVec, int>& positions, int64_t nRows, int64_t nCols)
{

    for (int64_t i = 0; i < nRows; ++i)
    {
        std::string row(nCols, '.');
        for (int64_t j = 0; j < nCols; ++j)
        {
            IntVec p({ i , j });
            if (positions.contains(p))
                row[j] = std::to_string(positions.at(p) % 10)[0];
        }
        std::cout << row << '\n';
    }
    std::cout << "\n\n\n";

}

double calcVar(const std::map<IntVec, int>& positions)
{
    double var = 0;



    for (const auto& [p1, c1] : positions)
    {
        for (const auto& [p2, c2] : positions)
        {
            auto diff = p1 - p2;
            var += diff[0] * diff[0] + diff[1] * diff[1];
            var *= c1 * c2;
        }
    }
    return var;
}


int64_t prob1(std::string inputFile)
{
    std::ifstream file(inputFile);
    auto content = readFile(file);

    //ToricEngine engine(7, 11);
    ToricEngine engine(103, 101);
    int64_t steps = 100;

    
    std::map<int, int64_t> quadrantCounts;
    
    for (const auto& v : std::views::transform(content, readNumsl))
    {
        particle part(v);
        engine.moveParticle(part, steps);
        ++quadrantCounts[engine.getQuadrant(part.p)];
    }    
    
    return std::ranges::fold_left(std::views::values(quadrantCounts)  | std::views::drop(1), 1, std::multiplies{});
}


int64_t prob2(std::string inputFile)
{
    std::ifstream file(inputFile);
    auto content = readFile(file);

    ToricEngine engine(103, 101);
    int64_t steps = 100;


    std::vector<double> var;
    double minVar = std::numeric_limits<double>::max();
    int64_t candidate = 0;
    auto parts = std::views::transform(content, readNumsl) | std::ranges::to<std::vector>();


    for (int steps = 0; steps < 10000; ++steps)
    {
        std::map<IntVec, int> positions;
        for (const auto& v : parts)
        {
            particle part(v);
            engine.moveParticle(part, steps);
            positions[part.p] = 1;
        }

        double v = calcVar(positions);
        var.push_back(v);
        if (v < minVar)
        {
            minVar = v;
            candidate = steps;
        }
    }



    std::map<IntVec, int> positions;
    for (const auto& v : parts)
    {
        particle part(v);
        engine.moveParticle(part, 6620);
        positions[part.p] = 1;
    }
    printParticles(positions, 101, 103);

    return candidate;
}

int main()
{
    std::string test = "test.txt";
    std::string input = "input.txt";

    std::cout << "Test P1:" << std::endl << prob1(test) << std::endl;
    std::cout << "Input P1:" << std::endl << prob1(input) << std::endl << std::endl;

    //std::cout << "Test P2:" << std::endl << prob2(test) << std::endl;
    std::cout << "Input P2:" << std::endl << prob2(input) << std::endl;
}
