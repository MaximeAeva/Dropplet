#include <iostream>
#include <time.h>
#include <display.hpp>
#include <reader.hpp>

int main()
{
    srand(time(NULL));
    Reader r("The Dropplet Project");
    Display d(r.titleSize.height, r.titleSize.width, r.text);
    d.show();
    r.~Reader();
    d.~Display();
    return 0;
}