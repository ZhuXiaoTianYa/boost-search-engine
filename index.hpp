#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <unordered_map>
#include <fstream>
#include <cstdint>
#include <mutex>
#include "util.hpp"

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

    private:
        Index() {}
        Index(const Index &) = delete;
        Index &operator=(const Index &) = delete;

    private:
        // static Index *instance;
        static std::mutex mtx;

    public:
        ~Index() {}

        // static Index *GetInstance()
        // {
        //     std::cout << "1" << std::endl;
        //     if (instance == nullptr)
        //     {
        //         std::cout << "2" << std::endl;
        //         mtx.lock();
        //         std::cout << "3" << std::endl;
        //         if (instance == nullptr)
        //         {
        //             std::cout << "4" << std::endl;
        //             instance = new Index();
        //             std::cout << "5" << std::endl;
        //         }
        //         mtx.unlock();
        //     }
        //     std::cout << "6" << std::endl;
        //     return instance;
        // }
        static Index *GetInstance()
        {
            static Index instance;
            return &instance;
        }

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
            int count = 0;
            while (std::getline(in, line))
            {
                DocInfo *doc = BuildForwardIndex(line);
                if (doc == nullptr)
                {
                    std::cerr << "build " << line << " error" << std::endl;
                    continue;
                }
                count++;
                BuildInvertedIndex(*doc);
                if (count % 50 == 0)
                {
                    std::cout << "正在索引文件: " << count << std::endl;
                }
            }
            return true;
        }

        DocInfo *BuildForwardIndex(const std::string &line)
        {
            std::vector<std::string> results;
            const std::string sep = "\3";
            ns_util::StringUtil::CutString(line, &results, sep);
            if (results.size() != 3)
            {
                return nullptr;
            }
            DocInfo doc;
            doc.title = results[0];
            doc.content = results[1];
            doc.url = results[2];
            doc.doc_id = forward_index.size();
            forward_index.push_back(std::move(doc));
            return &forward_index.back();
        }

        bool BuildInvertedIndex(const DocInfo doc)
        {
            struct word_cnt
            {
                int title_cnt;
                int content_cnt;
            };
            std::vector<std::string> title_word;
            ns_util::JiebaUtil::CutString(doc.title, &title_word);
            std::unordered_map<std::string, word_cnt> word_map;
            for (std::string word : title_word)
            {
                boost::to_lower(word);
                word_map[word].title_cnt++;
            }
            std::vector<std::string> content_word;
            ns_util::JiebaUtil::CutString(doc.content, &content_word);
            for (std::string word : content_word)
            {
                boost::to_lower(word);
                word_map[word].content_cnt++;
            }
#define TITLE_WEIGHT 10
#define CONTENT_WEIGHT 1
            for (auto &word_pair : word_map)
            {
                InvertedElem elem;
                elem.doc_id = doc.doc_id;
                elem.weight = TITLE_WEIGHT * word_pair.second.title_cnt + CONTENT_WEIGHT * word_pair.second.content_cnt;
                elem.word = word_pair.first;
                InvertedList &inverted_list = inverted_index[word_pair.first];
                inverted_list.push_back(std::move(elem));
            }
            return true;
        }
    };
    std::mutex Index::mtx;
    // Index *Index::instance = nullptr;
}