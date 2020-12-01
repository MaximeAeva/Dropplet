#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <cstddef>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <matter.hpp>

/**
 * @brief A matrix of matter
 * 
 */
class Matrix{

    public:
        Matrix(int heigth, int width, Coord cd, int waterLvl = 20, int matterKindDiversity = 1);
        ~Matrix();
       std::vector<std::vector<Matter> > mat;
       void animate(int time, bool t);
       inline int njMax() {return this->mat[0][0].getMass()*this->mat[0].size()*this->height;};
       float totalStrenght();
       void genDrop();

    private:
        void BoundaryConditions(float wallLoss, float timeStep);
        void Transmission(float transmission, float loss, float timeStep);
        void Reaction(float transmission, float loss);
        void Tension(float fluidTension);
        void Gravity(float force);
        void updatePosition(float timeStep);
        void updateSpeed(float timeStep);
        void resetAcceleration();
        int applicationVector[1] = {0};
        int height;
        int width;
};

#endif