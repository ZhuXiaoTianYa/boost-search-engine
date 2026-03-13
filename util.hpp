#pragma once
#include <iostream>
#include <string>
#include <fstream>

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
}