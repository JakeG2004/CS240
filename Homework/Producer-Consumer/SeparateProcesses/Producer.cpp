#include <iostream>
#include <fstream>

using namespace std;

void Produce(string);

int main()
{
    srand(time(NULL));

    //repeat forever
    while(true)
    {
        Produce("file.txt");
    }
}

void Produce(string fileName)
{
    //open the file
    ofstream myFile;
    myFile.open(fileName, std::ios::app);

    //build string of 10 characters
    string newString;

    for(int i = 0; i < 10; i++)
    {
        newString += (char)((rand() % 26) + 'a');
    }

    newString += '\n';

    //say what is being written, wriite it, and close the file
    cout << "PRODUCER: Writing " << newString << " to file." << endl;

    myFile << newString;

    myFile.close();
}