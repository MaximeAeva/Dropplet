#include <reader.hpp>

Reader::Reader(std::string word)
{   
    this->inFile.open("./res/letters.txt");
    if (!this->inFile) 
    {
        std::cout << "Problem" << std::endl;
        exit(1); 
    }
    char x;
    int n = 0;
    while (this->inFile >> std::noskipws >> x)  
    {
        if(x=='t')
        {
            n=0;
            x='\n';
        }
        if(x=='p')
        {
            n++;
            x='\n';
        }
            
        if((n%27)==1)
            this->text = this->text + x;
    }
    std::cout << this->text << std::endl;
}

Reader::~Reader()
{
    this->inFile.close();
    this->text= "";
}

std::vector<int> youMean(syd::string word)
{
    
}