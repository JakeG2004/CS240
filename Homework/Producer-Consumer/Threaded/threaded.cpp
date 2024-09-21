#include <iostream>
#include <thread>
#include <fstream>

using namespace std;

void Produce(string fileName);
void Consume(string fileName);

int main()
{
    srand(time(0));

    while(true)
    {
        //create 2 threads
        thread producer(Produce, "file.txt");
        thread consumer(Consume, "file.txt");

        //join them back in once complete
        producer.join();
        consumer.join();
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