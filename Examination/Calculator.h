#pragma once
#include "Stack.h"
#include "Exception.h"

class Calculator
{
	char* string;//строка разбиваемая на элементы двух стеков:числового и знакового
	int length;//длина строки
	bool expression_state;//состояние выражения: есть ли в нём ошибка
	double number;//каждое новое число, помещаемое в стек чисел
	bool construct_num;//состояние процесса конструирования числа, показывает конструируется ли в данный момент число
	double tenth;//сохраняемые разряды
	bool fractional;//переменная показывающая, дробное ли конструируемое число, то есть есть ли в нём десятичная точка
	double result;//результат выражения
	static char List[7][100];
	Stack<char> signs;//стек знаков
	Stack<double> numbers;//стек чисел
	void Multiply();
	void Divide(int symbol_number);
	void Pow(int symbol_number);
	void Sum();
	void Minus();
	
public:
	
	Calculator();
	~Calculator();
	void SetString(char* str);//добавление строки в класс
	bool GetExpressionState();
	double GetResult();
	
	void PrintOperations();
	void CheckMistakes();
	void MakeCalculations();
};



