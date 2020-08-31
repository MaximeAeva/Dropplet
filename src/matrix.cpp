#include <matrix.hpp>


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
 * @brief Give the traction forces
 *  
 */
void Matter::pfd()
{
    float epsilon = 1e-5;
    float x, y;
    for(int i = 0; i<8; i++)
    {
        switch(i)
        {
            case 0:
                x = -1/sqrt(2);y = -1/sqrt(2);
            break;
            case 1:
                x = -1;y = 0;
            break;
            case 2:
                x = -1/sqrt(2);y = 1/sqrt(2);
            break;
            case 3:
                x = 0;y = 1;
            break;
            case 4:
                x = 1/sqrt(2);y = 1/sqrt(2);
            break;
            case 5:
                x = 1;y = 0;
            break;
            case 6:
                x = 1/sqrt(2);y = -1/sqrt(2);
            break;
            case 7:
                x = 0;y = -1;
            break;
        }
        this->force.x += this->receive[i]*x;
        this->force.y += this->receive[i]*y;
    }
    if(abs(this->force.x) < epsilon) this->force.x = 0;
    if(abs(this->force.y) < epsilon) this->force.y = 0;
    int angle;
    if(this->force.x == 0 && this->force.y == 0) angle = 0;
    else angle = floor(atan2(this->force.x, this->force.y)/(3.1415/8));
    int giveDir;
    switch(angle)
    {
        case -9:
            giveDir = 7;
        break;
        case -8:
            giveDir = 7;
        break;
        case -7:
            giveDir = 6;
        break;
        case -6:
            giveDir = 6;
        break;
        case -5:
            giveDir = 5;
        break;
        case -4:
            giveDir = 5;
        break;
        case -3:
            giveDir = 4;
        break;
        case -2:
            giveDir = 4;
        break;
        case -1:
            giveDir = 3;
        break;
        case 0:
            giveDir = 3;
        break;
        case 1:
            giveDir = 2;
        break;
        case 2:
            giveDir = 2;
        break;
        case 3:
            giveDir = 1;
        break;
        case 4:
            giveDir = 1;
        break;
        case 5:
            giveDir = 0;
        break;
        case 6:
            giveDir = 0;
        break;
        case 7:
            giveDir = 7;
        break;
        case 8:
            giveDir = 7;
        break;
    }
    for(int i = -1; i<2; i++) this->give[(giveDir+i)%8] = this->strenght()/3;
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
    for(int i = 0; i<8; i++)
    {
        if(this->give[i]>0)
        {
            pos.push_back(i);
        }
    }
    if(pos.empty()) return -1;
    if(!b[pos[1]]) return pos[1];//Priority to the major direction
    else if(b[pos[0]] && b[pos[2]]) return -1;//If nothing stay here
    else if(!b[pos[0]] && !b[pos[2]]) return pos[0];//Randomly go right or left
    else
    {
        if(!b[pos[0]]) return pos[0];
        else return pos[2];
    }
}

/**
 * @brief Bounce on a surface
 * 
 * @param typ Where is the wall (u->up,r->right,b->bottom,l->left) 
 * @param c Coefficient of transmission
 */
void Matter::reverseGive(char typ, float c)
{
    switch(typ)
    {
        case 'u':
            this->force.x *= -c;
        break;
        case 'l':
            this->force.y *= -c;
        break;
        case 'r':
            this->force.y *= -c;
        break;
        case 'b':
            this->force.x *= -c;
        break;
    }
}

/**
 * @brief Give Matter movement strenght
 * 
 * @return float 
 */
float Matter::strenght()
{
    return sqrt(pow(this->force.x, 2)+pow(this->force.y, 2));
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
                //p.weight = 30;//Drop weight
                //p.force.y += 30;
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
void Matrix::animate(int time, bool t)
{
    for(int i = 0; i<=time; i++)
    {
        //std::cout <<"r";
        updateReceive();//Update external forces
        //std::cout <<"Rg";
        resetGive();//No direction for now
        //std::cout <<"g";
        updateGives();//Update movement direction
        //std::cout <<"Rr";
        resetReceive();//Say forces will be reevaluated
        //std::cout <<"p";
        updatePositions(t);//Update positions
        //std::cout <<"Rm";
        resetMoved();//Say each cell is movable
    }
}

/**
 * @brief Update external forces of each matter
 * 
 */
void Matrix::updateReceive()
{
    float fluidTension = 0;//Followed forces
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
                        break;
                        case 1:
                            this->mat[raw][col].receive[i] += this->mat[raw][col].weight;
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
                    //Bound
                    if((raw+sraw)<0 || (raw+sraw)>=this->height || (col+scol)<0 || (col+scol)>=this->width)
                    {   
                        //Counter reaction
                        if(i==5) this->mat[raw][col].receive[i] += this->mat[raw][col].weight;
                    }
                    //Interaction force
                    else if(this->mat[raw+sraw][col+scol].drop)
                    {
                        //Counter reaction
                        if(i==5) this->mat[raw][col].receive[i] += this->mat[raw][col].weight;
                        //give with loss (cancel + add a bounce force)
                        this->mat[raw+sraw][col+scol].receive[(i+4)%8] += transmission*this->mat[raw][col].give[i];
                        //cancel
                        this->mat[raw][col].receive[i] += transmission*(this->mat[raw][col].give[i]);
                        //Internal fluid tension
                        this->mat[raw][col].receive[(i+4)%8] += fluidTension;
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
    float wallLoss = 0.5;//When against a border
    float timeLoss = 0;
    for(int raw = 0; raw<this->height; raw++)
    {
        for(int col = 0; col<this->width; col++)
        {
            if(this->mat[raw][col].drop) 
            {
                if((raw-1)<0) this->mat[raw][col].reverseGive('u', 1-wallLoss);
                if((raw+1)>=this->height) this->mat[raw][col].reverseGive('b', 1-wallLoss);
                if((col-1)<0) this->mat[raw][col].reverseGive('l', 1-wallLoss);
                if((col+1)>=this->width) this->mat[raw][col].reverseGive('r', 1-wallLoss);
                this->mat[raw][col].pfd();
                for(int i = 0; i<8; i++) this->mat[raw][col].give[i] *= 1-timeLoss;
            }
        }
    }
}

/**
 * @brief Move each matter along it force vector
 * 
 */
void Matrix::updatePositions(bool sens)
{
    for(int raw = 0; raw<this->height; raw++)
    {
        if(!sens)
        {
            for(int col = this->width; col>=0; col--)
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
        else
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

/**
 * @brief Evaluate forces on each step
 * 
 */
void Matrix::resetReceive()
{
    for(int raw = 0; raw<this->height; raw++)
    {
        for(int col = 0; col<this->width; col++)
        {
            if(this->mat[raw][col].drop) 
                for(int i = 0; i<8; i++) this->mat[raw][col].receive[i] = 0;
        }
    }
}

/**
 * @brief Set give to 0
 * 
 */
void Matrix::resetGive()
{
    for(int raw = 0; raw<this->height; raw++)
    {
        for(int col = 0; col<this->width; col++)
        {
            if(this->mat[raw][col].drop) 
                for(int i = 0; i<8; i++) this->mat[raw][col].give[i] = 0;
        }
    }
}
