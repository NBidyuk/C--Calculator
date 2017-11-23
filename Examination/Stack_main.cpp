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
		//��������� ������ �� ������, ����� ���, �������� �� ������� ��������� � ������� MakeCalculations
		calculator.MakeCalculations();
		cout << "\nResult is " << calculator.GetResult() << '\n';
		//��� ����� if ������ ��� ��� ��������� ������ ����� ������������� ������ ���������
	}
	catch (Exception& object)
	{
		object.Print();
		cout << endl;
	}
}



