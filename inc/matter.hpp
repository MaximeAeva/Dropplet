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
        /**
         * @brief Get the Speed object
         * 
         * @return float 
         */
        inline float getSpeed(){return sqrt(pow(this->speed.x, 2)+pow(this->speed.y, 2));};
        /**
         * @brief Get the Coord object
         * 
         * @return Coord 
         */
        inline Coord getCoord(){return {round(this->position.x), round(this->position.y)};};
        /**
         * @brief Get the Acc object
         * 
         * @return F 
         */
        inline F getAcc(){return {this->acceleration.x, this->acceleration.y};};
        /**
         * @brief Get the Spd object
         * 
         * @return F 
         */
        inline F getSpd(){return {this->speed.x, this->speed.y};};
        /**
         * @brief Get the Pos object
         * 
         * @return F 
         */
        inline F getPos(){return {this->position.x, this->position.y};};
        /**
         * @brief Get the Size object
         * 
         * @return float 
         */
        inline float getSize(){return this->size;};
        /**
         * @brief Get the Mass object
         * 
         * @return float 
         */
        inline float getMass(){return this->mass;};
        /**
         * @brief Get the Arg object
         * 
         * @return float 
         */
        inline float getArg(){return atan2(this->speed.y, this->speed.x);};



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