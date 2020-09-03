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
    Reader r("hello world");
    drop.raw = 0;
    drop.col = floor(r.titleSize.width/2);
    Matrix m(height, r.titleSize.width, drop, height-5);
    Display d(height, r.titleSize.width, r.text);
    bool t = true;
    while(1)
    {
        d.show(m, 0);//Refresh time can be limit
        m.animate(0, t);
        if(t) t = false;
        else t = true;
    }

    m.~Matrix();
    r.~Reader();
    d.~Display();
    return 0;
}