#include <iostream>
#include <list>
#include <common/common.h>

class FileSystem
{
public:
    struct file
    {
        uint64_t id;
        uint64_t startPos;
        uint64_t endPos;

        bool operator< (const file& other) const
        {
            return startPos < other.startPos;
        }
    };

    FileSystem(const std::string_view compressedSystem)
    {
        uint64_t currPos = 0;
        for (uint64_t i = 0; i < std::size(compressedSystem); ++i)
        {
            uint64_t endPos = currPos + std::stol(std::string({ compressedSystem[i] }));
            if (i % 2 == 0)
            {
                m_fileSet.insert(file(
                    i / 2,
                    currPos,
                    endPos
                ));
            }
            else if(currPos < endPos)
            {
                m_freeSpace[currPos] = endPos;
            }
            currPos = endPos;
        }
        m_freeSpace[currPos] = std::numeric_limits<uint64_t>::max();

    }

    void compress()
    {
        auto itrTo = m_freeSpace.begin();
        auto itrFrom = --m_fileSet.end();

        while (itrTo->first < itrFrom->startPos)
        {
            realocFile(*itrFrom, itrTo);

            itrTo = m_freeSpace.begin();
            itrFrom = --m_fileSet.end();
        }
    }

    void compressWholeFile()
    {

        const auto fileSetBefore = m_fileSet;
        for (auto itrFrom = fileSetBefore.rbegin(); itrFrom != fileSetBefore.rend(); ++ itrFrom)
        {

            auto itrTo = m_freeSpace.begin();
            while (itrTo != m_freeSpace.end() 
                && itrTo->first < itrFrom->startPos 
                && (itrTo->second - itrTo->first < itrFrom->endPos - itrFrom->startPos)
                )
                ++itrTo;

            if(itrTo != m_freeSpace.end()
                && itrTo->first < itrFrom->startPos
                && (itrTo->second - itrTo->first >= itrFrom->endPos - itrFrom->startPos)
                )
                realocFile(*itrFrom, itrTo);
        }
    }



    int64_t calcChecksum() const
    {
        uint64_t sum = 0;
        for (const auto& file : m_fileSet)
        {
            sum += file.id * (file.endPos + file.startPos - 1) * (file.endPos - file.startPos) / 2;
        }
        return sum;
    }


private:

    uint64_t getLastEnd(const std::set<file>::iterator& itr) const
    {
        uint64_t lastFileEnd = 0;

        if (itr != m_fileSet.begin())
        {
            auto tempItr = itr;
            lastFileEnd = (--tempItr)->endPos;
        }
        return lastFileEnd;
    }


    uint64_t realocFile(file f, const  std::map<uint64_t, uint64_t>::iterator& itrTo)
    {
        auto leftPos = itrTo->first;
        auto rightPos = itrTo->second;

        auto sizeToMove = std::min(rightPos - leftPos, f.endPos - f.startPos);

        m_fileSet.insert(file(
           f.id,
           leftPos,
           leftPos + sizeToMove
            ));

        m_fileSet.erase(f);
        if (f.endPos - f.startPos > sizeToMove)
            m_fileSet.insert(file(
                f.id,
                f.startPos,
                f.endPos - sizeToMove
            ));

        m_freeSpace.erase(itrTo);

        if(leftPos + sizeToMove != rightPos)
            m_freeSpace[leftPos + sizeToMove] = rightPos;

        return sizeToMove;
    }

    std::set<file> m_fileSet;
    std::map<uint64_t, uint64_t> m_freeSpace;
};




int64_t prob1(std::string inputFile)
{
    std::ifstream file(inputFile);
    auto content = readFile(file);

    FileSystem fs(content[0]);
    fs.compress();

    return fs.calcChecksum();
}


int64_t prob2(std::string inputFile)
{
    std::ifstream file(inputFile);
    auto content = readFile(file);
    FileSystem fs(content[0]);
    fs.compressWholeFile();

    return fs.calcChecksum();
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
