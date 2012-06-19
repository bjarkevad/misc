#include "container_assign.hpp"
#include <iostream>
#include <list>
#include <map>
#include <vector>
#include <string>

using namespace std;

int list_pop(list<int>& l)
{
	int i = l.front();
	l.pop_front();
	return i;
}

int main()
{
	vector<int> ivec;
	ivec += 1, 2, 3;
	ivec += 4, 5, 6, 7, 8, 9;
	cout << "[ " << ivec[0] << ", " << ivec[1] << ", " << ivec[2] << ", ";
	cout << ivec[3] << ", " << ivec[4] << ", " << ivec[5] << ", ";
	cout << ivec[6] << ", " << ivec[7] << ", " << ivec[8] << " ]\n";
	int j = 0;
	j += 3;
	cout << j << "\n";
	list<int> ilist;
	ilist += 1, 2, 3;
	cout << "[ " << list_pop(ilist) << ", " << list_pop(ilist) << ", " << list_pop(ilist) << " ]\n";
	vector<string> svec;
	svec += "January", "February", "March", "April";
	cout << "[ " << svec[0] << ", " << svec[1] << ", " << svec[2] << ", " << svec[3] << " ]\n";
	map<int,string> ismap;
	ismap += pair<int,string>(1, "Foo"), pair<int,string>(2, "Bar");
	cout << "[ {1, " << ismap[1] << "}, {2, " << ismap[2] << "} ]\n";
	return 0;
}

