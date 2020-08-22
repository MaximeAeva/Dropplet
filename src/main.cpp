#include <iostream>
#include <time.h>
#include <display.hpp>
#include <reader.hpp>

int main()
{
    srand(time(NULL));
    Reader r("Maxime le zonzon");
    Display d(r.titleSize.height, r.titleSize.width, r.text);
    while(1) d.show();
    r.~Reader();
    d.~Display();
    return 0;
}