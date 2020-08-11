#include <display.hpp>

Display::Display(int height = 100, int width = 100, std::string title = "DROPPLET")
{
    this->height = height;
    this->width = width;
    this->title = title;
}

Display::~Display()
{

}

void Display::show()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    for(int raw = 0; raw < this->height; raw++)
    {
        for(int col = 0; col < this->width; col++)
        {
            
        }
    }
    SetConsoleTextAttribute(hConsole, 15);
}
