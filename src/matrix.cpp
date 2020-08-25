#include <matrix.hpp>


/**
 * @brief Woaw matter *-* (Construct a new Matter:: Matter object)
 * 
 * @param d Is it matter ?
 */
Matter::Matter(bool d)
{
    std::vector<double> receive(8, 0);
    this->give = receive;
    this->moved = false;
    if(d) receive[1] += 0.00005*9.8;//If water, it should weight a bit
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
void Matter::resultant()
{
    double epsilon = 1e-10;
    //only 4 direction
    for(int i = 0; i<4; i++)
    {
        double k = (this->receive[i])-(this->receive[i+4]);
        if(k<epsilon) k = 0;
        if(k>=0) this->give[i+4] = k;
        else this->give[i] = -1*k;
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
    std::vector<double> val;
    for(int i = 0; i<8; i++)
    {
        if((!b[i]) && this->give[i]>0)
        {
            pos.push_back(i);
            val.push_back(this->give[i]);
        }
    }
    //Jump
    if(val.empty())
    {
        std::vector<double> cp = this->give;
        for(int i = 0; i<8; i++) this->give[i] = cp[(i+4)%8];
    }
    //Fall
    else 
    {
        int k = 0;
        double l = 0;
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
            if(this->mat[raw][col].drop)
            {
                int sraw, scol;
                for(int i = 0; i<8; i++)
                {
                    switch(i)
                    {
                        case 0:
                            sraw = -1;scol = -1;
                            this->mat[raw][col].receive[i] += 0.25*0.00005*9.8;
                        break;
                        case 1:
                            sraw = -1;scol = 0;
                            this->mat[raw][col].receive[i] += 0.5*0.00005*9.8;
                        break;
                        case 2:
                            sraw = -1;scol = 1;
                            this->mat[raw][col].receive[i] += 0.25*0.00005*9.8;
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
                    //Boundary conditions
                    if((raw+sraw)<0) this->mat[raw][col].receive[i] += 0;
                    else if((raw+sraw)>=this->height) this->mat[raw][col].receive[i] += this->mat[raw][col].receive[i-4];
                    else if ((col+scol)<0) this->mat[raw][col].receive[i] += 0;
                    else if((col+scol)>=this->width) this->mat[raw][col].receive[i] += 0;
                    else this->mat[raw][col].receive[i] += this->mat[raw+sraw][col+scol].give[(i+4)%8];
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
            if(this->mat[raw][col].drop) 
                this->mat[raw][col].resultant();
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