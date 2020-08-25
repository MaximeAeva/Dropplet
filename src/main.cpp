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
    Reader r("You re in");
    drop.raw = 5;
    drop.col = int(r.titleSize.width/2);
    Matrix m(height, r.titleSize.width, drop, height-0);
    Display d(height, r.titleSize.width, r.text);
    int i = 0;
    while(i<30)
    {
        d.show(m, 100);
        i++;
        m.animate(0);
    }

    m.~Matrix();
    r.~Reader();
    d.~Display();
    return 0;
}