#include <common/common.h>
#include <common/grid.h>

enum class entityEnum
{
    wall,
    box,
    robot,
    empy,
    boxL,
    boxR
};



class Warehouse
{
public:
    Warehouse(const Grid<char> initialState, bool wide = false) :
        m_state(initialState.nRows(), wide ? initialState.nCols()*2 : initialState.nCols())
    {
        for (auto it = initialState.begin(); it != initialState.end(); ++it)
       {
            int i = it.getRow();
            int j = it.getCol();

            if (wide)
            {
                switch (*it)
                {
                case '#':
                    m_state.get(i, 2 * j) = entityEnum::wall;
                    m_state.get(i, 2 * j+1) = entityEnum::wall;
                    break;
                case 'O':
                    m_state.get(i, 2 * j) = entityEnum::boxL;
                    m_state.get(i, 2 * j +1) = entityEnum::boxR;
                    break;
                case '@':
                    m_state.get(i, 2 * j) = entityEnum::robot;
                    m_state.get(i, 2 * j + 1 ) = entityEnum::empy;
                    m_robotPos = { i, 2 * j };
                    break;
                default:
                    m_state.get(i, 2* j) = entityEnum::empy;
                    m_state.get(i, 2 * j + 1) = entityEnum::empy;
                    break;
                }
            }
            else
            {
                switch (*it)
                {
                case '#':
                    m_state.get(i, j) = entityEnum::wall;
                    break;
                case 'O':
                    m_state.get(i, j) = entityEnum::box;
                    break;
                case '@':
                    m_state.get(i, j) = entityEnum::robot;
                    m_robotPos = { i,j };
                    break;
                default:
                    m_state.get(i, j) = entityEnum::empy;
                    break;
                }
            }
        }
    }

    void moveRobot(dir d)
    {
        std::vector<std::set<std::pair<int64_t, int64_t>>> moveStack = { {m_robotPos} };

        while (!moveStack.back().empty())
        {
            std::set<std::pair<int64_t, int64_t>> nextLayer;

            for (const auto& [r, c] : moveStack.back())
            {
                auto testPos = moveInDir(d, r, c);
                if (m_state.get(testPos) == entityEnum::wall)
                    return;
                if (m_state.get(testPos) == entityEnum::box)
                    nextLayer.insert(testPos);
                if (m_state.get(testPos) == entityEnum::boxL)
                {
                    nextLayer.insert(testPos);
                    if(d == dir::N || d == dir::S)
                        nextLayer.insert({ testPos.first, testPos.second + 1 });
                }
                if (m_state.get(testPos) == entityEnum::boxR)
                {
                    nextLayer.insert(testPos);
                    if (d == dir::N || d == dir::S)
                        nextLayer.insert({ testPos.first, testPos.second - 1 });
                }

            }
            moveStack.emplace_back(std::move(nextLayer));
        }

        for (auto itr = moveStack.rbegin(); itr != moveStack.rend(); ++itr)
        {
            for (const auto& [r, c] : *itr)
            {
                auto nextPos = moveInDir(d, r, c);
                m_state.get(nextPos) = m_state.get(r, c);
                m_state.get(r, c) = entityEnum::empy;
            }
        }

        m_robotPos = moveInDir(d, m_robotPos.first, m_robotPos.second);

    }

    int64_t calcGPSSum() const
    {
        int64_t tot = 0;
        for (auto it = m_state.begin(); it != m_state.end(); ++it)
        {
            if (*it == entityEnum::box || *it == entityEnum::boxL)
                tot += 100 * (m_state.nRows() - 1 - it.getRow()) + it.getCol();
        }
        return tot;
    }

    void print() const
    {
        std::string out;
        int currRow = m_state.nRows() - 1;
        int pos = 0;

        for (int i = m_state.nRows() - 1; i >= 0; --i)
        {
            std::string temp(m_state.nCols() + 1, '\n');

            for (int j = 0; j < m_state.nCols(); ++j)
            {
                switch (m_state.get(i, j))
                {
                case entityEnum::empy:
                    temp[j] = '.';
                    break;
                case entityEnum::wall:
                    temp[j] = '#';
                    break;
                case entityEnum::box:
                    temp[j] = 'O';
                    break;
                case entityEnum::boxL:
                    temp[j] = '[';
                    break;
                case entityEnum::boxR:
                    temp[j] = ']';
                    break;
                case entityEnum::robot:
                    temp[j] = '@';
                    break;
                default:
                    break;
                }
            }

            out = out + temp;
        }
       
        std::cout << out << "\n\n\n";
    }


private:

    Grid<entityEnum> m_state;
    std::pair<int64_t, int64_t> m_robotPos;
};



int64_t prob1(std::string inputFile)
{
    std::ifstream file(inputFile);
    auto content = readFile(file);

    auto gridStr = std::views::take_while(content, [](const std::string& s) -> bool {return !s.empty(); });

    Grid<char> grid(gridStr);
    Warehouse warehouse( grid );

    auto commands = std::views::join(std::views::drop_while(content, [](const std::string& s) -> bool {return !s.empty(); }));

    for (auto c : commands)
    {
        warehouse.moveRobot(getDir(c));
    }

    return warehouse.calcGPSSum();
}


int64_t prob2(std::string inputFile)
{
    std::ifstream file(inputFile);
    auto content = readFile(file);

    auto gridStr = std::views::take_while(content, [](const std::string& s) -> bool {return !s.empty(); });

    Grid<char> grid(gridStr);
    Warehouse warehouse(grid, true);

    auto commands = std::views::join(std::views::drop_while(content, [](const std::string& s) -> bool {return !s.empty(); }));
    
    for (auto c : commands)
    {   
        warehouse.moveRobot(getDir(c));
    }


    return warehouse.calcGPSSum();
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
