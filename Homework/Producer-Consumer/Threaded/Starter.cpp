#include <iostream>
#include <cstdlib>
#include <limits>
#include <fstream>

using namespace std;

int main()
{
    //start with empty file
    ofstream myFile;
    myFile.open("file.txt");
    myFile << "";
    myFile.close();

    int numProducers;
    int numConsumers;

    //get num producers
    cout << "Enter how many producers: "; 
    while (!(cin >> numProducers) || numProducers < 0) {
        cin.clear(); // Clear the error state
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
        cout << "Invalid input. Please enter a non-negative integer for producers: ";
    }

    //get num consumers
    cout << "Enter how many consumers: "; 
    while (!(cin >> numConsumers) || numConsumers < 0) {
        cin.clear(); // Clear the error state
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
        cout << "Invalid input. Please enter a non-negative integer for consumers: ";
    }

    //spawn terminals with the process running
    for(int i = 0; i < numProducers; i++) {
        system("gnome-terminal -- ./Producer.out");
    }

    for(int i = 0; i < numConsumers; i++) {
        system("gnome-terminal -- ./Consumer.out");
    }
}
