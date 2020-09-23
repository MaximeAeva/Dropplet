#include <matter.hpp>

/**
 * @brief Woaw matter *-* (Construct a new Matter:: Matter object)
 * 
 * @param d Is it matter ?
 */
Matter::Matter(bool d)
{
    std::vector<float> v(8, 0);
    this->give = v;
    this->moved = false;
    this->drop = d;
    if(d) this->weight = 1;
    this->force = {0, 0};
    this->receive = v;
}

/**
 * @brief Here comes the blackhole (Destroy the Matter:: Matter object)
 * 
 */
Matter::~Matter()
{
    this->receive = {};
    this->give = {};
}

/**
 * @brief Say matrix there's no matter here anymore
 * 
 */
void Matter::bye()
{
    this->receive = {};
    this->give = {};
    this->drop = false;
    this->moved = false;
    this->weight = 0;
    this->force = {0, 0};
}

/**
 * @brief Welcome to the matter ! 
 * 
 * @param m 
 */
void Matter::hello(Matter m)
{
    this->drop = m.drop;
    this->weight = m.weight;
    this->give = m.give;
    this->receive = m.receive;
    this->moved = true;
    this->force = m.force;
}

/**
 * @brief Update forces and give its direction
 * 
 * @param wallLoss Loss against a wall
 * @param wall Where is the wall
 */
void Matter::pfd(float wallLoss, float timeLoss, int wall = 0)
{
    //Add force received from each direction
    float epsilon = 1e-10;
    float x, y;
    for(int i = 0; i<8; i++)
    {
        switch(i)
        {
            case 0:
                x = 1/sqrt(2);
                y = 1/sqrt(2);
            break;
            case 1:
                x = 1;
                y = 0;
            break;
            case 2:
                x = 1/sqrt(2);
                y = -1/sqrt(2);
            break;
            case 3:
                x = 0;
                y = -1;
            break;
            case 4:
                x = -1/sqrt(2);
                y = -1/sqrt(2);
            break;
            case 5:
                x = -1;
                y = 0;
            break;
            case 6:
                x = -1/sqrt(2);
                y = 1/sqrt(2);
            break;
            case 7:
                x = 0;
                y = 1;
            break;
        }
        this->force.x += this->receive[i]*x;
        this->force.y += this->receive[i]*y;
    }
    //Avoid quasi null force movement and apply timeLoss
    if(abs(this->force.x) < epsilon) this->force.x = 0;
    else this->force.x *= 1-timeLoss;
    if(abs(this->force.y) < epsilon) this->force.y = 0;
    else this->force.y *= 1-timeLoss;
    //Reverse if directed against a wall
    if(wall) this->reverseForce(wall, 1-wallLoss);
    //Compute force angle to know the movement direction
    int angle;
    if((this->force.x == 0) && (this->force.y == 0)) angle = 0;
    else angle = floor(atan2(-this->force.x, this->force.y)/(3.1415/8));
    switch(angle)
    {
        case -9:
            this->giveDir = 7;
        break;
        case -8:
            this->giveDir = 7;
        break;
        case -7:
            this->giveDir = 6;
        break;
        case -6:
            this->giveDir = 6;
        break;
        case -5:
            this->giveDir = 5;
        break;
        case -4:
            this->giveDir = 5;
        break;
        case -3:
            this->giveDir = 4;
        break;
        case -2:
            this->giveDir = 4;
        break;
        case -1:
            this->giveDir = 3;
        break;
        case 0:
            this->giveDir = 3;
        break;
        case 1:
            this->giveDir = 2;
        break;
        case 2:
            this->giveDir = 2;
        break;
        case 3:
            this->giveDir = 1;
        break;
        case 4:
            this->giveDir = 1;
        break;
        case 5:
            this->giveDir = 0;
        break;
        case 6:
            this->giveDir = 0;
        break;
        case 7:
            this->giveDir = 7;
        break;
        case 8:
            this->giveDir = 7;
        break;
    }
    //Translate into a given force (cos(pi/4))
    this->give[this->giveDir] = this->strenght()/2.0;
    this->give[(this->giveDir+1)%8] = this->strenght()/(2*sqrt(2));
    this->give[(this->giveDir-1)%8] = this->strenght()/(2*sqrt(2));
}

/**
 * @brief Give the position to move
 * 
 * @param b neighbours kind of matter
 * @return int position to move (-1 if stay)
 */
int Matter::move(std::vector<int> b)
{
    if(this->give[this->giveDir] == 0) return -1;
    if(!b[this->giveDir]) return this->giveDir;//Priority to the major direction
    else if(b[(this->giveDir-1)%8] && b[(this->giveDir+1)%8]) return -1;//If nothing stay here
    else if((!b[(this->giveDir-1)%8]) && (!b[(this->giveDir+1)%8])) return (this->giveDir - 1 + 2*(rand()%2))%8;//Randomly go right or left
    else
    {
        if(!b[(this->giveDir-1)%8]) return (this->giveDir-1)%8;
        else return (this->giveDir+1)%8;
    }
}

/**
 * @brief Bounce on a surface
 * 
 * @param typ Where is the wall (u->up,r->right,b->bottom,l->left) 
 * @param c Coefficient of transmission
 */
void Matter::reverseForce(int typ, float c)
{
    switch(typ)
    {
        case 1:
            if(this->force.x<0) 
            {
                this->force.x *= -c;
                this->force.y *= c;
            }
        break;
        case 10:
            if(this->force.y<0)
            {
                this->force.y *= -c;
                this->force.x *= c;
            } 
        break;
        case 20:
            if(this->force.y>0)
            {
                this->force.y *= -c;
                this->force.x *= c;
            } 
        break;
        case 2:
            if(this->force.x>0) 
            {
                this->force.x *= -c;
                this->force.y *= c;
            }
        break;
        case 11:
            if(this->force.x<0) this->force.x *= -c;
            if(this->force.y<0) this->force.y *= -c;
        break;
        case 21:
            if(this->force.x<0) this->force.x *= -c;
            if(this->force.y>0) this->force.y *= -c;
        break;
        case 12:
            if(this->force.x>0) this->force.x *= -c;
            if(this->force.y<0) this->force.y *= -c;
        break;
        case 22:
            if(this->force.x>0) this->force.x *= -c;
            if(this->force.y>0) this->force.y *= -c;
        break;
    }
}