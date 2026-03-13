#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <unordered_map>
#include <fstream>
#include <cstdint>

namespace ns_index
{
    struct DocInfo
    {
        std::string title;
        std::string content;
        std::string url;
        uint64_t doc_id;
    };

    struct InvertedElem
    {
        uint64_t doc_id;
        std::string word;
        int weight;
    };

    // 倒排拉链
    typedef std::vector<InvertedElem> InvertedList;

    class Index
    {
    private:
        // 数组下标就是天然的doc_id;
        std::vector<DocInfo> forward_index;
        std::unordered_map<std::string, InvertedList> inverted_index;

    public:
        Index() {}
        ~Index() {}
        DocInfo *GetforwardIndex(const uint64_t &doc_id)
        {
            if (doc_id >= forward_index.size())
            {
                std::cerr << "doc id out range, error" << std::endl;
                return nullptr;
            }
            return &forward_index[doc_id];
        }

        InvertedList *GetInvertedList(const std::string &word)
        {
            if (inverted_index.find(word) == inverted_index.end())
            {
                std::cerr << word << " have no InvertedList" << std::endl;
                return nullptr;
            }
            return &inverted_index[word];
        }

        bool BuildIndex(const std::string &input)
        {
            std::ifstream in(input, std::ios::in | std::ios::binary);
            if (!in.is_open())
            {
                std::cerr << "open file " << input << " failed" << std::endl;
                return false;
            }
            std::string line;
            while (std::getline(in, line))
            {
                DocInfo *doc = BuildForwardIndex(line);
                if (doc == nullptr)
                {
                    std::cerr << "build " << line << " error" << std::endl;
                    continue;
                }
                BuildInvertedIndex(*doc);
            }
            return true;
        }

        DocInfo *BuildForwardIndex(const std::string &line)
        {
        }

        bool BuildInvertedIndex(const DocInfo doc)
        {
        }
    };
}