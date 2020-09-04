#ifndef DISPLAY_H
#define DISPLAY_H

#include <iostream>
#include <matrix.hpp>
#include <string>
#include <vector>
#include <deque>
#include <windows.h>
#include <cstdlib>

class Display{

    public:
        Display(int h, int w, std::vector<std::string> t);
        ~Display();
        void show(Matrix m, int nj, int clingTime = 500);

    private:
        int height;
        int width;
        std::deque<int> color;
        std::vector<std::string> title;
        void gotoxy( int column, int line );

};

#endif