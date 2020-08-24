#include <matrix.hpp>

Matrix::Matrix(int height, int width, Coord cd, int waterLvl)
{
    this->mat = {};
    for(int raw = 0; raw < height; raw++)
    {
        this->mat.push_back(new std::vector<bool>);
        for(int col = 0; col < width; col++)
        {
            if((raw==cd.raw)&&(col==cd.col)) this->mat[raw]->push_back("true");
            else if (raw >= waterLvl) this->mat[raw]->push_back("true");
            else this->mat[raw]->push_back("false");
        }
    }
}

Matrix::~Matrix()
{
    for(int i = 0; i<this->mat.size(); i++)
    {
        delete this->mat[i];
        this->mat[i] = NULL;
    }
}