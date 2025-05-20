#include <common/common.h>

enum class GateType
{
    AND,
    OR,
    XOR
};


struct Gate
{
    GateType type;
    std::string in1;
    std::string in2;
    std::string out;
    int64_t id;

    bool eval(bool in1, bool in2) const
    {
        switch (type)
        {
        case GateType::AND:
            return in1 && in2;
            break;
        case GateType::OR:
            return in1 || in2;
            break;
        case GateType::XOR:
            return in1 != in2;
            break;
        default:
            break;
        }
    }
};

class Circuit
{
private:
    std::vector<Gate> m_gates;
    std::unordered_map<std::string_view, std::vector<const Gate*>> m_inConnections;

public:
    Circuit(const std::vector<Gate>& gates) :
        m_gates{ gates }
    {
        for (const auto& gate : gates)
        {
            m_inConnections[gate.in1].push_back(&gate);
            m_inConnections[gate.in2].push_back(&gate);
        }
    }

    std::unordered_map<std::string, bool> calcWires(const std::unordered_map<std::string, bool>& startValues) const
    {
        std::unordered_map<int64_t, int64_t> dependenceMap;
        std::vector<const Gate*> computeQueue;
        std::unordered_map<std::string, bool> wireValues{ startValues };

        for (const auto& [wire, val] : wireValues)
        {
            if (!m_inConnections.contains(wire))
                continue;

            for (const auto& gate : m_inConnections.at(wire))
            {
                int n = ++dependenceMap[gate->id];
                if (n == 2)
                    computeQueue.push_back(gate);
            }
        }

        while (!computeQueue.empty())
        {
            const Gate* gate = computeQueue.back();
            computeQueue.pop_back();
            wireValues[gate->out] = gate->eval(wireValues[gate->in1], wireValues[gate->in2]);

            if (!m_inConnections.contains(gate->out))
                continue;

            for (const auto& gateI : m_inConnections.at(gate->out))
            {
                int n = ++dependenceMap[gateI->id];
                if (n == 2)
                    computeQueue.push_back(gateI);
            }
        }
        return wireValues;

    }

    int64_t calcOutput(const std::unordered_map<std::string, bool>& startValues) const
    {
        int64_t tot = 0;
        for (const auto& [wire, val] : calcWires(startValues))
        {
            if (wire[0] != 'z')
                continue;
            auto n = std::stol(std::string(wire.substr(1)));
            tot += static_cast<int64_t>(val) * (1i64 << n);
        }

        return tot;

    }


};

Gate readGate(std::string_view gateStr, int64_t id)
{
    Gate gate;

    gate.id = id;
    gate.in1 = gateStr.substr(0, 3);

    int nextGatePos = 0;
    
    switch (gateStr[4])
    {
    case 'A':
        nextGatePos = 8;
        gate.type = GateType::AND;
        break;
    case 'O':
        nextGatePos = 7;
        gate.type = GateType::OR;
        break;
    case 'X':
        nextGatePos = 8;
        gate.type = GateType::XOR;
        break;
    default:
        break;
    }

    gate.in2 = gateStr.substr(nextGatePos, 3);
    gate.out = gateStr.substr(nextGatePos+7, 3);

    return gate;
}



int64_t prob1(std::string inputFile)
{
    std::ifstream file(inputFile);
    auto content = readFile(file);

    std::unordered_map<std::string, bool> startValues;
    std::vector<Gate> gates;


    auto it = content.begin();

    for (; it != content.end() && !it->empty(); ++it)
    {
        startValues[std::string(it->c_str(), 3)] = (*it)[5] == '1';
    }
    ++it;
    int64_t id = 0;
    for (; it != content.end(); ++it, ++id)
    {
        gates.push_back(readGate(*it, id));
    }

    Circuit circuit(gates);

    return circuit.calcOutput(startValues);
}


int64_t prob2(std::string inputFile)
{
    std::ifstream file(inputFile);
    auto content = readFile(file);

    std::unordered_map<std::string_view, bool> startValues;
    std::vector<Gate> gates;


    auto it = content.begin();

    for (; it != content.end() && !it->empty(); ++it)
    {
    }
    ++it;
    int64_t id = 0;
    for (; it != content.end(); ++it, ++id)
    {
        gates.push_back(readGate(*it, id));
    }

    Circuit circuit(gates);



    std::string carryWire = "prt";

    std::array<std::tuple<bool, bool, bool>, 8> testCases =
    {
        std::tuple {false, false, false},
        std::tuple {true, false, false},
        std::tuple {false, true, false},
        std::tuple {true, true, false},
        std::tuple {false, false, true},
        std::tuple {true, false, true},
        std::tuple {false, true, true},
        std::tuple {true, true, true},
    };

   for (int i = 1; i <= 44; ++i)
   {
       std::string numStr = std::format("{:02d}", i);

       std::vector<std::set<std::string>> nextCarryCandidates;
       std::vector<std::set<std::string>> outputCandidates;

       for (const auto& [xT, yT, cT] : testCases)
       {
           std::unordered_map<std::string, bool> testVals{};
           testVals["x" + numStr] = xT;
           testVals["y" + numStr] = yT;
           testVals[carryWire] = cT;

           auto outWires = circuit.calcWires(testVals);

           std::set<std::string> outCands;
           std::set<std::string> nextCarryCands;
           for (const auto& [wire, val] : outWires)
           {
               if (val == (xT ^ yT ^ cT))
                   outCands.insert(wire);
               if (val == (static_cast<int>(xT) + static_cast<int>(yT) + static_cast<int>(cT) >= 2))
                   nextCarryCands.insert(wire);
           }
           outputCandidates.push_back(outCands);
           nextCarryCandidates.push_back(nextCarryCands);
       }

       auto nextCarry = nextCarryCandidates[0];
       for (const auto& set : nextCarryCandidates | std::views::drop(1))
       {
           std::set<std::string> nextCarryTemp{};
           for (const auto& wire : nextCarry)
           {
               if (set.contains(wire))
                   nextCarryTemp.insert(wire);
           }
           nextCarry = nextCarryTemp;
       }

       if (nextCarry.size() != 1)
       {
           std::cout << i << '\n';
           return 0;
       }

       carryWire = *nextCarry.begin();

       auto outWire = outputCandidates[0];
       for (const auto& set : outputCandidates | std::views::drop(1))
       {
           std::set<std::string> outTemp{};
           for (const auto& wire : outWire)
           {
               if (set.contains(wire))
                   outTemp.insert(wire);
           }
           outWire = outTemp;
       }

       if (outWire.size() != 1)
       {
           std::cout << i << '\n';
           return 0;
       }

   }

    return 0;
}

int main()
{
    std::string test = "test.txt";
    std::string input = "input.txt";

    std::cout << "Test P1:" << std::endl << prob1(test) << std::endl;
    std::cout << "Input P1:" << std::endl << prob1(input) << std::endl << std::endl;

    std::cout << "Test P2:" << std::endl << prob2("fixed.txt") << std::endl;
    //std::cout << "Input P2:" << std::endl << prob2(input) << std::endl;
}
