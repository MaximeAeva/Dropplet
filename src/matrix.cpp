#include <matrix.hpp>


/**
 * @brief Woaw matter *-* (Construct a new Matter:: Matter object)
 * 
 * @param d Is it matter ?
 */
Matter::Matter(bool d)
{
    std::vector<float> receive(8, 0);
    this->give = receive;
    this->moved = false;
    this->drop = d;
    if(d) this->weight = 1;
    this->receive = receive;
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
}

/**
 * @brief Welcome to the matter ! 
 * 
 * @param m 
 */
void Matter::hello(Matter m)
{
    this->drop = m.drop;
    this->give = m.give;
    this->receive = m.receive;
    this->moved = true;
}

/**
 * @brief Give the traction forces
 *  
 */
void Matter::pfd()
{
    //Add each received force but care not to only add (result in infinite movement)
    float epsilon = 0.05;
    float a;
    for(int i = 0; i<4; i++)
    {
        a = this->receive[i]-this->receive[(i+4)%8];
        if(abs(a)<epsilon) this->receive[i] = 0;
        if(a>0) this->give[i] -= a;
        else this->give[(i+4)%8] += a;
    }
    for(int i = 0; i<8; i++) if(this->give[i]<0)
    {
        this->give[(i+4)%8] -= this->give[i];
        this->give[i] = 0;
    }
    //Cushion (avoid infinite sliding)
    for(int i = 3; i<8; i+=4) this->give[i] *= 0.95;
}

/**
 * @brief Give the position to move
 * 
 * @param b neighbours kind of matter
 * @return int position to move (-1 if stay)
 */
int Matter::move(std::vector<bool> b)
{
    std::vector<int> pos;
    std::vector<float> val;
    for(int i = 0; i<8; i++)
    {
        if((!b[i]) && this->give[i]>0)
        {
            pos.push_back(i);
            val.push_back(this->give[i]);
        }
    }
    //Distribute
    if(val.empty())
    {
        int pos = 0;
        int val = 0;
        for(int i = 0; i<8; i++)
        {
            if(this->give[i] > val)
            {
                pos = i;
                val = this->give[i];
            }
        } 
        if(val>0)
        {
            float k = this->give[pos];
            for(int i = -1; i<2; i++) this->give[(pos+i)%8] += (2-abs(i))*k/4;
            this->give[pos] -= k;
        }
        return -1;
    }
    //Move
    else 
    {
        int k = 0;
        float l = 0;
        for(int i = 0; i<val.size(); i++)
        {
            if(val[i]>l)
            {
                k = pos[i];
                l = val[i];
            }
        }
        return k;
    }
    
}

/**
 * @brief Rebound (vertical or hori) with a loss coefficient
 * 
 * @param vert 
 * @param c 
 */
void Matter::reverseGive(bool vert, float c)
{
    float t;
    if(vert)
    {
        t = c*this->give[6]; this->give[6] = c*this->give[0];this->give[0] = t;
        t = c*this->give[5]; this->give[5] = c*this->give[1];this->give[1] = t;
        t = c*this->give[4]; this->give[4] = c*this->give[2];this->give[2] = t;
    }
    else
    {
        t = c*this->give[0]; this->give[0] = c*this->give[2];this->give[2] = t;
        t = c*this->give[7]; this->give[7] = c*this->give[3];this->give[3] = t;
        t = c*this->give[6]; this->give[6] = c*this->give[4];this->give[4] = t;
    }
    
}

/**
 * @brief Give Matter movement strenght
 * 
 * @return float 
 */
float Matter::strenght()
{
    float m = 0;
    for(int i = 0; i<8; i++) m += this->give[i];
    return m;
}


/**
 * @brief A Matrix of matter right here (Construct a new Matrix:: Matrix object)
 * 
 * @param height 
 * @param width 
 * @param cd 
 * @param waterLvl 
 */
