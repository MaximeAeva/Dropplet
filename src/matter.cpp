#include <matter.hpp>

/**
 * @brief Construct a new Matter:: Matter object
 * 
 * @param matterKind Kind of matter it belong
 * @param mass mass
 * @param size matter ray (consider its a spherical particule)
 * @param xPos position along x
 * @param yPos position along y
 * @param xSpd speed along x
 * @param ySpd speed along y
 * @param xAcc acceleration along x
 * @param yAcc acceleration along y
 */
Matter::Matter(int matterKind, float mass, float size, float xPos, float yPos, float xSpd, float ySpd, float xAcc, float yAcc)
{
    this->matterKind = matterKind;
    this->mass = mass;
    this->size = size;
    this->position = {xPos, yPos};
    this->speed = {xSpd, ySpd};
    this->acceleration = {xAcc, yAcc};
}

/**
 * @brief Here comes the blackhole (Destroy the Matter:: Matter object)
 * 
 */
Matter::~Matter()
{

}

/**
 * @brief Add a force to the acceleration
 * 
 * @param x along x axis
 * @param y along y axis
 */
void Matter::computeAcceleration(float x, float y)
{
    this->acceleration.x += x/this->mass;
    this->acceleration.y += y/this->mass;
}

/**
 * @brief Compute the speed at time t0+t
 * 
 * @param t 
 */
void Matter::computeSpeed(float t)
{
    this->speed.x += t*this->acceleration.x;
    this->speed.y += t*this->acceleration.y;
}

/**
 * @brief Compute position at time t0+t
 * 
 * @param t 
 */
void Matter::computePosition(float t)
{
    this->position.x += t*this->speed.x;
    this->position.y += t*this->speed.y;
}