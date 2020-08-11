#ifndef DISPLAY_H
#define DISPLAY_H

#include <iostream>
#include <string>
#include <windows.h>

class Display{

    public:
        Display(int h, int w, std::string t);
        ~Display();
        void show();

    private:
        int height;
        int width;
        std::string title;

};

#endif