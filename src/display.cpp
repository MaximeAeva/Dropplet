#include <display.hpp>

Display::Display(int height = 100, int width = 100, std::vector<std::string> title = {})
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
        std::cout << "=";
    }
    std::cout << "+" << std::endl;
    
    for(int i = 0; i<this->title.size(); i++) 
    {
        int k = rand() % 14 + 1; 
        std::cout << "|";
        SetConsoleTextAttribute(hConsole, k);
        std::cout << this->title[i];
        SetConsoleTextAttribute(hConsole, 15);
        std::cout << "|" << std::endl;
    }
    
    std::cout << "+";
    for(int col = 0; col < this->width; col++)
    {
        std::cout << "=";
    }
    std::cout << "+" << std::endl;
    SetConsoleTextAttribute(hConsole, 15);
}
