#include <iostream>
#include <time.h>
#include <display.hpp>
#include <reader.hpp>
#include <matrix.hpp>

int main()
{
    int height = 25;
    srand(time(NULL));
    Coord drop;
    Reader r("The Dropplet Project");
    drop.raw = 5;
    drop.col = int(r.titleSize.width/2);
    Matrix m(height, r.titleSize.width, drop, height-5);
    Display d(height, r.titleSize.width, r.text);
    int i = 0;
    while(i<100)
    {
        d.show(m, 100);
        i++;
        m.animate(50);
    }

    m.~Matrix();
    r.~Reader();
    d.~Display();
    return 0;
}