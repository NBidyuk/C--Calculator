#pragma once
#include <windows.h>
#include <string.h>
#include <iostream>

using namespace std;

void Russian(const char * str);

class Exception
{
	int position_mistake;
	static char Message[23][150];
	int index_string;
public:
	Exception(int index, int number) :position_mistake(number), index_string(index)
	{
	}
	void Print()
	{
		Russian("\nПозиция ");
		cout << position_mistake << endl;
		Russian(Message[index_string]);//сообщение об ошибке
	}
};

