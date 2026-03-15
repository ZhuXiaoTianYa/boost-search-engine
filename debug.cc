#include "searcher.hpp"

const std::string input = "data/raw_html/raw.bin";

int main()
{
    ns_searcher::Searcher search;
    search.InitSearcher(input);
    while (true)
    {
        std::cout << "Please Enter You Search Query# ";
        std::string query;
        std::string json_string;
        std::cin >> query;
        search.Search(query, &json_string);
        std::cout << json_string << std::endl;
    }
    return 0;
}