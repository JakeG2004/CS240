#include <iostream>
#include <fstream>

using namespace std;

void Consume(string);

int main()
{
    //repeat forever
    while(true)
    {
        Consume("file.txt");
    }
}

void Consume(string fileName)
{
    //strings to read in file
    string line = "";
    string contents = "";

    //open the file
    ifstream inFile(fileName);

    //check for errors
    if(!inFile)
    {
        cout << "Failed to open file." << endl;
        return;
    }

    //write to contents
    while(getline(inFile, line))
    {
        //store only nonempty lines
        if(line != "")
        {
            contents += line + '\n';
        }
    }

    inFile.close();

    //check for empty contents
    if(contents == "")
    {
        cout << "Empty contents..." << endl;
        return;
    }

    //empty and save file
    cout << "CONSUMER: Consuming " << contents << endl;
    ofstream outFile("file.txt");
    outFile << "";
    outFile.close();
}