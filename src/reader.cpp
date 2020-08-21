#include <reader.hpp>

Reader::Reader(std::string word)
{   
    this->inFile.open("./res/letters.txt");
    if (!this->inFile) 
    {
        std::cout << "Problem" << std::endl;
        exit(1); 
    }
    spliter(word);
    std::cout << this->text << std::endl;
}

Reader::~Reader()
{
    this->inFile.close();
    this->text= "";
}

void Reader::spliter(std::string word)
{
    bool end = false;

    for(int line = 0; line < 8; line++)
    {
        for(int i = 0; i<word.size(); i++) 
        {
            
            if((i+1)==word.size()) end = true;
            else end = false;

            switch(word[i])
            {
                case 'A':
                    search(false, 1, line, end);
                break;
                case 'B':
                    search(false, 2, line, end);
                break;
                case 'C':
                    search(false, 3, line, end);
                break;
                case 'D':
                    search(false, 4, line, end);
                break;
                case 'E':
                    search(false, 5, line, end);
                break;
                case 'F':
                    search(false, 6, line, end);
                break;
                case 'G':
                    search(false, 7, line, end);
                break;
                case 'H':
                    search(false, 8, line, end);
                break;
                case 'I':
                    search(false, 9, line, end);
                break;
                case 'J':
                    search(false, 10, line, end);
                break;
                case 'K':
                    search(false, 11, line, end);
                break;
                case 'L':
                    search(false, 12, line, end);
                break;
                case 'M':
                    search(false, 13, line, end);
                break;
                case 'N':
                    search(false, 14, line, end);
                break;
                case 'O':
                    search(false, 15, line, end);
                break;
                case 'P':
                    search(false, 16, line, end);
                break;
                case 'Q':
                    search(false, 17, line, end);
                break;
                case 'R':
                    search(false, 18, line, end);
                break;
                case 'S':
                    search(false, 19, line, end);
                break;
                case 'T':
                    search(false, 20, line, end);
                break;
                case 'U':
                    search(false, 21, line, end);
                break;
                case 'V':
                    search(false, 22, line, end);
                break;
                case 'W':
                    search(false, 23, line, end);
                break;
                case 'X':
                    search(false, 24, line, end);
                break;
                case 'Y':
                    search(false, 25, line, end);
                break;
                case 'Z':
                    search(false, 26, line, end);
                break;
                case 'a':
                    search(true, 1, line, end);
                break;
                case 'b':
                    search(true, 2, line, end);
                break;
                case 'c':
                    search(true, 4, line, end);
                break;
                case 'd':
                    search(true, 3, line, end);
                break;
                case 'e':
                    search(true, 5, line, end);
                break;
                case 'f':
                    search(true, 6, line, end);
                break;
                case 'g':
                    search(true, 7, line, end);
                break;
                case 'h':
                    search(true, 8, line, end);
                break;
                case 'i':
                    search(true, 9, line, end);
                break;
                case 'j':
                    search(true, 10, line, end);
                break;
                case 'k':
                    search(true, 11, line, end);
                break;
                case 'l':
                    search(true, 12, line, end);
                break;
                case 'm':
                    search(true, 13, line, end);
                break;
                case 'n':
                    search(true, 14, line, end);
                break;
                case 'o':
                    search(true, 15, line, end);
                break;
                case 'p':
                    search(true, 16, line, end);
                break;
                case 'q':
                    search(true, 17, line, end);
                break;
                case 'r':
                    search(true, 18, line, end);
                break;
                case 's':
                    search(true, 19, line, end);
                break;
                case 't':
                    search(true, 20, line, end);
                break;
                case 'u':
                    search(true, 21, line, end);
                break;
                case 'v':
                    search(true, 22, line, end);
                break;
                case 'w':
                    search(true, 23, line, end);
                break;
                case 'x':
                    search(true, 24, line, end);
                break;
                case 'y':
                    search(true, 25, line, end);
                break;
                case 'z':
                    search(true, 26, line, end);
                break;
                default:
                    this->text += '         ';
                break;
            }
        }
    }
}

/**
 * @brief Read a line from a letter
 * 
 * @param lcase capital or not
 * @param letter letter number
 * @param line line to read
 * @param end last letter of the word ?
 */
void Reader::search(bool lcase, int letter, int line, bool end = false)
{
    char c = 's';
    int l = 0;
    if(lcase) c = 'p';
    char x;
    int n = 0;
    this->inFile.seekg(0);
    while (this->inFile >> std::noskipws >> x)  
    {
        if(x=='e') break;
        if(x==c)
        {
            n++;
            if(!(n%27)) l++;
            x = 'N';
        }
        if(((n%27)==(letter))&&(l==line)) 
        {
            if(x!='N') this->text += x;
        }
    }
    if(end) this->text += '\n';
}