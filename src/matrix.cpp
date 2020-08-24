#include <matrix.hpp>

Matter::Matter(bool d)
{
    std::vector<double> receive(8, 0.0);
    if(d) receive[2] += 0.00005*9.8;//If water, it should weight a bit
    this->drop = d;
    this->receive = receive;
}

Matter::~Matter(){}

Matrix::Matrix(int height, int width, Coord cd, int waterLvl)
{
    this->mat = {};
    for(int raw = 0; raw < height; raw++)
    {
        std::vector<Matter *> v;
        for(int col = 0; col < width; col++)
        {
            if((raw==cd.raw)&&(col==cd.col)) 
            {
                Matter *p = new Matter(true);
                v.push_back(p);
            }
            else if(raw >= waterLvl) 
            {
                Matter *p = new Matter(true);
                v.push_back(p);
            }
            else 
            {
                Matter *p = new Matter(false);
                v.push_back(p);
            }
        }
        this->mat.push_back(v);
    }
}

Matrix::~Matrix()
{
    for(int i = 0; i<this->mat.size(); i++)
    {
        this->mat[i] = {};
    }
    this->mat = {};
}

