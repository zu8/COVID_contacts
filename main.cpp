#include "infected.h"
#include<windows.h>
unsigned int Interval = 7;

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	list <Tgroup> mylist = createGroups("input.txt");
	expandGroups(mylist, Interval);
	writeMaxGroup(mylist, "output.txt");
	cin.get();
	return 0;
}