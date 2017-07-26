#include <fstream>
#include <ostream>
#include <set>
#include <algorithm>
#include <array>
#include <mutex>
#include <string>
#include <thread>
#include <iostream>
#include <deque>
#include <vector>
#include <cassert>

const size_t BLOCK_SIZE =  1024 * 1024; ;
const unsigned int THREAD_NUM = 4;
std::ifstream input;
std::mutex mutRead;
std::mutex mutNumWrite;

template<class T>
void sortAndSplitBlocks(std::deque<std::string> & files)
{
    static unsigned int blockNum = 0; //current block of data number
    bool endOfFileReached = false;
    std::array<T, BLOCK_SIZE> blockData;
    int symbolsRead = BLOCK_SIZE;
    std::string outFile; //file for writing each block;
    while (symbolsRead == BLOCK_SIZE) //while end of file not reached
    {
        symbolsRead = 0;
        mutRead.lock();
        while ((symbolsRead < BLOCK_SIZE) && (input >> blockData[symbolsRead]))
            symbolsRead++;
        mutRead.unlock();
        if (symbolsRead == 0)
            return;
        std::sort(blockData.begin(), blockData.begin() + symbolsRead);
        outFile = "out" + std::to_string(blockNum++) + ".txt";
        mutNumWrite.lock();
        std::ofstream out(outFile);
        files.push_back(std::move(outFile));
        mutNumWrite.unlock();
        for (size_t i = 0; i < symbolsRead; i++)
            out << blockData[i] << " ";
    }
}

//k-way merge implementation
template<class T>
void kWayMerge(std::vector<std::ifstream> & fileDesriptors, std::ofstream && out)
{
    const unsigned int bufSize = 1024 * 1024;
    std::set<std::pair<T, int> > mergeSet;
    std::vector<std::deque<T>> inBuffer(fileDesriptors.size());
    T newObj;
    std::vector<T> outBuffer;
    for (int i = 0; i < fileDesriptors.size(); i++)
    {
        int symbolsRead = 0;
        while ((symbolsRead < bufSize) && (fileDesriptors[i] >> newObj))
            inBuffer[i].push_back(std::move(newObj));
    }
    for (int i = 0; i < inBuffer.size(); i++)
    {
        mergeSet.insert({ inBuffer[i][0], i });
        if (inBuffer.empty())
        {
            assert(false); //this cannot happen if algorithm works correct
            return;
        }
        inBuffer[i].pop_front();
    }

    while (!mergeSet.empty())
    {
        outBuffer.push_back(mergeSet.begin()->first);
        int bufToUpdate = mergeSet.begin()->second;
        mergeSet.erase(mergeSet.begin());
        if (!inBuffer[bufToUpdate].empty())
        {
            mergeSet.insert({ std::move(inBuffer[bufToUpdate][0]), bufToUpdate });
            inBuffer[bufToUpdate].pop_front();
        }
        if (inBuffer[bufToUpdate].empty())
        {
            int symbolsRead = 0;
            while ((symbolsRead < bufSize) && (fileDesriptors[bufToUpdate] >> newObj))
            {
                inBuffer[bufToUpdate].push_back(std::move(newObj));
            }
        }
        if (outBuffer.size() == bufSize)
        {
            for (const auto & i : outBuffer)
                out << i << " ";
            outBuffer.clear();
        }
    }
    for (const auto & i : outBuffer)
        out << i << " ";
    out.close();
}

//merge files
template<class T>
void merge(std::deque<std::string> & files)
{
    const unsigned int k = 16;
    unsigned int numMerged = 0; //num of file to print out buffer
    std::vector<std::ifstream> fileDesriptors;
    std::ofstream out;
    while (!files.empty())
    {
        fileDesriptors.clear();
        for (int i = 0; i < k && !files.empty(); i++)
        {
            fileDesriptors.emplace_back(files[0]);
            if (fileDesriptors[i].fail())
            {
                assert(false);
                std::cerr << "cannot open file " << files[0];
                return;
            }
            files.pop_front();
        }
        if (!files.empty())
        {
            std::string fileOut("outMerge" + std::to_string(numMerged++) + ".txt");
            out.open(fileOut);
            files.push_back(std::move(fileOut));
        }
        else
            out.open("sorted.txt");

        if (out.fail())
        {
            assert(false);
            std::cerr << "cannot create output file ";
            return;
        }

        kWayMerge<T>(fileDesriptors, std::move(out));
    }
}

int main()
{
    input.open("in.txt");
    if (input.fail())
    {
        std::cout << "fail";
        system("pause");
    }
    std::deque<std::string> files;
    std::array<std::thread, THREAD_NUM> threads;
    if (input.is_open())
    {
        for (int i = 0; i < THREAD_NUM; i++)
            threads[i] = std::thread(sortAndSplitBlocks<int>, std::ref(files));
        for (auto & i : threads)
            i.join();
    }
    merge<int>(files);
    return 0;
}