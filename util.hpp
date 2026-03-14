#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include "include/cppjieba/Jieba.hpp"

namespace ns_util
{
    class FileUtil
    {
    public:
        static bool ReadFile(const std::string &file_path, std::string *out)
        {
            std::ifstream is;
            is.open(file_path, std::ios::in);
            if (!is.is_open())
            {
                std::cerr << "open file " << file_path << " error" << std::endl;
                return false;
            }
            std::string line;
            while (std::getline(is, line))
            {
                *out += line;
            }
            is.close();
            return true;
        }
    };

    class StringUtil
    {
    public:
        static void CutString(const std::string &target, std::vector<std::string> *out, const std::string &sep)
        {
            boost::split(*out, target, boost::is_any_of(sep), boost::token_compress_on);
            return;
        }
    };

    const char *const DICT_PATH = "./dict/jieba.dict.utf8";
    const char *const HMM_PATH = "./dict/hmm_model.utf8";
    const char *const USER_DICT_PATH = "./dict/user.dict.utf8";
    const char *const IDF_PATH = "./dict/idf.utf8";
    const char *const STOP_WORD_PATH = "./dict/stop_words.utf8";

    class JiebaUtil
    {
    private:
        static cppjieba::Jieba jieba;

    public:
        static void CutString(const std::string &str, std::vector<std::string> *out)
        {
            jieba.CutForSearch(str, *out);
        }
    };
    cppjieba::Jieba JiebaUtil::jieba(DICT_PATH, HMM_PATH, USER_DICT_PATH, IDF_PATH, STOP_WORD_PATH);
}