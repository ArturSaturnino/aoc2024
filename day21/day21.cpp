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

 std::vector<std::string> getNeighbors(std::string_view state, const KeyPad& directionalKeypad, const KeyPad& numericalKeypad)
 {
    
     std::vector<std::string> moves;
     
     int i = 0;
     while ( i < state.size() && state[i] == 'A')
         ++i;
    
     if (i + 1 < state.size())
     {
         const KeyPad& activeKeypad = i + 2 == state.size() ? numericalKeypad : directionalKeypad;
         char activeState = state[i + 1];
         auto activePos = activeKeypad.getPos(activeState);
         auto proposedPos = moveInDir(getDir(state[i]), activePos);
         if (activeKeypad.isValid(proposedPos))
         {
             std::string newState(state);
             newState[i + 1] = activeKeypad.getState(proposedPos);
             moves.push_back(newState);
         } 
     }
      
     const KeyPad& firstPad = state.size() == 0 ? numericalKeypad : directionalKeypad;
     auto currPos = firstPad.getPos(state[0]);
     for (const auto& d : firstPad.getValidMoves(currPos))
     {
         auto testMove = moveInDir(d, currPos);
         char testState = firstPad.getState(testMove);
         moves.emplace_back(state);
         moves.back()[0] = testState;
     }
     return moves;
 }

 int64_t getPathLength(char startState, char endState, int nRobots, const KeyPad& directionalKeypad, const KeyPad& numericalKeypad, std::vector<std::string>& path)
 {
     std::string startStateFull(nRobots, 'A');
     std::string endStateFull(nRobots, 'A');

     startStateFull.back() = startState;
     endStateFull.back() = endState;

     std::priority_queue<std::tuple<int64_t, std::string, std::string>> queue{};
     std::set<std::string> visited;
     std::map<std::string, std::string> parents;

     queue.emplace(0, startStateFull, "");

     while (!queue.empty())
     {
         auto [dist, state, parent] = queue.top();
         dist *= -1;
         queue.pop();

         if (visited.contains(state))
             continue;

         visited.insert(state);
         parents[state] = parent;

         if (state == endStateFull)
         {
             std::string pathState = state;
             while (!pathState.empty())
             {
                 path.push_back(pathState);
                 pathState = parents[pathState];
             }
             path = std::vector(path.rbegin(), path.rend());

             return dist + 1;
         }

         for (const auto& s : getNeighbors(state, directionalKeypad, numericalKeypad))
         {
             queue.push({ -dist -1, s, state});
         }
     }

     throw std::runtime_error("No path found");

 }


 int64_t getShortestSeq(std::string_view digits, const KeyPad& directionalKeypad, const KeyPad& numericalKeypad, int nRobots)
 {
     char lastState = 'A';
     int64_t tot = 0;
     std::string moves;
   
     for (const char nextState : digits)
     {
         std::vector<std::string> path;
         tot += getPathLength(lastState, nextState, nRobots, directionalKeypad, numericalKeypad, path);
         lastState = nextState;
     }
     return tot;
 }

int64_t prob1(std::string inputFile)
{
    std::ifstream file(inputFile);
    auto content = readFile(file);

    KeyPad numericalKeypad = getNumericalKeypad();
    KeyPad directionalKeypad = getDirectionalKeypad();

    int64_t tot = 0;
    for (const auto& moves : content)
    {

        tot += getShortestSeq(moves, directionalKeypad, numericalKeypad, 3) * std::stoll(moves.substr(0, moves.size() -1));
    }
    return tot;
}


int64_t prob2(std::string inputFile)
{
    std::ifstream file(inputFile);
    auto content = readFile(file);

    KeyPad numericalKeypad = getNumericalKeypad();
    KeyPad directionalKeypad = getDirectionalKeypad();

    int64_t tot = 0;
    for (const auto& moves : content)
    {
        tot += getShortestSeq(moves, directionalKeypad, numericalKeypad, 10) * std::stoll(moves.substr(0, moves.size() - 1));
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
