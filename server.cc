#include "searcher.hpp"

const std::string raw_html = "data/raw_html/raw.bin";

int main()
{
    ns_searcher::Searcher *search;
    search->InitSearcher(raw_html);
    while (true)
    {
        std::cout << "cin query# ";
        std::string query;
        std::string json_string;
        std::cin >> query;
        search->Search(query, &json_string);
        std::cout << json_string << std::endl;
    }
    return 0;
}