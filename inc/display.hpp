#ifndef DISPLAY_H
#define DISPLAY_H

#include <iostream>
#include <string>
#include <vector>
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
        std::vector<std::string> title;

};

#endif