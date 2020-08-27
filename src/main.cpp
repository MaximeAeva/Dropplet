#include <iostream>
#include <time.h>
#include <display.hpp>
#include <reader.hpp>
#include <matrix.hpp>

int main()
{
    int height = 29;
    srand(time(NULL));
    Coord drop;
    Reader r("Welcome to my Github");
    drop.raw = 3;
    drop.col = int(r.titleSize.width/2);
    Matrix m(height, r.titleSize.width, drop, height-10);
    Display d(height, r.titleSize.width, r.text);
    int i = 0;
    while(1)
    {
        d.show(m, 10);
        i++;
        m.animate(0);
    }

    m.~Matrix();
    r.~Reader();
    d.~Display();
    return 0;
}