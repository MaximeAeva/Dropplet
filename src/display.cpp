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
    std::cout << "+";
    for(int col = 0; col < this->width; col++)
    {
        std::cout << "-";
    }
    std::cout << "+" << std::endl;
    std::cout << '\t' << this->title;
    std::cout << "+";
    for(int col = 0; col < this->width; col++)
    {
        std::cout << "-";
    }
    std::cout << "+" << std::endl;
    SetConsoleTextAttribute(hConsole, 15);
}
