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
        


    private:
        void spliter(std::string word);
        void search(bool lcase, int letter, int line, bool end);
        
        std::ifstream inFile;
        std::string text;
};

#endif