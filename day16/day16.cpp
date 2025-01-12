#include <common/common.h>
#include <common/grid.h>


using State = std::pair<std::pair<int64_t, int64_t>, dir>;


State getStart(const  Grid<char>& maze)
{
    for (auto itr = maze.begin(); itr != maze.end(); ++itr)
    {
        if (*itr == 'S')
            return{ {itr.getRow(), itr.getCol()}, dir::E };

    }
    return { {-1,-1}, dir::NE };
}

std::pair<int64_t, int64_t> getEnd(const  Grid<char>& maze)
{
    for (auto itr = maze.begin(); itr != maze.end(); ++itr)
    {
        if (*itr == 'E')
            return {itr.getRow(), itr.getCol()};

    }
    return {-1,-1};
}

bool canMoveForward(const  Grid<char>& maze, const State& state)
{
    return maze.get(moveInDir(state.second, state.first)) != '#';
}




int64_t calcMinCost(const  Grid<char>& maze)
{
    State currState = getStart(maze);
    std::map<State, int64_t> costs{ {currState, 0} };
    std::map<State, int64_t> toExplore;
    auto endPos = getEnd(maze);

    do
    {
        State lRot = { currState.first, rotateLeft(currState.second) };
        if (!costs.contains(lRot) || costs[lRot] > 1000 + costs[currState])
            toExplore[lRot] = 1000 + costs[currState];

        State rRot = { currState.first, rotateRight(currState.second) };
        if (!costs.contains(rRot) || costs[rRot] > 1000 + costs[currState])
            toExplore[rRot] = 1000 + costs[currState];

        if (canMoveForward(maze, currState))
        {
            State fwd = { moveInDir(currState.second, currState.first), currState.second };
            if (!costs.contains(fwd) || costs[fwd] > 1 + costs[currState])
                toExplore[fwd] = 1 + costs[currState];
        }
       
        auto cheapest = std::ranges::fold_left(toExplore, 
            std::pair<State, int64_t>{{}, std::numeric_limits<int64_t>::max()},
            [](const std::pair<State, int64_t>& pl, const std::pair<State, int64_t>& pr)
            {
            if (pl.second <= pr.second)
                return pl;
            return pr;
        });


        currState = cheapest.first;

        costs[currState] = cheapest.second;
        toExplore.erase(currState);

    } while (currState.first != endPos);
    
    return costs[currState];

}

std::set<std::pair<int64_t, int64_t>> getAllPaths(const  Grid<char>& maze)
{
    const State startState = getStart(maze);
    std::map<State, int64_t> costs{ {startState, 0} };
    std::set<State> explored;
    int64_t currCost = 0;
    int64_t targetCost = std::numeric_limits<int64_t>::max();
    auto endPos = getEnd(maze);

    std::map<State, std::set<State>> minDistTree;
    State currState = startState;
    while (currCost <= targetCost)
    {
        State lRot = { currState.first, rotateLeft(currState.second) };
        if (!costs.contains(lRot) || costs[lRot] >= 1000 + currCost)
        {
            if (costs[lRot] > 1000 + currCost)
                minDistTree[lRot] = {};

            costs[lRot] = 1000 + currCost;
            minDistTree[lRot].insert(currState);
        }


        State rRot = { currState.first, rotateRight(currState.second) };
        if (!costs.contains(rRot) || costs[rRot] >= 1000 + currCost)
        {
            if (costs[rRot] > 1000 + currCost)
                minDistTree[rRot] = {};

            costs[rRot] = 1000 + currCost;
            minDistTree[rRot].insert(currState);
        }


        if (canMoveForward(maze, currState))
        {
            State fwd = { moveInDir(currState.second, currState.first), currState.second };
            if (!costs.contains(fwd) || costs[fwd] >= 1 + currCost)
            {
                if (costs[fwd] > 1 + currCost)
                    minDistTree[fwd] = {};

                costs[fwd] = 1 + currCost;
                minDistTree[fwd].insert(currState);
            }

        }

        auto cheapest = std::ranges::fold_left(
            costs | std::views::filter([&explored](const auto& p) {return !explored.contains(p.first); }),
            std::pair<State, int64_t>{{}, std::numeric_limits<int64_t>::max()},
            [](const std::pair<State, int64_t>& pl, const std::pair<State, int64_t>& pr)
            {

                if (pl.second < pr.second)
                    return pl;
                return pr;
            });

        explored.insert(currState);
        if (currState.first == endPos)
            targetCost = std::min(currCost, targetCost);

        currState = cheapest.first;
        currCost = cheapest.second;
    }


    std::set<std::pair<int64_t, int64_t>> lanes;
    

    for (const auto& d: cardinal_dirs)
    {
        State state = { endPos, d };
        auto stateSet = minDistTree[state];
        lanes.insert(state.first);

        while (!stateSet.empty())
        {
            std::set<State> nextStateSet{};
            for (const auto& s : stateSet)
            {
                nextStateSet.merge(minDistTree[s]);
                lanes.insert(s.first);
            }
           
            stateSet = nextStateSet;
        }
    }

    return lanes;
}


int64_t prob1(std::string inputFile)
{
    std::ifstream file(inputFile);
    auto content = readFile(file);
    Grid<char> maze(content);


    return calcMinCost(maze);
}


int64_t prob2(std::string inputFile)
{
    std::ifstream file(inputFile);
    auto content = readFile(file);
    Grid<char> maze(content);

    return  getAllPaths(maze).size();
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
