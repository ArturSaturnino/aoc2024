#include <common/common.h>
#include <common/grid.h>
#include <common/vec.h>

using IntVec = vec<int64_t, 2>;

enum class entityEnum
{
    wall,
    box,
    robot, 
    empy
};



class Warehouse
{
public:
    Warehouse(const Grid<char> initialState) :
        m_state(initialState.nRows(), initialState.nCols())
    {
        for (auto it = initialState.begin(); it != initialState.end(); ++it)
       {
            int i = it.getRow();
            int j = it.getCol();
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

    void moveRobot(dir d)
    {
        auto testPos = m_robotPos;
        auto line = m_state.getLine(m_robotPos.first, m_robotPos.second, d);
        
        auto itr = line.begin();

        while (*itr != entityEnum::wall && *itr != entityEnum::empy)
            ++itr;
        
        if (*itr == entityEnum::empy)
        {
            auto iterLast = itr;
            while (itr != line.begin())
            {
                *iterLast = *(--itr);
                iterLast = itr;
            }
            *iterLast = entityEnum::empy;
            m_robotPos = moveInDir(d, m_robotPos.first, m_robotPos.second);
            
        }
    }

    int64_t calcGPSSum() const
    {
        int64_t tot = 0;
        for (auto it = m_state.begin(); it != m_state.end(); ++it)
        {
            if (*it == entityEnum::box)
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


dir getDir(char c)
{
    switch (c)
    {
    case '^':
        return dir::N;
    case '>':
        return dir::E;
    case 'v':
        return dir::S;
    case '<':
        return dir::W;
    default:
        break;
    }

    return dir::SW;
}





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
        //warehouse.print();
        warehouse.moveRobot(getDir(c));
    }
    //warehouse.print();


    return warehouse.calcGPSSum();
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
