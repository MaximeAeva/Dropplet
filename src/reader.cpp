#include <reader.hpp>

Reader::Reader()
{   
    this->inFile.open("./res/letters.txt");
    if (!this->inFile) 
    {
        std::cout << "Problem" << std::endl;
        exit(1); 
    }
    char x;
    while (this->inFile >> x)  
    {
        this->text = this->text + x;
    }
    std::cout << this->text << std::endl;
    this->text = "";
    for(int i = 1; i < 3; i++)
    {
        std::getline(inFile, this->text);
        std::cout << this->text << std::endl;
    }
    
}

Reader::~Reader()
{
    this->inFile.close();
    this->text= "";
}