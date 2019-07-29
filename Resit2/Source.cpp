// Die Rolling Program.

#include <iostream> /* Include printf() */
#include<random>

using namespace std;

int iDie;

char cKeypressed;

/* int main()
Program entry point */

int main()
{
	/* Let the computer pick a random number */
	random_device rd; // non-deterministic engine
	mt19937 gen{ rd() }; // deterministic engine. For most common uses, ←-
	uniform_int_distribution<> dis{ 1, 6 };

	do {
		iDie = dis(gen);
		cout << "You rolled a " << iDie << "\n";
		cout << "Do you want to try again? Y/N.\n";
		cin >> cKeypressed;

	} while (cKeypressed == 'y' || cKeypressed == 'Y');
	system("PAUSE");
	return 0;
}