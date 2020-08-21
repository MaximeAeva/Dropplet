#include <iostream>
#include <display.hpp>
#include <reader.hpp>

int main()
{
    Reader r("You are welcome");
    r.~Reader();
    return 0;
}