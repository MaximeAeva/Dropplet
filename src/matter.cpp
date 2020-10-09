#include <matter.hpp>

/**
 * @brief Woaw matter *-* (Construct a new Matter:: Matter object)
 * 
 * @param d Is it matter ?
 */
Matter::Matter(int d)
{
    this->matterKind = d;
    this->speed = {0, 0};
    this->acceleration = {0, 0};
}

/**
 * @brief Here comes the blackhole (Destroy the Matter:: Matter object)
 * 
 */
Matter::~Matter()
{

}

/**
 * @brief Compute the speed at time t0+t
 * 
 * @param t 
 */
void Matter::computeSpeed(int t)
{
    this->speed.x += t*this->acceleration.x;
    this->speed.y += t*this->acceleration.y;
}

/**
 * @brief Compute position at time t0+t
 * 
 * @param t 
 */
void Matter::computePosition(int t)
{
    this->position.raw += t*this->speed.x;
    this->position.col += t*this->speed.y;
}