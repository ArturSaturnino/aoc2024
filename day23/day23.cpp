#include <common/common.h>

template<typename R, typename T>
concept RangeOfPairs =
std::ranges::range<R> &&
std::convertible_to<std::ranges::range_value_t<R>, std::pair<T,T>>;


template<typename T>
class Graph
{
private:
    std::map<T, std::set<T>> m_edges;
    std::set<T> m_vertices;

public:
   Graph(const RangeOfPairs<T> auto& edges)
    {
       for (const auto& [eL, eR] : edges)
       {
           m_edges[eL].insert(eR);
           m_vertices.insert(eL);
           m_vertices.insert(eR);
       }
    }
   


    const std::set<T>& getEdges(const T& vertex) const
    {
        return m_edges.at(vertex);
    }

    const std::set<T>& getVertices() const
    {
        return m_vertices;
    }
};


int64_t prob1(std::string inputFile)
{
    std::ifstream file(inputFile);
    auto content = readFile(file);
    auto edges = content | std::views::transform([](std::string_view s) {
        return  std::array<std::pair<std::pair<char, char>, std::pair<char, char>>,2>({
            std::pair(std::pair(s[0], s[1]), std::pair(s[3], s[4])),
            std::pair(std::pair(s[3], s[4]), std::pair(s[0], s[1])) 
            });
        });


   Graph<std::pair<char, char>> connectionGraph(std::views::join(edges) | std::ranges::to<std::vector>());


   const auto& vertices = connectionGraph.getVertices();


   int64_t tot = 0;
   
   for (auto it1 = vertices.lower_bound({ 't', '\0' }); it1 != vertices.end() && std::get<0>(*it1) == 't'; ++it1)
   {
       const auto& vEdges1 = connectionGraph.getEdges(*it1);
       for (const auto& v2: vEdges1)
       {
           const auto& vEdges2 = connectionGraph.getEdges(v2);

           if (std::get<0>(v2) == 't' && v2 > *it1)
               continue;

           for (auto it3 = vEdges2.upper_bound(v2); it3 != vEdges2.end(); ++it3)
           {

               if (std::get<0>(*it3) == 't' && *it3 > *it1)
                   continue;

               if (vEdges1.contains(*it3))
               {
                   ++tot;
               }    
           }
       }
   }


    return tot;
}


void getAllMaxSetsInner(const  Graph<std::pair<char, char>>& graph, const std::set<std::pair<char, char>>& clique, const std::set<std::pair<char, char>>& candidates,  std::set<std::string>& cliques)
{
    if (candidates.empty())
    {
        std::string cliqueStr = "";
        for (const auto& s : clique | std::views::transform([](const auto& p) { return std::format("{}{},", p.first, p.second); }))
            cliqueStr += s;

        cliques.insert(cliqueStr);
    }

    std::set<std::pair<char, char>> repetitionGuard;

    for (const auto& candidate : candidates)
    {
        if (repetitionGuard.contains(candidate))
            continue;
        auto cliqueN = clique;
        cliqueN.insert(candidate);
        std::set<std::pair<char, char>> candidatesN{};
        for (const auto& c : candidates)
        {
            if (c > candidate && graph.getEdges(candidate).contains(c))
            {
                candidatesN.insert(c);
                repetitionGuard.insert(c);
            }
        }
        getAllMaxSetsInner(graph, cliqueN, candidatesN, cliques);
    }
}

std::set<std::string> getAllMaxSets(const  Graph<std::pair<char, char>>& graph)
{
    std::set<std::string> cliques;


    for (const auto& vertex : graph.getVertices())
    {
        std::set<std::pair<char, char>> candidates{};
        for (const auto& c : graph.getEdges(vertex))
        {
            if (c > vertex)
                candidates.insert(c);
        }

        getAllMaxSetsInner(graph, { vertex }, candidates, cliques);
    }

    return cliques;
}

int64_t prob2(std::string inputFile)
{
    std::ifstream file(inputFile);
    auto content = readFile(file);
    auto edges = content | std::views::transform([](std::string_view s) {
        return  std::array<std::pair<std::pair<char, char>, std::pair<char, char>>, 2>({
            std::pair(std::pair(s[0], s[1]), std::pair(s[3], s[4])),
            std::pair(std::pair(s[3], s[4]), std::pair(s[0], s[1]))
            });
        });


    Graph<std::pair<char, char>> connectionGraph(std::views::join(edges) | std::ranges::to<std::vector>());

    auto maxSets = getAllMaxSets(connectionGraph);

    std::string_view maxSet;

    for (const auto& set : maxSets)
    {
        if (maxSet.size() < set.size())
            maxSet = set;
    }

    std::cout << maxSet << std::endl;


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
