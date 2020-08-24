#include <iostream>
#include <time.h>
#include <display.hpp>
#include <reader.hpp>
#include <matrix.hpp>

int main()
{
    srand(time(NULL));
    Coord drop;
    Reader r("The Dropplet Project");
    Matrix m(25, r.titleSize.width, drop);
    Display d(25, r.titleSize.width, r.text);
    while(1) d.show(m);
    m.~Matrix();
    r.~Reader();
    d.~Display();
    return 0;
}