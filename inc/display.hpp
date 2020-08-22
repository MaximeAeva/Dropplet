#ifndef DISPLAY_H
#define DISPLAY_H

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <windows.h>
#include <cstdlib>

class Display{

    public:
        Display(int h, int w, std::vector<std::string> t);
        ~Display();
        void show();

    private:
        int height;
        int width;
        std::queue<int> color;
        std::vector<std::string> title;
        void gotoxy( int column, int line );

};

#endif