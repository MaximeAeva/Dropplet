#ifndef MATTER_H
#define MATTER_H

#include <vector>
#include <cstddef>
#include <cstdlib>
#include <cmath>
#include <iostream>

struct F{
    float x;
    float y;
};

/**
 * @brief A way to give matter its own behaviour
 * 
 */
class Matter{
    public:
        Matter(bool d);
        ~Matter();
        bool drop = false;//is it a water drop?
        bool moved = false;//Has it moved ?
        int weight;
        int giveDir;
        std::vector<float> receive;//Forces from 8 directions
        std::vector<float> give;//give others drop its forces
        F force;
        
        void pfd(float wallLoss, float timeLoss, int wall);//Dynamic fundamental principe
        int move(std::vector<int> b);//Where do I go ?
        void reverseForce(int typ, float coeff = 0.8);
        void bye();
        inline float strenght(){return sqrt(pow(this->force.x, 2)+pow(this->force.y, 2));};
        void hello(Matter m);
        
};

#endif