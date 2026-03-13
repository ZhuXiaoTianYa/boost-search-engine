#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <unordered_map>
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
            return nullptr;
        }

        InvertedList *GetInvertedList(const std::string &word)
        {
            return nullptr;
        }

        bool BuildIndex(const std::string &input)
        {
            return true;
        }
    };
}