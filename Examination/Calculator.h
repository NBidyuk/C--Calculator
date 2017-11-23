#pragma once
#include "Stack.h"
#include "Exception.h"

class Calculator
{
	char* string;//������ ����������� �� �������� ���� ������:��������� � ���������
	int length;//����� ������
	bool expression_state;//��������� ���������: ���� �� � �� ������
	double number;//������ ����� �����, ���������� � ���� �����
	bool construct_num;//��������� �������� ��������������� �����, ���������� �������������� �� � ������ ������ �����
	double tenth;//����������� �������
	bool fractional;//���������� ������������, ������� �� �������������� �����, �� ���� ���� �� � �� ���������� �����
	double result;//��������� ���������
	static char List[7][100];
	Stack<char> signs;//���� ������
	Stack<double> numbers;//���� �����
	void Multiply();
	void Divide(int symbol_number);
	void Pow(int symbol_number);
	void Sum();
	void Minus();
	
public:
	
	Calculator();
	~Calculator();
	void SetString(char* str);//���������� ������ � �����
	bool GetExpressionState();
	double GetResult();
	
	void PrintOperations();
	void CheckMistakes();
	void MakeCalculations();
};



