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

    int64_t calcOutput(const std::unordered_map<std::string_view, bool>& startValues) const
    {
        std::unordered_map<int64_t, int64_t> dependenceMap;
        std::vector<const Gate*> computeQueue;
        std::unordered_map<std::string_view, bool> wireValues{ startValues };

        for (const auto& [wire, val] : wireValues)
        {
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


        int64_t tot = 0;
        for (const auto& [wire, val] : wireValues)
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

    std::unordered_map<std::string_view, bool> startValues;
    std::vector<Gate> gates;


    auto it = content.begin();

    for (; it != content.end() && !it->empty(); ++it)
    {
        startValues[std::string_view(it->c_str(), 3)] = (*it)[5] == '1';
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
