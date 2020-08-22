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
}

Reader::~Reader()
{
    this->inFile.close();
    this->text = {};
}

void Reader::spliter(std::string word)
{

    for(int line = 0; line < 8; line++)
    {
        this->text.push_back("");
        for(int i = 0; i<word.size(); i++) 
        {
            switch(word[i])
            {
                case 'A':
                    search(false, 1, line);
                break;
                case 'B':
                    search(false, 2, line);
                break;
                case 'C':
                    search(false, 3, line);
                break;
                case 'D':
                    search(false, 4, line);
                break;
                case 'E':
                    search(false, 5, line);
                break;
                case 'F':
                    search(false, 6, line);
                break;
                case 'G':
                    search(false, 7, line);
                break;
                case 'H':
                    search(false, 8, line);
                break;
                case 'I':
                    search(false, 9, line);
                break;
                case 'J':
                    search(false, 10, line);
                break;
                case 'K':
                    search(false, 11, line);
                break;
                case 'L':
                    search(false, 12, line);
                break;
                case 'M':
                    search(false, 13, line);
                break;
                case 'N':
                    search(false, 14, line);
                break;
                case 'O':
                    search(false, 15, line);
                break;
                case 'P':
                    search(false, 16, line);
                break;
                case 'Q':
                    search(false, 17, line);
                break;
                case 'R':
                    search(false, 18, line);
                break;
                case 'S':
                    search(false, 19, line);
                break;
                case 'T':
                    search(false, 20, line);
                break;
                case 'U':
                    search(false, 21, line);
                break;
                case 'V':
                    search(false, 22, line);
                break;
                case 'W':
                    search(false, 23, line);
                break;
                case 'X':
                    search(false, 24, line);
                break;
                case 'Y':
                    search(false, 25, line);
                break;
                case 'Z':
                    search(false, 26, line);
                break;
                case 'a':
                    search(true, 1, line);
                break;
                case 'b':
                    search(true, 2, line);
                break;
                case 'c':
                    search(true, 4, line);
                break;
                case 'd':
                    search(true, 3, line);
                break;
                case 'e':
                    search(true, 5, line);
                break;
                case 'f':
                    search(true, 6, line);
                break;
                case 'g':
                    search(true, 7, line);
                break;
                case 'h':
                    search(true, 8, line);
                break;
                case 'i':
                    search(true, 9, line);
                break;
                case 'j':
                    search(true, 10, line);
                break;
                case 'k':
                    search(true, 11, line);
                break;
                case 'l':
                    search(true, 12, line);
                break;
                case 'm':
                    search(true, 13, line);
                break;
                case 'n':
                    search(true, 14, line);
                break;
                case 'o':
                    search(true, 15, line);
                break;
                case 'p':
                    search(true, 16, line);
                break;
                case 'q':
                    search(true, 17, line);
                break;
                case 'r':
                    search(true, 18, line);
                break;
                case 's':
                    search(true, 19, line);
                break;
                case 't':
                    search(true, 20, line);
                break;
                case 'u':
                    search(true, 21, line);
                break;
                case 'v':
                    search(true, 22, line);
                break;
                case 'w':
                    search(true, 23, line);
                break;
                case 'x':
                    search(true, 24, line);
                break;
                case 'y':
                    search(true, 25, line);
                break;
                case 'z':
                    search(true, 26, line);
                break;
                default:
                    for(int i = 0; i<5; i++) this->text[line] += ' ' ;
                    if(line == 1) titleSize.width += 5;
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
void Reader::search(bool lcase, int letter, int line)
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
            if(x!='N') this->text[line] += x;
            if((line == 1) && (x!='N')) titleSize.width++;
        }
    }
}