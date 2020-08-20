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
        std::vector<int> youMean(std::string word);


    private:
        std::ifstream inFile;
        std::string text;
};

#endif