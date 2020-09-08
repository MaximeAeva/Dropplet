#ifndef MATRIX_H
#define MATRIX_H

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
 * @brief A way to give matter its own behaviour
 * 
 */
class Matter{
    public:
        Matter(bool d);
        ~Matter();
        bool drop = false;//is it a water drop?
        bool moved = false;//Has it moved ?
        void pfd(float wallLoss, float timeLoss, int wall);//Dynamic fundamental principe
        int move(std::vector<int> b);//Where do I go ?
        void reverseForce(int typ, float coeff = 0.8);
        void bye();
        inline float strenght(){return sqrt(pow(this->force.x, 2)+pow(this->force.y, 2));};
        void hello(Matter m);
        int weight;
        int giveDir;
        std::vector<float> receive;//Forces from 8 directions
        std::vector<float> give;//give others drop its forces
        F force;
};

/**
 * @brief A matrix of matter
 * 
 */
class Matrix{

    public:
        Matrix(int heigth, int width, Coord cd, int waterLvl = 20);
        ~Matrix();
       std::vector<std::vector<Matter> > mat;
       void animate(int time, bool t);
       inline int njMax(int dropPound, int wtrLvl, int drop8) {return dropPound*(drop8-wtrLvl);};
       int totalStrenght();

    private:
        void Transmission(float transmission, float loss);
        void Tension(float fluidTension);
        void Gravity(float force);
        void updatePositions(bool sens);
        void updateGives(float wallLoss, float timeLoss);
        void resetMoved();
        void resetReceive();
        void resetGive();
        int height;
        int width;
};

#endif