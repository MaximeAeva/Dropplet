#include <display.hpp>

Display::Display(int height = 100, int width = 100, std::vector<std::string> title = {})
{
    this->height = height;
    this->width = width;
    this->title = title;
    for(int i = 0; i < title.size(); i++) this->color.push_front(rand() % 14 + 1);
    if (system("CLS")) system("clear");
}

Display::~Display()
{

}

void Display::show(Matrix m, int clingTime)
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
    this->color.pop_back();
    this->color.push_front(rand() % 14 + 1);
    for(int i = 0; i<this->title.size(); i++) 
    {
        std::cout << "|";
        SetConsoleTextAttribute(hConsole, color[i]);
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
    for(int raw = 0; raw < this->height; raw++)
    {
        std::cout << "|";
        for(int col = 0; col < this->width; col++)
            {
                if(m.mat[raw][col]) SetConsoleTextAttribute(hConsole, 30);
                std::cout << " ";
                SetConsoleTextAttribute(hConsole, 15);
            }
        std::cout << "|" << std::endl;
    }
    std::cout << "+";
    for(int col = 0; col < this->width; col++)
    {
        std::cout << "=";
    }
    std::cout << "+" << std::endl;
    SetConsoleTextAttribute(hConsole, 15);
    Sleep(clingTime);
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
