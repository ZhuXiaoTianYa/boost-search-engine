#include "../include/searcher/searcher.hpp"
#include "../include/httplib/httplib.h"

const std::string root_path = "wwwroot";
const std::string input_path = "data/raw_html/raw.bin";

int main(int argc, char *argv[], char *env[])
{
    if (argc != 2)
    {
        std::cerr << "Usage:\n"
                  << "./http_server port" << std::endl;
        return -1;
    }
    ns_searcher::Searcher seach;
    seach.InitSearcher(input_path);
    httplib::Server svr;
    svr.set_base_dir(root_path);
    svr.Get("/s", [&seach](const httplib::Request &req, httplib::Response &rsp)
            { 
                if(!req.has_param("word"))
                {
                    rsp.set_content("请输入搜索关键词！", "text/plain; charset=utf-8");
                    return;
                }
                std::string word = req.get_param_value("word");
                std::string json_string;
                seach.Search(word, &json_string);
                rsp.set_content(json_string,"application/json"); });
    svr.listen("0.0.0.0", std::stoi(argv[1]));
    return 0;
}