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
    this->acceleration = {0, 0};
    this->speed = {0, 0};
    this->drop = d;
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
    this->acceleration = {0, 0};
    this->speed = {0, 0};
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
    this->speed = m.speed;
    this->acceleration = m.acceleration;
}

/**
 * @brief Give the traction forces
 *  
 */
void Matter::pfd()
{
    float epsilon = 1e-5;
    //only 4 direction
    for(int i = 0; i<8; i++)
    {
        if(abs(this->receive[i])<epsilon) this->receive[i] = 0;
        if(this->receive[i]>0) this->give[i] -= this->receive[i];
        else this->give[(i+4)%8] += this->receive[i];
    }
    for(int i = 0; i<8; i++) if(this->give[i]<0)
    {
        this->give[(i+4)%8] += this->give[i];
        this->give[i] = 0;
    }
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
            for(int i = pos-2; i<pos+3; i++) this->give[(i+pos)%8] += (3-abs(pos))*this->give[pos]/8;//(7x/8)Small loss
            this->give[pos] /= 2;
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
    for(int raw = 0; raw<this->height; raw++)
    {
        for(int col = 0; col<this->width; col++)
        {
            if(this->mat[raw][col].drop)//If a drop, look forces around
            {
                this->mat[raw][col].acceleration = {0, 0};//Evaluate at each step
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
                    //Boundary conditions
                    if((raw+sraw)<0) this->mat[raw][col].receive[i] = 0;
                    else if((raw+sraw)>=this->height) this->mat[raw][col].receive[i] -= 0;
                    else if ((col+scol)<0) this->mat[raw][col].receive[i] -= 0;
                    else if((col+scol)>=this->width) this->mat[raw][col].receive[i] -= 0;
                    else 
                    {   
                        if(i>3 && i!=7) this->mat[raw][col].receive[(i+4)%8] += 2-abs(i-5);
                        if(i>3 && i!=7) this->mat[raw][col].acceleration.y += 1;//weight along y
                        
                        if(this->mat[raw+sraw][col+scol].drop)//Give its force
                        {
                            //Project on the right vector
                            float angl = atan2(scol, sraw) - atan2(this->mat[raw+sraw][col+scol].acceleration.y, 
                                                                            this->mat[raw+sraw][col+scol].acceleration.x);
                            if((abs(angl))<3.1415)//If force is directed to this cell, transmit
                            {
                                this->mat[raw][col].acceleration.x += this->mat[raw+sraw][col+scol].acceleration.x;
                                this->mat[raw][col].acceleration.y += this->mat[raw+sraw][col+scol].acceleration.y;
                                this->mat[raw+sraw][col+scol].acceleration = {0, 0};
                            }
                            this->mat[raw][col].receive[i] += this->mat[raw+sraw][col+scol].give[(i+4)%8];
                            this->mat[raw+sraw][col+scol].give[(i+4)%8] = 0;
                        }
                        else this->mat[raw][col].receive[i] += 0;
                    }
                }
                //Should check if this force can be used or must be distributed
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
            if(this->mat[raw][col].drop) 
                this->mat[raw][col].pfd();
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
