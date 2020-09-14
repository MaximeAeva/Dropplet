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
 * @brief Update forces and give its direction
 * 
 * @param wallLoss Loss against a wall
 * @param wall Where is the wall
 */
void Matter::pfd(float wallLoss, float timeLoss, int wall = 0)
{
    //Add force received from each direction
    float epsilon = 1e-3;
    float x, y;
    for(int i = 0; i<8; i++)
    {
        switch(i)
        {
            case 0:
                x = -1/sqrt(2);
                y = -1/sqrt(2);
            break;
            case 1:
                x = -1;
                y = 0;
            break;
            case 2:
                x = -1/sqrt(2);
                y = 1/sqrt(2);
            break;
            case 3:
                x = 0;
                y = 1;
            break;
            case 4:
                x = 1/sqrt(2);
                y = 1/sqrt(2);
            break;
            case 5:
                x = 1;
                y = 0;
            break;
            case 6:
                x = 1/sqrt(2);
                y = -1/sqrt(2);
            break;
            case 7:
                x = 0;
                y = -1;
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
    else angle = floor(atan2(this->force.x, this->force.y)/(3.1415/8));
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
            if(this->force.x>0) 
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
            if(this->force.x<0) 
            {
                this->force.x *= -c;
                this->force.y *= c;
            }
        break;
        case 11:
            if(this->force.x>0) this->force.x *= -c;
            if(this->force.y<0) this->force.y *= -c;
        break;
        case 21:
            if(this->force.x>0) this->force.x *= -c;
            if(this->force.y>0) this->force.y *= -c;
        break;
        case 12:
            if(this->force.x<0) this->force.x *= -c;
            if(this->force.y<0) this->force.y *= -c;
        break;
        case 22:
            if(this->force.x<0) this->force.x *= -c;
            if(this->force.y>0) this->force.y *= -c;
        break;
    }
}

/*#############################Second Class#############################*/  

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
                //p.weight = 10;//Drop weight
                //p.force.y += 20;
                v.push_back(p);
            }
           else if((raw==cd.raw)&&(col==(cd.col+1))) 
            {
                Matter p(true);
                //p.weight = 10;//Drop weight
                //p.force.y += 20;
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
        this->mat.push_back(v);//Matter matrix
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

    for(int i = 0; i<=time; i++)
    {
        //std::cout << "1";
        Gravity(gravity);//Make them fall
        updateGives(wallLoss, timeLoss);
        Transmission(transmission, loss);//Update external forces from gives
        updateGives(wallLoss, timeLoss);//Update movement direction
        //Tension(fluidTension);//Update internal fluid tensions
        //updateGives(wallLoss, timeLoss);//Update movement direction
        updatePositions(t);//Update positions
        std::cout << this->totalStrenght();
    }
}

/**
 * @brief Gravity simulation
 * 
 * @param force Regarding matter's weight 
 */
