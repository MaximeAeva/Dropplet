#ifndef READER_H
#define READER_H

#include <fstream>
#include <string>
#include <vector>
#include <iostream>

struct coord 
{
    int height = 8;
    int width = 0;
};

class Reader 
{
    public:
        Reader(std::string word);
        ~Reader();
        std::string text;
        coord titleSize;

    private:
        void spliter(std::string word);
        void search(bool lcase, int letter, int line, bool end);
        
        std::ifstream inFile;
        
};

#endif