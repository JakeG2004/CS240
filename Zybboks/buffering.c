#include <iostream>

using namespace std;

int main()
{
	int numData = 0;
	cout << "Enter total #: ";
	cin >> numData;

	int fillTime = 0;
	cout << "Enter fill time: ";
	cin >> fillTime;

	int emptyTime = 0;
	cout << "Enter empty time: ";
	cin >> emptyTime;

	int totalTime = (emptyTime * numData) + fillTime;
	int prodIdle = (2 * fillTime) + (emptyTime * (numData - 2));
	prodIdle -= (numData * fillTime);

	printf("total time: %i\n", totalTime);
	printf("producer Idle time: %i\n", prodIdle);
	printf("consumer Idel: %i\n", fillTime);
}
