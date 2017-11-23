#include "Stack.h"
#include "Calculator.h"
#include <iostream>
using namespace std;

void main()
{
	Calculator calculator;
	calculator.PrintOperations();
	char* string = new char[100];
	cin.getline(string, 100);
	calculator.SetString(string);
	delete[]string;
	try
	{
		calculator.CheckMistakes();
		//проверяем сперва на ошибки, кроме тех, проверка на которые находится в функции MakeCalculations
		calculator.MakeCalculations();
		cout << "\nResult is " << calculator.GetResult() << '\n';
		//тут нужен if потому что для некоторых ошибок нужно предусмотреть особую обработку
	}
	catch (Exception& object)
	{
		object.Print();
		cout << endl;
	}
}



