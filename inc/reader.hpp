#ifndef READER_H
#define READER_H

#include <fstream>
#include <string>
#include <vector>
#include <iostream>

class Reader {
    public:
        Reader(std::string word);
        ~Reader();
        std::vector<int> spliter(std::string word);
        void search(bool lcase, int letter, int line, bool end);
        void selector(std::vector<int> word);


    private:
        std::ifstream inFile;
        std::string text;
};

#endif