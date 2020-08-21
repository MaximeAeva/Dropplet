#include <iostream>
#include <display.hpp>
#include <reader.hpp>

int main()
{
    Reader r("The Dropplet Project");
    Display d(r.titleSize.height, r.titleSize.width, r.text);
    std::cout << r.titleSize.width;
    r.~Reader();
    return 0;
}