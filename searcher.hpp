#pragma once
#include <algorithm>
#include <jsoncpp/json/json.h>
#include "index.hpp"

namespace ns_searcher
{
    class Searcher
    {
    private:
        ns_index::Index *index;

    public:
        Searcher() {}
        ~Searcher() {}

    public:
        void InitSearcher(const std::string &input)
        {
            std::cout << "获取index单例..." << std::endl;
            index = ns_index::Index::GetInstance();
            std::cout << "7" << std::endl;
            if (index == nullptr)
            {
                std::cout << "8" << std::endl;
            }
            index->BuildIndex(input);
            std::cout << "构建索引..." << std::endl;
        }

        void Search(const std::string &query, std::string *json_string)
        {
            std::vector<std::string> words;
            ns_util::JiebaUtil::CutString(query, &words);
            ns_index::InvertedList inverted_list_all;
            for (std::string word : words)
            {
                boost::to_lower(word);
                ns_index::InvertedList *inverted_list = index->GetInvertedList(word);
                if (inverted_list == nullptr)
                {
                    continue;
                }
                inverted_list_all.insert(inverted_list_all.end(), inverted_list->begin(), inverted_list->end());
            }
            std::sort(inverted_list_all.begin(), inverted_list_all.end(), [](const ns_index::InvertedElem &e1, const ns_index::InvertedElem &e2)
                      { return e1.weight > e2.weight; });

            Json::Value root;
            for (auto &item : inverted_list_all)
            {
                ns_index::DocInfo *doc = index->GetforwardIndex(item.doc_id);
                if (doc == nullptr)
                {
                    continue;
                }
                Json::Value elem;
                elem["title"] = doc->title;
                elem["desc"] = GetDesc(doc->content, item.word);
                elem["url"] = doc->url;

                // elem["id"] = doc->doc_id;
                // elem["weight"] = item.weight;
                root.append(elem);
            }
            Json::StyledWriter writer;
            *json_string = writer.write(root);
            return;
        }

        std::string GetDesc(const std::string &html_content, const std::string &word)
        {
            const size_t prev_step = 50;
            const size_t next_step = 100;
            int start = 0;
            int end = html_content.size() - 1;
            auto iter = std::search(html_content.begin(), html_content.end(), word.begin(), word.end(), [](const int &a, const int &b){ return std::tolower(a) == std::tolower(b); });
            if (iter == html_content.end())
            {
                return "None1";
            }
            int pos = std::distance(html_content.begin(), iter);

            if (pos > start + prev_step)
                start = pos - prev_step;

            if (pos + next_step < end)
                end = pos + next_step;

            if (start >= end)
            {
                return "None2";
            }
            return html_content.substr(start, end - start);
        }
    };
}