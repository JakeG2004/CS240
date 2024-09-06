#include <iostream>
#include <fstream>

using namespace std;

//void Produce(fstream& myFile);
//void Consume(fstream& myFile);
void Consume(string);
void clearFileContents(const std::string& filePath);

int main()
{
    int size = 0;

    Consume("file.txt");

    //while(true)
    {
        //Consume(myFile);
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

    //find last newline character + second to last if it exists
    int lastNewlinePos = contents.rfind('\n');
    int secondLastNewlinePos = contents.rfind('\n', lastNewlinePos - 1);

    //If newline is present, then erase up to it
    if(secondLastNewlinePos != -1)
    {
        contents.erase(secondLastNewlinePos);
        //cout << "Erased " << secondLastNewlinePos << " characters" << endl;
    }

    //if only one line or zero
    else
    {
        contents.clear();
        //cout << "cleared" << endl;
    }

    //save file
    ofstream outFile("file.txt");
    outFile << contents;
    outFile.close();
}