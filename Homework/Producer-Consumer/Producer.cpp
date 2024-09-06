#include <iostream>
#include <fstream>

using namespace std;

void Produce(ofstream& myFile);

int main()
{
    srand(time(NULL));

    ofstream outFile;
    outFile.open("file.txt", std::ios::app);

    if(!outFile)
    {
        cout << "Failed to open file" << endl;
    }

    for(int i = 0; i < 10; i++)
    {
        Produce(outFile);
    }
    //Produce(myFile);
}

void Produce(ofstream& myFile)
{
    string newString;

    string tmp;

    //build a string of 10 random characters
    for(int i = 0; i < 10; i++)
    {
        newString += (char)((rand() % 26) + 'a');
    }

    newString += '\n';

    cout << "PRODUCER: Writing " << newString << " to file." << endl;

    myFile << newString;
}