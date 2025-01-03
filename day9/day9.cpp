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
    };

    FileSystem(const std::string_view compressedSystem)
    {
        uint64_t currPos = 0;
        for (uint64_t i = 0; i < std::size(compressedSystem); ++i)
        {
            uint64_t endPos = currPos + std::stol(std::string({ compressedSystem[i] }));
            if (i % 2 == 0)
            {
                m_fileList.emplace_back(
                    i / 2,
                    currPos,
                    endPos
                );
            }
            currPos = endPos;
        }
    }

    void compress()
    {
        auto itrTo = m_fileList.begin();
        auto itrFrom = --m_fileList.end();

        while (itrTo->startPos < itrFrom->startPos)
        {
            while (itrTo->startPos <= itrFrom->startPos && itrTo->startPos - getLastEnd(itrTo) == 0)
            {
                ++itrTo;
            }
            realocFile(itrFrom, itrTo);

            if (itrFrom == itrTo && itrFrom->endPos == itrFrom->startPos)
            {
                m_fileList.pop_back();
                break;
            }
            else if (itrFrom->endPos == itrFrom->startPos)
            {
                m_fileList.pop_back();
                itrFrom = --m_fileList.end();
            }
        }
    }

    int64_t calcChecksum() const
    {
        uint64_t sum = 0;
        for (const auto& file : m_fileList)
        {
            sum += file.id * (file.endPos + file.startPos - 1) * (file.endPos - file.startPos) / 2;
        }
        return sum;
    }


private:

    uint64_t getLastEnd(const std::list<file>::iterator& itr) const
    {
        uint64_t lastFileEnd = 0;

        if (itr != m_fileList.begin())
        {
            auto tempItr = itr;
            lastFileEnd = (--tempItr)->endPos;
        }
        return lastFileEnd;
    }


    void realocFile(const std::list<file>::iterator& itrFrom, const std::list<file>::iterator& itrTo)
    {
        auto leftPos = getLastEnd(itrTo);
        auto sizeToMove = std::min(itrTo->startPos - leftPos, itrFrom->endPos - itrFrom->startPos);
        itrFrom->endPos -= sizeToMove;
        m_fileList.insert(itrTo, {
            itrFrom->id,
           leftPos,
           leftPos + sizeToMove
            });
    }


    std::list<file> m_fileList;

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
