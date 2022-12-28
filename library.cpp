#include "library.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include "rapidjson/document.h"
#include "SimpleHtmlDocTree.h"

using namespace rapidjson;

void read() {
    std::string line;
    std::cout << std::filesystem::current_path() <<std::endl;
    std::fstream file("../data/usk052/content.json");
    time_t stime = get_time();
    int count = 0;
    while (std::getline(file, line)) {
//        std::cout<<line.length()<<std::endl;
        Document document;
        document.Parse(line.c_str(), line.length());
        auto contents = document["contents"].GetArray();
        auto content = contents.begin()->GetObject();
        auto content_str = content["content"].GetString();
        SimpleHtmlDoc::parse_raw(content_str);
        line.clear();
//        break;
        count += 1;
        time_t etime = get_time();
        std::cout<<"cost:"<<etime-stime<<",count:"<<count<<",avg:"<<(etime-stime)/count<<std::endl;
    }
    time_t etime = get_time();
    std::cout<<"total, cost:"<<etime-stime<<",count:"<<count<<std::endl;
}
void hello() {
    auto json = R"( {
        "hello": "world",
        "t": true ,
        "f": false,
        "n": null,
        "i": 123,
        "pi": 3.1416,
        "a": [1, 2, 3, 4]
    } )";
    Document document;
    document.Parse(json);
    auto a = document["i"].GetInt();
//    double y = doc["y"]; // The cursor is now after the 2 (at })
//    double x = doc["x"]; // Success: [] loops back around to find "x"

    std::cout <<a<<"," << std::endl;
    std::cout << "Hello, World!" << std::endl;
}

int main() {
//    hello();
    read();
    return 0;
}
