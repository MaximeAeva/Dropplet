#include <iostream>
#include <time.h>
#include <display.hpp>
#include <reader.hpp>
#include <matrix.hpp>

int main()
{
    int height = 15;//Display height
    int waterLvl = 1;//Water level
    int drop8 = 7;//Drop height 
    std::string s = "abcde";//Message
    
    /*######################## ... ########################*/

    srand(time(NULL));
    Coord drop;
    Reader r(s);
    drop.raw = height-drop8-1;
    drop.col = floor(r.titleSize.width/2);
    //drop.col = r.titleSize.width-2;
    Matrix m(height, r.titleSize.width, drop, height-waterLvl);
    Display d(height, r.titleSize.width, r.text);
    bool t = true;
    while(1)
    {
        d.show(m, m.njMax(), 100);//Refresh time can be limit
        m.animate(10, t);
        if(t) t = false;
        else t = true;
        if(GetAsyncKeyState(VK_DOWN))
        {
            m.genDrop();
        }
    }

    m.~Matrix();
    r.~Reader();
    d.~Display();
    return 0;
}