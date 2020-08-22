#include <display.hpp>

Display::Display(int height = 100, int width = 100, std::vector<std::string> title = {})
{
    this->height = height;
    this->width = width;
    this->title = title;
    for(int i = 0; i < title.size(); i++) this->color.push(rand() % 14 + 1);
    if (system("CLS")) system("clear");
}

Display::~Display()
{

}

void Display::show()
{
    std::cout << "\033[2J";
    gotoxy(0, 0);
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    std::cout << "+";
    for(int col = 0; col < this->width; col++)
    {
        std::cout << "=";
    }
    std::cout << "+" << std::endl;
    
    for(int i = 0; i<this->title.size(); i++) 
    {
        this->color.pop();
        this->color.push(rand() % 14 + 1); 
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
    Sleep(500);
}

void Display::gotoxy( int column, int line )
{
COORD coord;
coord.X = column;
coord.Y = line;
SetConsoleCursorPosition(
GetStdHandle( STD_OUTPUT_HANDLE ),
coord
);
}
