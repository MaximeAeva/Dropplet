#include <iostream>
#include <display.hpp>
#include <reader.hpp>

int main()
{
    Reader r("Hello World");
    Display d(r.titleSize.height, r.titleSize.width, r.text);
    d.show();
    r.~Reader();
    d.~Display();
    return 0;
}