void Matrix::Gravity(float force = 1)
{
    for(int raw = 0; raw<this->height; raw++)
    {
        for(int col = 0; col<this->width; col++)
        {
            if(this->mat[raw][col].drop && !this->mat[raw][col].moved)//If a drop, look forces around
            {
                this->mat[raw][col].receive[1] += force*this->mat[raw][col].weight;
                for(int i = 0; i<8; i++)
                {  
                    int sraw = theSwitcher(i, true);
                    int scol = theSwitcher(i, false);

                    //Bound
                    if((raw+sraw)<0 || (raw+sraw)>=this->height || (col+scol)<0 || (col+scol)>=this->width)
                    {   
                        //Counter reaction
                        
                        if(i==5) this->mat[raw][col].receive[i] += force*this->mat[raw][col].weight/2.0;
                        else if(abs(5-i)==1) this->mat[raw][col].receive[i] += force*this->mat[raw][col].weight/(2*sqrt(2));
                    }
                    //Interaction force
                    else if(this->mat[raw+sraw][col+scol].drop)
                    {
                        //Counter reaction
                        if(i==5) this->mat[raw][col].receive[i] += force*this->mat[raw][col].weight/2.0;
                        else if(abs(5-i)==1) this->mat[raw][col].receive[i] += force*this->mat[raw][col].weight/(2*sqrt(2));
                    }
                }
            }
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
void Matrix::Transmission(float transmission, float loss)
{
    for(int raw = 0; raw<this->height; raw++)
    {
        for(int col = 0; col<this->width; col++)
        {
            if(this->mat[raw][col].drop && !this->mat[raw][col].moved)//If a drop, look forces around
            {
                float initx = this->mat[raw][col].force.x;
                float inity = this->mat[raw][col].force.y;
                float initf = this->mat[raw][col].strenght();
                for(int i = 0; i<8; i++)
                {  
                    int sraw = theSwitcher(i, true);
                    int scol = theSwitcher(i, false);
                    if((raw+sraw)<0 || (raw+sraw)>=this->height || (col+scol)<0 || (col+scol)>=this->width){}
                    else if(this->mat[raw+sraw][col+scol].drop)
                    {
                        float k;
                        //Keep the norm
                        if(((i+4)%8)==this->mat[raw+sraw][col+scol].giveDir) k=1;
                        else k = 1/sqrt(2);
                        //Receive
                        this->mat[raw][col].receive[i] += transmission*k*this->mat[raw+sraw][col+scol].give[(i+4)%8];
                        //Give
                        this->mat[raw][col].receive[i] += transmission*this->mat[raw][col].give[i];
                    }
                }
                //for(int i = 0; i<8; i++) std::cout << this->mat[raw][col].receive[i] << ",";
                //std::cout << std::endl;
            }
        }       
    }
}

/**
 * @brief Update fluid tension
 * 
 * @param fluidTension coeff of tension
 */
void Matrix::Tension(float fluidTension)
{
    for(int raw = 0; raw<this->height; raw++)
    {
        for(int col = 0; col<this->width; col++)
        {
            if(this->mat[raw][col].drop && !this->mat[raw][col].moved)//If a drop, look forces around
            {
                for(int i = 0; i<8; i++)
                {
                    int sraw = theSwitcher(i, true);
                    int scol = theSwitcher(i, false);
                    if((raw+sraw)<0 || (raw+sraw)>=this->height || (col+scol)<0 || (col+scol)>=this->width){}
                    else if(this->mat[raw+sraw][col+scol].drop)
                    {
                        /*//Internal fluid tension
                        this->mat[raw][col].receive[(i+4)%8] += fluidTension*this->mat[raw+sraw][col+scol].give[i];
                        //Internal fluid loss
                        this->mat[raw+sraw][col+scol].receive[i] += fluidTension*this->mat[raw+sraw][col+scol].give[i];*/
                    }
                }
            }
        }       
    }
}

/**
 * @brief Update forces and set new give
 * 
 * @param wallLoss Loss energy during a collision with a wall
 * @param timeLoss Vanishing energy over time
 */
void Matrix::updateGives(float wallLoss, float timeLoss)
{
    resetGive();
    for(int raw = 0; raw<this->height; raw++)
    {
        for(int col = 0; col<this->width; col++)
        {
            if(this->mat[raw][col].drop && !this->mat[raw][col].moved) 
            {
                int wall = 0;
                if((raw-1)<0) wall += 1;
                else if((raw+1)>=this->height) wall += 2;
                if((col-1)<0) wall += 10;
                else if((col+1)>=this->width) wall += 20;
                this->mat[raw][col].pfd(wallLoss, timeLoss, wall);
            }
        }
    }
    resetReceive();
}

/**
 * @brief Move each matter along it force vector
 * 
 */
void Matrix::updatePositions(bool sens)
{
    std::vector<int> neighbours;
    for(int raw = 0; raw<this->height; raw++)
    {
        for(int col = 0; col<this->width; col++)
        {
            if(sens) col = this->width-1-col;
            //Look for neighbourhood
            if(this->mat[raw][col].drop && !(this->mat[raw][col].moved))
            {
                neighbours = {};
                for(int i = 0; i<8; i++)
                {
                    int sraw = theSwitcher(i, true);
                    int scol = theSwitcher(i, false);
                    //Boundary neighboors (nothing goes out)
                    if((raw+sraw)<0) neighbours.push_back(1);
                    else if((raw+sraw)>=this->height) neighbours.push_back(1);
                    else if ((col+scol)<0) neighbours.push_back(1);
                    else if((col+scol)>=this->width) neighbours.push_back(1);
                    else
                    {
                        if(this->mat[raw+sraw][col+scol].drop) neighbours.push_back(1);
                        else neighbours.push_back(0);
                    }
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
    resetMoved();
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
            {
                this->mat[raw][col].moved = false; 
                //std::cout << this->mat[raw][col].strenght() << " ";
            }

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

/**
 * @brief Sum of norms
 * 
 * @return int 
 */
int Matrix::totalStrenght()
{
    int k = 0;
    for(int raw = 0; raw<this->height; raw++)
    {
        for(int col = 0; col<this->width; col++)
        {
            if(this->mat[raw][col].drop) 
                k += this->mat[raw][col].strenght();
        }
    }
    return k;
}