Matrix::Matrix(int height, int width, Coord cd, int waterLvl)
{
    this->height = height;
    this->width = width;
    for(int raw = 0; raw < height; raw++)
    {
        std::vector<Matter> v;
        for(int col = 0; col < width; col++)
        {
            if((raw==cd.raw)&&(col==cd.col)) 
            {
                Matter p(true);
                //p.weight = 20;//Drop weight
                v.push_back(p);
            }
            else if(raw >= waterLvl) 
            {
                Matter p(true);
                v.push_back(p);
            }
            else 
            {
                Matter p(false);
                v.push_back(p);
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
    for(int i = 0; i<this->height; i++)
    {
        for(int k = 0; k<this->width; k++)
        {
            this->mat[i][k] = NULL;
        }
        this->mat[i] = {};
    }
    this->mat = {};
}

/**
 * @brief Compute the next step
 * 
 * @param time Speed at wich you want to travel
 */
void Matrix::animate(int time)
{
    for(int i = 0; i<=time; i++)
    {
        updateReceive();//Update external forces

        updateGives();//Update internal forces

        updatePositions();//Update positions

        resetMoved();//Say each cell is movable
    }
}

/**
 * @brief Update external forces of each matter
 * 
 */
void Matrix::updateReceive()
{
    float fluidTension = 0.4;//Followed forces
    float wallLoss = 0.5;//When against a border
    float transmission = 0.8;//Give each others
    for(int raw = 0; raw<this->height; raw++)
    {
        for(int col = 0; col<this->width; col++)
        {
            if(this->mat[raw][col].drop)//If a drop, look forces around
            {
                int sraw, scol;
                for(int i = 0; i<8; i++)
                {
                    switch(i)
                    {
                        case 0:
                            sraw = -1;scol = -1;
                            this->mat[raw][col].receive[i] = 0;
                        break;
                        case 1:
                            sraw = -1;scol = 0;
                            this->mat[raw][col].receive[i] = 0;
                        break;
                        case 2:
                            sraw = -1;scol = 1;
                            this->mat[raw][col].receive[i] = 0;
                        break;
                        case 3:
                            sraw = 0;scol = 1;
                            this->mat[raw][col].receive[i] = 0;
                        break;
                        case 4:
                            sraw = 1;scol = 1;
                            this->mat[raw][col].receive[i] = 0;
                        break;
                        case 5:
                            sraw = 1;scol = 0;
                            this->mat[raw][col].receive[i] = 0;
                        break;
                        case 6:
                            sraw = 1;scol = -1;
                            this->mat[raw][col].receive[i] = 0;
                        break;
                        case 7:
                            sraw = 0;scol = -1;
                            this->mat[raw][col].receive[i] = 0;
                        break;
                    }
                    //Boundary conditions (rebound with a coefficient)
                    if((raw+sraw)<0) this->mat[raw][col].reverseGive(true, 1-wallLoss);
                    else if((raw+sraw)>=this->height) this->mat[raw][col].reverseGive(true, 1-wallLoss);
                    else if ((col+scol)<0) this->mat[raw][col].reverseGive(false, 1-wallLoss);
                    else if((col+scol)>=this->width) this->mat[raw][col].reverseGive(false, 1-wallLoss);
                    else 
                    {   
                        //Gravity
                        if(i>2) this->mat[raw][col].receive[(i+4)%8] += this->mat[raw][col].weight*(3-abs(i-5))/3;
                        if(this->mat[raw+sraw][col+scol].drop)//Interaction force
                        {
                            //Counter reaction
                            if(i>2) this->mat[raw][col].receive[i] += this->mat[raw][col].weight*(3-abs(i-5))/3;
                            //give with loss
                            this->mat[raw][col].receive[i] += transmission*this->mat[raw+sraw][col+scol].give[(i+4)%8];
                            //Reflection
                            this->mat[raw+sraw][col+scol].give[i] += (1-transmission)*this->mat[raw+sraw][col+scol].give[(i+4)%8];
                            this->mat[raw+sraw][col+scol].give[(i+4)%8] *= 1-transmission;
                            //Internal fluid tension
                            this->mat[raw][col].receive[(i+4)%8] += fluidTension*this->mat[raw+sraw][col+scol].give[i];
                            this->mat[raw+sraw][col+scol].give[i] *= 1-fluidTension;
                        }
                        else this->mat[raw][col].receive[i] += 0;
                    }
                }
            }
        }
    }
}

/**
 * @brief Update internal forces of each matter
 * 
 */
void Matrix::updateGives()
{
    for(int raw = 0; raw<this->height; raw++)
    {
        for(int col = 0; col<this->width; col++)
        {
            if(this->mat[raw][col].drop) this->mat[raw][col].pfd();
        }
    }
}

/**
 * @brief Move each matter along it force vector
 * 
 */
void Matrix::updatePositions()
{

    for(int raw = 0; raw<this->height; raw++)
    {
        for(int col = 0; col<this->width; col++)
        {
            //Look for neighbourhood
            if(this->mat[raw][col].drop && !(this->mat[raw][col].moved))
            {
                std::vector<bool> neighbours;
                int sraw, scol;
                for(int i = 0; i<8; i++)
                {
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
                    //Boundary neighboors (nothing goes out)
                    if((raw+sraw)<0) neighbours.push_back(true);
                    else if((raw+sraw)>=this->height) neighbours.push_back(true);
                    else if ((col+scol)<0) neighbours.push_back(true);
                    else if((col+scol)>=this->width) neighbours.push_back(true);
                    else neighbours.push_back(this->mat[raw+sraw][col+scol].drop);
                }
                //Move if needed
                switch(this->mat[raw][col].move(neighbours))
                {
                    case 0:
                        this->mat[raw-1][col-1].hello(this->mat[raw][col]);
                        this->mat[raw][col].bye();
                    break;
                    case 1:
                        this->mat[raw-1][col].hello(this->mat[raw][col]);
                        this->mat[raw][col].bye();
                    break;
                    case 2:
                        this->mat[raw-1][col+1].hello(this->mat[raw][col]);
                        this->mat[raw][col].bye();
                    break;
                    case 3:
                        this->mat[raw][col+1].hello(this->mat[raw][col]);
                        this->mat[raw][col].bye();
                    break;
                    case 4:
                        this->mat[raw+1][col+1].hello(this->mat[raw][col]);
                        this->mat[raw][col].bye();
                    break;
                    case 5:
                        this->mat[raw+1][col].hello(this->mat[raw][col]);
                        this->mat[raw][col].bye();
                    break;
                    case 6:
                        this->mat[raw+1][col-1].hello(this->mat[raw][col]);
                        this->mat[raw][col].bye();
                    break;
                    case 7:
                        this->mat[raw][col-1].hello(this->mat[raw][col]);
                        this->mat[raw][col].bye();
                    break;
                    default:
                    break;
                }
            }
        }
    }
}

/**
 * @brief Just say everything goes ok
 * 
 */
void Matrix::resetMoved()
{
    for(int raw = 0; raw<this->height; raw++)
    {
        for(int col = 0; col<this->width; col++)
        {
            if(this->mat[raw][col].drop) 
                this->mat[raw][col].moved = false;
        }
    }
}
