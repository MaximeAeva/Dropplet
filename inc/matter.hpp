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
        Matter(int matterKind, float mass, float size, float xPos, float yPos, float xSpd, float ySpd, float xAcc, float yAcc);
        ~Matter();
        void computeSpeed(float t);
        void computePosition(float t);
        void computeAcceleration(float x, float y);
        inline void resetAcceleration(){this->acceleration = {0, 0};};
        /**
         * @brief Kind of matter (To assign it its physics laws)
         * 
         */
        int matterKind;
        inline float getSpeed(){return sqrt(pow(this->speed.x, 2)+pow(this->speed.y, 2));};
        inline Coord getPos(){return {round(this->position.x), round(this->position.y)};};

    private:
        /**
         * @brief Mass not weight !
         * 
         */
        float mass;
        /**
         * @brief Matter drop ray (collision purpose)
         * 
         */
        float size;
        /**
         * @brief Acceleration vector (translate sum of force at t)
         * 
         */
        F acceleration;
        /**
         * @brief Speed vector (result of acceleration)
         * 
         */
        F speed;
        /**
         * @brief Actual position (Result of speed)
         * 
         */
        F position;  
};

#endif