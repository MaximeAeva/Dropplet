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

/**
 * @brief Proximity law (tension is more about attraction than contact)
 * 
 * @param value factor of distance regarding particule ray
 * @return float 
 */
float tensionFunction(float value)
{
    return 0.5*(0.5*tanh((16*value)-20));
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
        int i = 0;
        while(i < 500)
        {
            try {
            Matter p(0, 1, 0.31, rand() % (height-1) + (rand() % 100 + 50)/100, rand() % (width-1) + (rand() % 100 + 50)/100, 0, 0, 0, 0);
            v.push_back(p);
            } catch (const std::bad_alloc& e) {
                std::cout << "Allocation failed: " << e.what() << std::endl;
            }
            i++;
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
            if(mat[k][seed].getPos().x + mat[k][seed].getSize() >= this->height-1)//floor
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
            else if(mat[k][seed].getPos().y + mat[k][seed].getSize() >= this->width-1)//right wall
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
                    if(distance <= (mat[applicationVector[kind]][seed].getSize()+mat[applicationVector[kind]][otherSeed].getSize()))//HeavySide proximity function
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
                    float factor = tensionFunction((mat[applicationVector[kind]][seed].getSize()+mat[applicationVector[kind]][otherSeed].getSize())/
                                            distance);                
                    float arg = atan2(mat[applicationVector[kind]][otherSeed].getPos().y - mat[applicationVector[kind]][seed].getPos().y, 
                    mat[applicationVector[kind]][otherSeed].getPos().x - mat[applicationVector[kind]][seed].getPos().x);
                    float tr = fluidTension*factor;
                    mat[applicationVector[kind]][otherSeed].computeAcceleration(tr*cos(arg), tr*sin(arg));
                    mat[applicationVector[kind]][seed].computeAcceleration(-tr*cos(arg), -tr*sin(arg));
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
    float fluidTension = 0.001;//Percentage of attraction
    float loss = 0.4;//Loss energy at each collision
    float wallLoss = 0.8;//Loss at each wall collision
    float timeLoss = 0.3;//Loss at each step
    float timeStep = 0.05;

    for(int i = 0; i<=time; i++)
    {
        Gravity(gravity);//Make them fall

        Transmission(transmission, loss, timeStep);

        Tension(fluidTension);

        BoundaryConditions(wallLoss, timeStep);
        
        
        updatePosition(timeStep);//This speed applied during a small delta time
        updateSpeed(timeStep, timeLoss);//These forces applied during a delta time (small)
        
        resetAcceleration();//No forces
    }
    //std::cout << this->mat[0][0].getCoord().raw << ", " << this->mat[0][0].getCoord().col << std::endl;

}

/**
 * @brief Update forces and set new give
 * 
 * @param wallLoss Loss energy during a collision with a wall
 * @param timeLoss Vanishing energy over time
 */
void Matrix::updateSpeed(float timeStep, float timeLoss)
{
    for(int kind = 0; kind<mat.size(); kind++)//Through the authorised kind of matter
    {
        for(int seed = 0; seed<mat[kind].size(); seed++)//Through the matter
        {
            mat[kind][seed].computeSpeed(timeStep, timeLoss);
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
    for(int i = 0; i<this->mat.size(); i++)
    {
        for(int j = 0; j<this->mat[i].size(); j++)
            k += this->mat[i][j].getMass()*(this->height- this->mat[i][j].getPos().x) +
                    0.5*this->mat[i][j].getMass()*pow(this->mat[i][j].getSpeed(), 2);
    }
    return k;
}

void Matrix::genDrop()
{
    int i = 0;
    while(i < 18)
    {
        try {
        Matter p(0, 1, 0.31, 0.9*(i/6), width/2-1+0.9*(i%6), 0, 0, 0, 0);
        this->mat[0].push_back(p);
        } catch (const std::bad_alloc& e) {
            std::cout << "Allocation failed: " << e.what() << std::endl;
        }
        i++;
        
    }
}

int Matrix::njMax()
{
    int nj = 0;
    for(int i = 0; i<this->mat.size(); i++)
    {
        for(int j = 0; j<this->mat[i].size(); j++)
            nj += this->mat[i][j].getMass()*(this->height- this->mat[i][j].getPos().x) +
                    0.5*this->mat[i][j].getMass()*pow(this->mat[i][j].getSpeed(), 2);
    }
    return nj;
}

