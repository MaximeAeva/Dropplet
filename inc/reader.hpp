#ifndef READER_H
#define READER_H

#include <fstream>
#include <string>
#include <iostream>

class Reader {
    public:
        Reader();
        ~Reader();

    private:
        std::ifstream inFile;
        std::string text;
};

#endif