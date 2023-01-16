#include "pingfunctions.h";

int WordsInString(string str)
{
    int counter = 0;
    string temp= " ";
    
    while (temp != "")
    {
        counter++;
        temp = ExtractWord(str, counter);
        
    }
    counter--;
    return counter;
}

string ExtractWord(string str, int counter)
{
    stringstream ss;
    string temp, exwords;

    ss << str;

    while (!ss.eof() && counter > 0)
    {
        /* extracting word by word from stream */
        ss >> temp;
        counter--;
        if (counter == 0)
        {
            exwords = temp;
        }
        temp = "";
    }

    return exwords;
}

int YesOrNo(string ch)
{
    int temp;
    if (ch == "N" || ch == "n" || ch == "no" || ch == "NO")temp = 0;
    else if (ch == "Y" || ch == "y" || ch == "yes" || ch == "YES")temp = 1;
    else temp = 2;
    return temp;
}


int ExtractInt(string str, int counter)
{
    stringstream ss;
    string temp;
    int found, num = 0;

    ss << str;

    while (!ss.eof() && counter > 0)
    {
        /* extracting word by word from stream */
        ss >> temp;
        /* Checking the given word is integer or not */
        if (stringstream(temp) >> found)
        {
            num = found;
            counter--;
        }
        /* To save from space at the end of string */
        temp = "";
    }

    return num;
}