#include <iostream>
#include <display.hpp>
#include <reader.hpp>

int main()
{
    std::cout << "TheQuickStart wish you the best !" << std::endl;
    Reader r("Hello");
    r.~Reader();
    return 0;
}