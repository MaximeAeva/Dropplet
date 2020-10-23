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
                if((raw==1) && (col>= 7) && col < 9) 
                {
                    Matter p(0, 1, 0.5, raw+(rand() % 100 - 50)/100, col+(rand() % 100 -50)/100, 0, rand() % 10 - 5, 0, 0);
                    v.push_back(p);
                }

                if(raw>height-3)
                {
                    Matter p(0, 1, 0.5, raw+(rand() % 100 - 50)/100, col+(rand() % 100 -50)/100, 0, rand() % 10 - 5, 0, 0);
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

void Matrix::BoundaryConditions(float wallLoss, float timeStep)
{
    int arrSize = sizeof(applicationVector)/sizeof(applicationVector[0]);
    float coeff = 1 + (1-wallLoss);
    for(int kind = 0; kind<arrSize; kind++)//Through the authorised kind of matter
    {
        for(int seed = 0; seed<mat[applicationVector[kind]].size(); seed++)//Through the matter
        {
            int k = applicationVector[kind];
            if(mat[k][seed].getPos().x + mat[k][seed].getSize() >= this->height-mat[k][seed].getSize()/2)//floor
            {
                if(mat[k][seed].getAcc().x > 0) mat[k][seed].computeAcceleration(-mat[k][seed].getAcc().x*mat[applicationVector[kind]][seed].getMass(), 0);
                if(mat[k][seed].getSpd().x > 0) mat[k][seed].computeAcceleration((-coeff*mat[k][seed].getSpd().x)*mat[applicationVector[kind]][seed].getMass()/timeStep, 0);
            }
            else if(mat[k][seed].getPos().x - mat[k][seed].getSize() < -mat[k][seed].getSize()/2)//ceilling
            {
                if(mat[k][seed].getAcc().x < 0) mat[k][seed].computeAcceleration(-mat[k][seed].getAcc().x*mat[applicationVector[kind]][seed].getMass(), 0);
                if(mat[k][seed].getSpd().x < 0) mat[k][seed].computeAcceleration((-coeff*mat[k][seed].getSpd().x)*mat[applicationVector[kind]][seed].getMass()/timeStep, 0);
            }
            else if(mat[k][seed].getPos().y - mat[k][seed].getSize() < -mat[k][seed].getSize()/2)//left wall
            {
                if(mat[k][seed].getAcc().y < 0) mat[k][seed].computeAcceleration(0, -mat[k][seed].getAcc().y*mat[applicationVector[kind]][seed].getMass());
                if(mat[k][seed].getSpd().y < 0) mat[k][seed].computeAcceleration(0, (-coeff*mat[k][seed].getSpd().y)*mat[applicationVector[kind]][seed].getMass()/timeStep);
            }
            else if(mat[k][seed].getPos().y + mat[k][seed].getSize() >= this->width-mat[k][seed].getSize()/2)//right wall
            {
                if(mat[k][seed].getAcc().y > 0) mat[k][seed].computeAcceleration(0, -mat[k][seed].getAcc().y*mat[applicationVector[kind]][seed].getMass());
                if(mat[k][seed].getSpd().y > 0) mat[k][seed].computeAcceleration(0, (-coeff*mat[k][seed].getSpd().y)*mat[applicationVector[kind]][seed].getMass()/timeStep);
            }
        }       
    }
}

/**
 * @brief Gravity simulation
 * 
 * @param force Regarding matter's weight 
 */
void Matrix::Gravity(float force)
{
    int arrSize = sizeof(applicationVector)/sizeof(applicationVector[0]);
    for(int kind = 0; kind<arrSize; kind++)//Through the authorised kind of matter
    {
        for(int seed = 0; seed<mat[applicationVector[kind]].size(); seed++)//Through the matter
        {
            mat[applicationVector[kind]][seed].computeAcceleration(force*mat[applicationVector[kind]][seed].getMass(), 0);
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
void Matrix::Transmission(float transmission, float loss, float timeStep)
{
    int arrSize = sizeof(applicationVector)/sizeof(applicationVector[0]);
    for(int kind = 0; kind<arrSize; kind++)//Through the authorised kind of matter
    {
        for(int seed = 0; seed<mat[kind].size(); seed++)//Through the matter
        {
           for(int otherSeed = 0; otherSeed<mat[kind].size(); otherSeed++)//Through the matter
            {
                if(otherSeed != seed)//Do not interact with itself
                {
                    float distance = sqrt(pow(mat[applicationVector[kind]][seed].getPos().x - mat[applicationVector[kind]][otherSeed].getPos().x, 2) + 
                                        pow(mat[applicationVector[kind]][seed].getPos().y - mat[applicationVector[kind]][otherSeed].getPos().y, 2));
                    if(distance <= (mat[applicationVector[kind]][seed].getSize()+mat[applicationVector[kind]][otherSeed].getSize()))//Close enough
                    {
                        float arg = atan2(mat[applicationVector[kind]][otherSeed].getPos().y - mat[applicationVector[kind]][seed].getPos().y, 
                        mat[applicationVector[kind]][otherSeed].getPos().x - mat[applicationVector[kind]][seed].getPos().x);
                        if(cos(mat[applicationVector[kind]][seed].getSpdArg()-arg)>0)//Well oriented
                        {
                            float tr = mat[applicationVector[kind]][seed].getSpeed()*cos(mat[applicationVector[kind]][seed].getSpdArg()-arg)
                            *mat[applicationVector[kind]][seed].getMass();
                            mat[applicationVector[kind]][otherSeed].computeAcceleration(transmission*tr*cos(arg)/timeStep, transmission*tr*sin(arg)/timeStep);
                            mat[applicationVector[kind]][seed].computeAcceleration(-(2-loss-transmission)*tr*cos(arg)/timeStep, -(2-loss-transmission)*tr*sin(arg)/timeStep);
                        }
                        if(cos(mat[applicationVector[kind]][seed].getAccArg()-arg)>0)
                        {
                            float tra = mat[applicationVector[kind]][seed].getAcceleration()*cos(mat[applicationVector[kind]][seed].getAccArg()-arg)*
                            mat[applicationVector[kind]][seed].getMass();
                            mat[applicationVector[kind]][otherSeed].computeAcceleration(tra*cos(arg), tra*sin(arg));
                            mat[applicationVector[kind]][seed].computeAcceleration(-tra*cos(arg), -tra*sin(arg));
                        }
                    }
                }    
            }  
        }       
    }
}


void Matrix::Reaction(float transmission, float loss)
{
    int arrSize = sizeof(applicationVector)/sizeof(applicationVector[0]);

}

/**
 * @brief Update fluid tension
 * 
 * @param fluidTension coeff of tension
 */
void Matrix::Tension(float fluidTension)
{
    int arrSize = sizeof(applicationVector)/sizeof(applicationVector[0]);
    for(int kind = 0; kind<arrSize; kind++)//Through the authorised kind of matter
    {
        for(int seed = 0; seed<mat[kind].size(); seed++)//Through the matter
        {
           for(int otherSeed = 0; otherSeed<mat[kind].size(); otherSeed++)//Through the matter
            {
                if(otherSeed != seed)//Do not interact with itself
                {
                    float distance = sqrt(pow(mat[applicationVector[kind]][seed].getPos().x - mat[applicationVector[kind]][otherSeed].getPos().x, 2) + 
                                        pow(mat[applicationVector[kind]][seed].getPos().y - mat[applicationVector[kind]][otherSeed].getPos().y, 2));
                    if(distance <= (mat[applicationVector[kind]][seed].getSize()+mat[applicationVector[kind]][otherSeed].getSize()))//Close enough
                    {
                        float arg = atan2(mat[applicationVector[kind]][otherSeed].getPos().y - mat[applicationVector[kind]][seed].getPos().y, 
                        mat[applicationVector[kind]][otherSeed].getPos().x - mat[applicationVector[kind]][seed].getPos().x);
                        if(cos(mat[applicationVector[kind]][seed].getSpdArg()-arg)<0)//Well oriented
                        {
                            float tr = mat[applicationVector[kind]][seed].getSpeed()*cos(mat[applicationVector[kind]][seed].getSpdArg()-arg)*fluidTension;
                            mat[applicationVector[kind]][otherSeed].computeAcceleration(tr*cos(arg), tr*sin(arg));
                            mat[applicationVector[kind]][seed].computeAcceleration(-tr*cos(arg), -tr*sin(arg));
                        }
                    }
                }    
            }  
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
    float transmission = 0.5;//Energy given to the others
    float gravity = 1;//Force in g
    float fluidTension = 0.5;//Percentage of follow up
    float loss = 0.2;//Loss energy at each collision
    float wallLoss = 0.5;//Loss at each wall collision
    float timeLoss = 0;//Loss at each step
    float timeStep = 0.01;

    for(int i = 0; i<=time; i++)
    {
        Gravity(gravity);//Make them fall

        Transmission(transmission, loss, timeStep);

        Tension(fluidTension);

        BoundaryConditions(wallLoss, timeStep);
        
        
        updatePosition(timeStep);//This speed applied during a small delta time
        updateSpeed(timeStep);//These forces applied during a delta time (small)
        
        resetAcceleration();//No forces
    }
    std::cout << this->mat[0][0].getSpeed() << ", " << this->mat[0][0].getSpdArg() << std::endl;

}

/**
 * @brief Update forces and set new give
 * 
 * @param wallLoss Loss energy during a collision with a wall
 * @param timeLoss Vanishing energy over time
 */
void Matrix::updateSpeed(float timeStep)
{
    for(int kind = 0; kind<mat.size(); kind++)//Through the authorised kind of matter
    {
        for(int seed = 0; seed<mat[kind].size(); seed++)//Through the matter
        {
            mat[kind][seed].computeSpeed(timeStep);
        }       
    }
}

/**
 * @brief Move each matter along it force vector
 * 
 */
void Matrix::updatePosition(float timeStep)
{
    for(int kind = 0; kind<mat.size(); kind++)//Through the authorised kind of matter
    {
        for(int seed = 0; seed<mat[kind].size(); seed++)//Through the matter
        {
            mat[kind][seed].computePosition(timeStep);
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
