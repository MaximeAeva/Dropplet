#ifndef MATTER_H
#define MATTER_H

#include <vector>
#include <cstddef>
#include <cstdlib>
#include <cmath>
#include <iostream>

struct Coord{
    int raw;
    int col;
};

struct F{
    float x;
    float y;
};

/**
 * @brief Nude matter.
 * 
 */
class Matter{
    public:
        Matter(int matterKind);
        ~Matter();
        void computeSpeed(int t);
        void computePosition(int t);
        F acceleration;

    private:
        int matterKind = 0;
        int mass;
        F speed;
        Coord position;  
};

#endif