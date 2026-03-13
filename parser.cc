#include <iostream>
#include <unistd.h>
#include <vector>
#include <string>
#include <boost/filesystem.hpp>
#include "util.hpp"

const std::string src_path = "./data/input";
const std::string out_path = "./data/raw_html/raw.bin";

struct DocInfo_t
{
    std::string title;
    std::string content;
    std::string url;
};

bool EnumFile(const std::string &src_path, std::vector<std::string> *file_list);
bool ParseHtml(const std::vector<std::string> &file_list, std::vector<DocInfo_t> *results);
bool SaveHtml(const std::vector<DocInfo_t> &results, const std::string &output);

static bool ParseTitle(const std::string &result, std::string *title);
static bool ParseContent(const std::string &result, std::string *content);
static bool ParseUrl(const std::string &file_path, std::string *out);

// 输入: const &
// 输出： *
// 输入输出：&

// debug
static void show(const DocInfo_t &doc)
{
    std::cout << "title: " << doc.title << std::endl;
    std::cout << "content: " << doc.content << std::endl;
    std::cout << "url: " << doc.url << std::endl;
}

int main()
{
    std::vector<std::string> file_list;

    if (!EnumFile(src_path, &file_list))
    {
        std::cerr << "enum file error" << std::endl;
        return 1;
    }

    std::vector<DocInfo_t> results;
    if (!ParseHtml(file_list, &results))
    {
        std::cerr << "parse html error" << std::endl;
        return 2;
    }

    if (!SaveHtml(results, out_path))
    {
        std::cerr << "save html error" << std::endl;
        return 3;
    }

    return 0;
}

bool EnumFile(const std::string &src_path, std::vector<std::string> *file_list)
{
    namespace fs = boost::filesystem;
    fs::path root_path(src_path);
    fs::recursive_directory_iterator end;
    if (!fs::exists(root_path))
    {
        std::cerr << src_path << " is not exists" << std::endl;
        return false;
    }
    for (fs::recursive_directory_iterator iter(root_path); iter != end; iter++)
    {
        if (!fs::is_regular_file(*iter))
            continue;
        if (iter->path().extension() != ".html")
            continue;
        std::cout << iter->path().string() << std::endl;
        file_list->push_back(iter->path().string());
    }
    return true;
}

bool ParseHtml(const std::vector<std::string> &file_list, std::vector<DocInfo_t> *results)
{
    for (const std::string &file : file_list)
    {
        std::string result;
        if (!ns_util::FileUtil::ReadFile(file, &result))
        {
            continue;
        }
        DocInfo_t doc;
        if (!ParseTitle(result, &doc.title))
        {
            continue;
        }
        if (!ParseContent(result, &doc.content))
        {
            continue;
        }
        if (!ParseUrl(file, &doc.url))
        {
            continue;
        }
        // 走到这都是成功读取解析的
        results->push_back(std::move(doc));
        // show(doc);
        // break;
    }
    return true;
}

bool SaveHtml(const std::vector<DocInfo_t> &results, const std::string &output)
{
#define SEP '\3'
    std::ofstream out(out_path, std::ios::out | std::ios::binary);
    if (!out.is_open())
    {
        std::cerr << "open " << out_path << " failed!" << std::endl;
        return false;
    }
    for (auto &iter : results)
    {
        out << iter.title << SEP << iter.content << SEP << iter.url << '\n';
    }
    out.close();
    return true;
}

static bool ParseTitle(const std::string &result, std::string *title)
{
    size_t begin = result.find("<title>");
    if (begin == std::string::npos)
    {
        return false;
    }
    size_t end = result.find("</title>");
    if (end == std::string::npos)
    {
        return false;
    }
    if (begin > end)
    {
        return false;
    }
    begin += std::string("<title>").size();
    *title = result.substr(begin, end - begin);
    return true;
}

static bool ParseContent(const std::string &result, std::string *content)
{
    // 去标签，基于一个简易的状态机
    enum status
    {
        LABLE,
        CONTENT
    };

    enum status s = LABLE;

    for (char c : result)
    {
        switch (s)
        {
        case LABLE:
            if (c == '>')
                s = CONTENT;
            break;
        case CONTENT:
            if (c == '<')
                s = LABLE;
            else
            {
                if (c == '\n')
                    c = ' ';
                content->push_back(c);
            }
            break;
        default:
            break;
        }
    }
    return true;
}

static bool ParseUrl(const std::string &file_path, std::string *url)
{
    std::string url_head = "https://www.boost.org/doc/libs/1_90_0/doc/html";
    std::string url_tail = file_path.substr(src_path.size());
    *url = url_head + url_tail;
    return true;
}
