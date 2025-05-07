#include <common/common.h>
#include <common/grid.h>

#include <queue>

class Maze
{
private:
    Grid<char> m_maze;
    std::map<std::pair<size_t, size_t>, int64_t> m_sDist;
    std::map<std::pair<size_t, size_t>, int64_t> m_eDist;
    std::pair<size_t, size_t> m_start;
    std::pair<size_t, size_t> m_end;

public:

    std::map<std::pair<size_t, size_t>, int64_t> getDistances(const std::pair<size_t, size_t>& basePt) const
    {
        std::map<std::pair<size_t, size_t>, int64_t> distances;

        using Entry = std::pair<std::pair<size_t, size_t>, int64_t>;

        auto comp = [](const Entry& left, const Entry& right) -> bool
            {
                return left.second > right.second;
            };
        std::priority_queue<Entry, std::vector<Entry>, decltype(comp)> queue;
        queue.emplace(basePt, 0);

        while (queue.size() > 0)
        {
            auto [pt, dis] = queue.top();
            queue.pop();

            if (distances.contains(pt))
                continue;

            distances[pt] = dis;
            for (const auto& d : cardinal_dirs)
            {
                auto pt_d = moveInDir(d, pt);
                if (m_maze.inBounds(pt_d) && m_maze.get(pt_d) != '#' && !distances.contains(pt_d))
                    queue.emplace(pt_d, dis + 1);
            }
        }

        return distances;
    }

    Maze(const std::vector<std::string>& content) :
        m_maze(content)
    {
        for (auto itr = m_maze.begin(); itr != m_maze.end(); ++itr)
        {
            if (*itr == 'S')
                m_start = { itr.getRow(), itr.getCol() };
            if (*itr == 'E')
                m_end = { itr.getRow(), itr.getCol() };
        }
        m_sDist = getDistances(m_start);
        m_eDist = getDistances(m_end);
    }

    std::vector<std::tuple<std::pair<size_t, size_t>, std::pair<size_t, size_t>, int64_t>> getShortcuts() const
    {
        int64_t baseDist = m_sDist.at(m_end);
        std::vector<std::tuple<std::pair<size_t, size_t>, std::pair<size_t, size_t>, int64_t>> shortcuts;

        for (const auto& [pt, sDis] : m_sDist)
        {
            for (const auto& d1 : cardinal_dirs)
            {
                auto pt_st = moveInDir(d1, pt);
                if (m_maze.inBounds(pt_st) && m_maze.get(pt_st) == '#')
                {
                    for (const auto& d2 : cardinal_dirs)
                    {
                        auto pt_end = moveInDir(d2, pt_st);
                        if (m_eDist.contains(pt_end))
                            shortcuts.emplace_back(pt_st, pt_end, baseDist - (sDis + 2 + m_eDist.at(pt_end)));
                    }
                }
            }
        }
        return shortcuts;
    }

    std::map<std::pair<std::pair<size_t, size_t>, std::pair<size_t, size_t>>, int64_t> getShortcuts(int cheatDuration) const
    {
        int64_t baseDist = m_sDist.at(m_end);
        std::map<std::pair<std::pair<size_t, size_t>, std::pair<size_t, size_t>>, int64_t> shortcuts;

        for (const auto& [pt, sDis] : m_sDist)
        {
            using Entry = std::pair<std::pair<size_t, size_t>, int64_t>;

            auto comp = [](const Entry& left, const Entry& right) -> bool
                {
                    return left.second > right.second;
                };
            std::priority_queue<Entry, std::vector<Entry>, decltype(comp)> queue;

            std::vector<std::pair<size_t, size_t>> pt_ends;
            std::set<std::pair<size_t, size_t>> explored;
            queue.emplace(pt, 0);


            while (queue.size() > 0)
            {
                auto [pt_curr, dist] = queue.top();
                queue.pop();

                if (explored.contains(pt_curr))
                    continue;

                explored.insert(pt_curr);

                if (m_eDist.contains(pt_curr) && dist > 0)
                {
                    shortcuts[{pt, pt_curr}] = baseDist - (sDis + dist + m_eDist.at(pt_curr));
                    continue;
                }
                else
                {
                    for (const auto& d : cardinal_dirs)
                    {
                        auto pt_d = moveInDir(d, pt_curr);

                        if (m_maze.get(pt_curr) != '#' && m_maze.get(pt_d) != '#')
                            continue;

                        if (m_maze.inBounds(pt_d) && dist < cheatDuration)
                        {
                            queue.emplace(pt_d, dist + 1);
                        }
                    }
                }
            }
        }
        return shortcuts;
    }
};


int64_t prob1(std::string inputFile)
{
    std::ifstream file(inputFile);
    auto content = readFile(file);

    Maze maze(content);
    auto shortcuts = maze.getShortcuts();

    int64_t shortcutCounts = 0;

    for (const auto [pt_st, pt_end, s] : shortcuts)
    {
        if(s >= 100)
            ++shortcutCounts;
    }

    return shortcutCounts;
}


int64_t prob2(std::string inputFile)
{
    std::ifstream file(inputFile);
    auto content = readFile(file);

    Maze maze(content);
    auto shortcuts = maze.getShortcuts(20);

    int64_t shortcutCounts = 0;
    std::map<int64_t, int64_t> shortcutCountsMap{};


    for (const auto [pts, s] : shortcuts)
    {
        ++shortcutCountsMap[s];
        if (s >= 100)
            ++shortcutCounts;
    }

    return shortcutCounts;
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
