#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

#include "src/Parser.h"

using HTMLDecorators::IdMap;
using HTMLDecorators::Parser;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::unordered_map;

void printDef(HTMLDecorators::DecoratorDef def) {
    cout << "-------" << endl;
    cout << "Id=" << def.Id << endl;
    cout << "Name=" << def.Name << endl;
    for(auto pair : def.Params) {
        cout << "Key=" << pair.first << ", Value=" << pair.second << endl;
    }
}

void printAllDefs(vector<HTMLDecorators::DecoratorDef> defs) {
    for(auto def : defs) {
        printDef(def);
    }
}

int main() {

    std::ifstream t("testdata/test1.html");
    std::string str((std::istreambuf_iterator<char>(t)),
                    std::istreambuf_iterator<char>());

    std::ifstream t2("testdata/test2.html");
    std::string str2((std::istreambuf_iterator<char>(t2)),
                    std::istreambuf_iterator<char>());

    std::ifstream t3("testdata/test3.html");
    std::string str3((std::istreambuf_iterator<char>(t3)),
                     std::istreambuf_iterator<char>());

    std::ifstream t4("testdata/test4.html");
    std::string str4((std::istreambuf_iterator<char>(t4)),
                     std::istreambuf_iterator<char>());

    std::ifstream t5("testdata/test5.html");
    std::string str5((std::istreambuf_iterator<char>(t5)),
                     std::istreambuf_iterator<char>());

    IdMap GenIds {};

    Parser parser = Parser{GenIds};

    // test 1
    //string html = parser.Parse(str);
    //cout << html << endl;
    //cout << parser.DecoratorList.size() << endl;

    // test 2
    //string html2 = parser.Parse(str2);
    //cout << html2 << endl;
    //cout << parser.DecoratorList.size() << endl;
    //printAllDefs(parser.DecoratorList);

    // test 3
    //string html3 = parser.Parse(str3);
    //cout << html << endl;
    //cout << parser.DecoratorList.size() << endl;
    //printAllDefs(parser.DecoratorList);

    unordered_map<string,string> data {};
    data.insert({"value1","Value 1"});
    data.insert({"headline","Headline"});
    data.insert({"underlined","is underlined"});
    data.insert({"test","Test 1"});
    data.insert({"test2","Test 2"});
    data.insert({"test3","Test 3"});
    data.insert({"test4","Test 4"});

    // test 4
    //string html4 = parser.Parse(str4, data);
    //cout << html4 << endl;
    //cout << parser.DecoratorList.size() << endl;
    //printAllDefs(parser.DecoratorList);

    // test 5
    //string html5 = parser.Parse(str5, data);
    //cout << html5 << endl;
    //cout << parser.DecoratorList.size() << endl;

    return 0;
}
