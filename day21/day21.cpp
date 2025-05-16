#include <common/common.h>
#include <common/grid.h>
#include <queue>

class KeyPad
{
    Grid<char> m_keypad;
    std::map<char, std::pair<int64_t, int64_t>> m_posMap;

public:

    KeyPad(const Grid<char>& keypad) :
        m_keypad{ keypad }
    {
        for (auto it =  m_keypad.begin(); it != m_keypad.end(); ++ it)
        {
            m_posMap[*it] = {it.getRow(), it.getCol()};
        }
    }

    bool isValid(const std::pair<int64_t, int64_t>& pos) const
    {
        return m_keypad.inBounds(pos) && m_keypad.get(pos) != '\0';
    }

    std::vector<dir> getValidMoves(const std::pair<int64_t, int64_t>& pos) const
    {
        std::vector<dir> validMoves;
        for (const auto& d : cardinal_dirs)
        {
            auto newPos = moveInDir(d, pos);
            if (isValid(newPos))
                validMoves.push_back(d);
        }
        return validMoves;
    }


    std::pair<int64_t, int64_t> getPos(char state) const
    {
        if (auto it = m_posMap.find(state); it != m_posMap.end())
            return it->second;
        throw std::runtime_error("State not found!");
    }


    char getState(const std::pair<int64_t, int64_t>& pos) const
    {
        return m_keypad.get(pos);
    }

    std::vector<char> getKeys() const
    {
        std::vector<char> keys;
        for (char k : m_keypad)
        {
            if (k)
                keys.push_back(k);
        }

        return keys;
    }


};

KeyPad getNumericalKeypad()
{
    std::vector<std::vector<char>> keys{
        { '7', '8', '9' },
        { '4', '5', '6' },
        { '1', '2', '3' },
        { '\0', '0', 'A' }
    };


    KeyPad keyPad(keys);

   return keyPad;
}

KeyPad getDirectionalKeypad()
 {
     std::vector<std::vector<char>> keys{
         { '\0', '^', 'A' },
         { '<', 'v', '>' }
     };

     KeyPad keyPad(keys);

     return keyPad;
 }

 auto getDistances(int nRobots, const KeyPad& directionalKeypad, const KeyPad& numericalKeypad)
 {
   
     std::map<std::tuple<int, char, char>, int64_t> distances;
     for (int i = 0; i < nRobots; ++i)
     {
         const KeyPad& activeKeypad = i + 1 == nRobots ? numericalKeypad : directionalKeypad;
         auto keys = activeKeypad.getKeys();

         for (const auto& start : keys)
         {
             std::priority_queue<std::tuple<int64_t, std::string>> queue{};
             std::set<std::string> visited;

             queue.emplace(0, "A" + std::string(1,start));

             while (!queue.empty())
             {
                 auto [ dist, state] = queue.top();
                 dist *= -1;
                 queue.pop();

                 if (visited.contains(state))
                     continue;

                 if (state[0] == 'A')
                     distances[{i, start, state[1]}] = dist;

                 visited.insert(state);

                                
                 for (const auto& k : directionalKeypad.getKeys())
                 {
                     int64_t moveCost = distances[{i - 1, state[0], k}];
                     std::string testStateFull = state;
                     testStateFull[0] = k;
                     if (!visited.contains(testStateFull))
                        queue.push({ -dist - moveCost, testStateFull });
                 }

                 if (state[0] == 'A')
                 {
                     distances[{i, start, state[1]}] = dist;
                 }
                 else
                 {
                     auto currPosActive = activeKeypad.getPos(state[1]);
                     auto posActiveProposed = moveInDir(getDir(state[0]), currPosActive);
                     if (activeKeypad.isValid(posActiveProposed))
                     {
                         std::string testStateFull = state;
                         testStateFull[1] = activeKeypad.getState(posActiveProposed);
                         if (!visited.contains(testStateFull))
                            queue.push({ -dist - 1, testStateFull });
                     }
                 }

             }
         }

     }
     return distances;

 }


int64_t prob1(std::string inputFile)
{
    std::ifstream file(inputFile);
    auto content = readFile(file);

    KeyPad numericalKeypad = getNumericalKeypad();
    KeyPad directionalKeypad = getDirectionalKeypad();

    int nRobots = 3;
    auto distMap = getDistances(nRobots, directionalKeypad, numericalKeypad);
    int64_t tot = 0;

    for (const auto& moves : content)
    {
        char lastState = 'A';
        int64_t dist = 0;
        for (const char nextState : moves)
        {
            dist += distMap[{nRobots - 1, lastState, nextState}] + 1;
            lastState = nextState;
        }
        tot += dist * std::stoll(moves.substr(0, moves.size() -1));
    }
    return tot;
}


int64_t prob2(std::string inputFile)
{
    std::ifstream file(inputFile);
    auto content = readFile(file);

    KeyPad numericalKeypad = getNumericalKeypad();
    KeyPad directionalKeypad = getDirectionalKeypad();

    int nRobots = 26;
    auto distMap = getDistances(nRobots, directionalKeypad, numericalKeypad);
    int64_t tot = 0;

    for (const auto& moves : content)
    {
        char lastState = 'A';
        int64_t dist = 0;
        for (const char nextState : moves)
        {
            dist += distMap[{nRobots - 1, lastState, nextState}] + 1;
            lastState = nextState;
        }
        tot += dist * std::stoll(moves.substr(0, moves.size() - 1));
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
