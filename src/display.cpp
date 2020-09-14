#include <display.hpp>

Display::Display(int height = 100, int width = 100, std::vector<std::string> title = {})
{
    this->height = height;
    this->width = width;
    this->title = title;
    for(int i = 0; i < int(title.size()); i++) this->color.push_front(rand() % 14 + 1);
    if (system("CLS")) system("clear");
}

Display::~Display()
{

}

void Display::show(Matrix m, int nj, int clingTime)
{
    int border = 15;
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
    for(int i = 0; i<int(this->title.size()); i++) 
    {
        SetConsoleTextAttribute(hConsole, border);
        std::cout << "|";
        SetConsoleTextAttribute(hConsole, color[i]);
        std::cout << this->title[i];
        SetConsoleTextAttribute(hConsole, border);
        std::cout << "|" << std::endl;
        SetConsoleTextAttribute(hConsole, 15);
    }
    int perc;
    if(nj) perc = 100*m.totalStrenght()/nj;
    else perc = m.totalStrenght();
    if(perc >= 1000) perc = 999;
    if(perc <= 100) SetConsoleTextAttribute(hConsole, 11);
    else SetConsoleTextAttribute(hConsole, 12);
    std::cout << perc << "%";
    if(perc > 100) perc = 100;
    SetConsoleTextAttribute(hConsole, 15);
    int first = 1;
    if(perc >= 10)
    {
        if(perc >=100) first += 2;
        else first += 1;
    }
    int step = perc * this->width / 100;
    for(int col = first; col < this->width; col++)
    {
        if(col < step) 
        {
            SetConsoleTextAttribute(hConsole, 3);
            std::cout << "=";
        }
        else if(col == step)
        {
            SetConsoleTextAttribute(hConsole, 3);
            std::cout << ">";
        }
        else 
        {
            SetConsoleTextAttribute(hConsole, 8);
            std::cout << "-";
        }
    }
    SetConsoleTextAttribute(hConsole, 15);
    std::cout << "+" << std::endl;
    for(int raw = 0; raw < this->height; raw++)
    {
        SetConsoleTextAttribute(hConsole, border);
        std::cout << "|";
        SetConsoleTextAttribute(hConsole, 15);
        for(int col = 0; col < this->width; col++)
        {
            if(m.mat[raw][col].drop) 
            {
                int s = int(m.mat[raw][col].strenght());
                switch(s)
                {
                    case 0:
                        SetConsoleTextAttribute(hConsole, 150);
                    break;
                    case 1:
                        SetConsoleTextAttribute(hConsole, 180);
                    break;
                    case 2:
                        SetConsoleTextAttribute(hConsole, 160);
                    break;
                    case 3:
                        SetConsoleTextAttribute(hConsole, 230);
                    break;
                    case 4:
                        SetConsoleTextAttribute(hConsole, 210);
                    break;
                    case 5:
                        SetConsoleTextAttribute(hConsole, 200);
                    break;
                    default:
                        SetConsoleTextAttribute(hConsole, 240);
                    break;
                }
                std::cout << " ";
                SetConsoleTextAttribute(hConsole, 15);
            }
            else
            {
                std::cout << " ";
            }  
        }
        SetConsoleTextAttribute(hConsole, border);
        std::cout << "|" << std::endl;
        SetConsoleTextAttribute(hConsole, 15);
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
SetConsoleCursorPosition(GetStdHandle( STD_OUTPUT_HANDLE ),coord);
}
