#include <matrix.hpp>

/**
 * @brief Neighboors coord
 * 
 * @param i Direction 
 * @param raw Return raw or col
 * @return int 
 */
int theSwitcher(int i, bool raw)
{
    int sraw, scol;
    switch(i)
    {
        case 0:
            sraw = -1;scol = -1;
        break;
        case 1:
            sraw = -1;scol = 0;
        break;
        case 2:
            sraw = -1;scol = 1;
        break;
        case 3:
            sraw = 0;scol = 1;
        break;
        case 4:
            sraw = 1;scol = 1;
        break;
        case 5:
            sraw = 1;scol = 0;
        break;
        case 6:
            sraw = 1;scol = -1;
        break;
        case 7:
            sraw = 0;scol = -1;
        break;
    }
    if(raw) return sraw;
    else return scol;
} 

//################## Creators #####################

/**
 * @brief A Matrix of matter right here (Construct a new Matrix:: Matrix object)
 * 
 * @param height 
 * @param width 
 * @param cd 
 * @param waterLvl 
 */
Matrix::Matrix(int height, int width, Coord cd, int waterLvl, int matterKindDiversity)
{
    this->height = height;
    this->width = width;
    for(int matKd = 0; matKd < matterKindDiversity; matKd++)
    {  
        std::vector<Matter> v;
        for(int raw = 0; raw < height; raw++)
        {   
            for(int col = 0; col < width; col++)
            {
                if((raw==cd.raw)&&(col==cd.col)) 
                {
                    Matter p(0, 1, 0.5, raw, col, 0, 0, 0, 0);
                    v.push_back(p);
                }
                else if((raw==cd.raw)&&(col==(cd.col+1))) 
                {
                    Matter p(0, 1, 0.5, raw, col, 0, 0, 0, 0);
                    v.push_back(p);
                }
                else if(raw >= waterLvl) 
                {
                    Matter p(0, 1, 0.5, raw, col, 0, 0, 0, 0);
                    v.push_back(p);
                }
            }
        }
        this->mat.push_back(v);
    }
}

/**
 * @brief Bye Matter (Destroy the Matrix:: Matrix object)
 * 
 */
Matrix::~Matrix()
{
    for(int i = mat.size(); i>=0; i--)
    {
        for(int k = 0; k<mat[i].size(); k++)
        {
            this->mat[i][k].~Matter();
        }
        this->mat[i] = {};
    }
    this->mat = {};
}

//################## Physics #######################

/**
 * @brief Gravity simulation
 * 
 * @param force Regarding matter's weight 
 */
void Matrix::Gravity(float force, int applicationVector[5])
{
    for(int kind = 0; kind<5; kind++)//Through the authorised kind of matter
    {
        for(int seed = 0; seed<mat[applicationVector[kind]].size(); seed++)//Through the matter
        {
            mat[applicationVector[kind]][seed].computeAcceleration(force, 0);
        }       
    }
}

/**
 * @brief Intercation forces simulation
 * 
 * @param transmission coeff of transmission
 * 
 * @param loss vanishing coeff
 */
void Matrix::Transmission(float transmission, float loss, int applicationVector[5])
{
    for(int kind = 0; kind<5; kind++)//Through the authorised kind of matter
    {
        for(int matter = 0; matter<mat[kind].size(); matter++)//Through the matter
        {
            mat[kind][matter].computeAcceleration(0, 0);
        }       
    }
}


void Matrix::Reaction(float transmission, float loss, int applicationVector[5])
{
    for(int kind = 0; kind<5; kind++)//Through the authorised kind of matter
    {
        for(int matter = 0; matter<mat[kind].size(); matter++)//Through the matter
        {
            mat[kind][matter].computeAcceleration(0, 0);
        }       
    }
}

/**
 * @brief Update fluid tension
 * 
 * @param fluidTension coeff of tension
 */
void Matrix::Tension(float fluidTension, int applicationVector[5])
{
    for(int kind = 0; kind<5; kind++)//Through the authorised kind of matter
    {
        for(int matter = 0; matter<mat[kind].size(); matter++)//Through the matter
        {
            mat[kind][matter].computeAcceleration(0, 0);
        }       
    }
}

//############### Matrix evolution #################

/**
 * @brief Simulation routine
 * 
 * @param time Display speed
 * @param t Parity
 */
void Matrix::animate(int time, bool t)
{
    float transmission = 1;//Energy given to the others
    float gravity = 1;//Force in g
    float fluidTension = 0;//Percentage of follow up
    float loss = 0;//Loss energy at each collision
    float wallLoss = 0;//Loss at each wall collision
    float timeLoss = 0;//Loss at each step
    int foo[5] = {0, 0 , 0, 0, 0};

    for(int i = 0; i<=time; i++)
    {
        Gravity(gravity, foo);//Make them fall
        updateSpeed(0, 0);//These forces applied during a delta time (small)
        updatePosition(0);//This speed applied during a small delta time
        resetAcceleration();//No forces
    }

}

/**
 * @brief Update forces and set new give
 * 
 * @param wallLoss Loss energy during a collision with a wall
 * @param timeLoss Vanishing energy over time
 */
void Matrix::updateSpeed(float wallLoss, float timeLoss)
{
    for(int kind = 0; kind<mat.size(); kind++)//Through the authorised kind of matter
    {
        for(int seed = 0; seed<mat[kind].size(); seed++)//Through the matter
        {
            mat[kind][seed].computeSpeed(0.1);
        }       
    }
}

/**
 * @brief Move each matter along it force vector
 * 
 */
void Matrix::updatePosition(bool sens)
{
    for(int kind = 0; kind<mat.size(); kind++)//Through the authorised kind of matter
    {
        for(int seed = 0; seed<mat[kind].size(); seed++)//Through the matter
        {
            mat[kind][seed].computePosition(0.1);
        }       
    }
}

/**
 * @brief Just say everything goes ok
 * 
 */
void Matrix::resetAcceleration()
{
    for(int kind = 0; kind<mat.size(); kind++)//Through the authorised kind of matter
    {
        for(int seed = 0; seed<mat[kind].size(); seed++)//Through the matter
        {
            mat[kind][seed].resetAcceleration();
        }       
    }
}

//################## Others ####################

/**
 * @brief Sum of norms
 * 
 * @return int 
 */
float Matrix::totalStrenght()
{
    float k = 0;
    for(int kind = 0; kind<mat.size(); kind++)//Through the authorised kind of matter
    {
        for(int seed = 0; seed<mat[kind].size(); seed++)//Through the matter
        {
            k += mat[kind][seed].getSpeed();
        }       
    }
    return k;
}
