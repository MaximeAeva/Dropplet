#include <iostream>
#include <time.h>
#include <display.hpp>
#include <reader.hpp>
#include <matrix.hpp>

int main()
{
    int height = 29;
    int waterLvl = 2;
    srand(time(NULL));
    Coord drop;
    Reader r("Dada");
    drop.raw = 0;
    drop.col = floor(r.titleSize.width/2);
    Matrix m(height, r.titleSize.width, drop, height-waterLvl);
    int nj = m.njMax(2, waterLvl, height-drop.raw-1);
    Display d(height, r.titleSize.width, r.text);
    bool t = true;
    while(1)
    {
        d.show(m, nj, 0);//Refresh time can be limit
        m.animate(0, t);
        if(t) t = false;
        else t = true;
    }

    m.~Matrix();
    r.~Reader();
    d.~Display();
    return 0;
}