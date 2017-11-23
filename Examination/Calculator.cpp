#include "Calculator.h"

char Calculator::List[7][100] =
{
	"�������� ���������� �������� � �������:\n",
	"\'-\'\t- ������� �����",
	"\'+\'\t- ��������",
	"\'-\'\t- ���������",
	"\'*\'\t- ���������",
	"\'/\'\t- �������",
	"\'^\'\t- ���������� � �������"
};

Calculator::Calculator() :string(nullptr), length(0), expression_state(true), number(0), construct_num(false), tenth(1.0), fractional(false),
result(0)
{
}

void Calculator::SetString(char* str)
{
	length = strlen(str) + 3;
	string = new char[length];
	string[0] = '(';
	for (int i = 1; i < length - 2; i++)
	{
		string[i] = str[i - 1];
	}
	string[length - 2] = ')';//����� �������� � ������ � � ����� ������
	string[length - 1] = 0;
}

bool Calculator::GetExpressionState()
{
	return expression_state;
}

double Calculator::GetResult()
{
	return result;
}

Calculator::~Calculator()
{
	delete[]string;
}

void Calculator::Multiply()
{
	double number2;
	double number1;
	if (numbers.Pop(number2) && numbers.Pop(number1))
	{
		result = number2*number1;
		numbers.Push(result);
	}
}

void Calculator::Divide(int symbol_number)
{
	double number2 = 0;
	double number1 = 0;
	if (numbers.Pop(number2) && numbers.Pop(number1))
	{
		if (number2)
		{
			result = 1 / number2*number1;
			numbers.Push(result);
		}
		else
		{
			int k = 0;
			do
			{
				k++;
			} while (string[symbol_number - k] != '/');
			throw Exception(18, symbol_number - k - 1);//������ �� ���� ������, ���������� ����� ������� ����� ������
			//����� 1 ����� �� ��������� ������ ������������� ������ ��� ������� ������� ������
		}
	}
}

void Calculator::Pow(int symbol_number)
{
	double number2 = 0;
	double number1 = 0;
	if (numbers.Pop(number2) && numbers.Pop(number1))
	{
		result = pow(number1, number2);
		if (errno == EDOM ||(number1==0&&number2<0))
		{
			int k = 0;
			do
			{
				k++;
			} while (string[symbol_number - k] != '^');
			throw Exception(19, symbol_number - k - 1);//�������� 1 ����� �� ��������� ����� ������ ������������� ������ (������� ���� ���������) ��� ������� ������� ������
		}
		numbers.Push(result);
	}
}

void Calculator::Sum()
{
	double number2 = 0;
	double number1 = 0;
	if (numbers.Pop(number2) && numbers.Pop(number1))
	{
		result = number2 + number1;
		numbers.Push(result);
	}
}

void Calculator::Minus()
{
	double number2 = 0;
	double number1 = 0;
	if (numbers.Pop(number2) && numbers.Pop(number1))
	{
		result = -number2 + number1;
		numbers.Push(result);
	}
}

void Calculator::CheckMistakes()
{
	char read_symbol;
	for (int i = 0; i < length - 1; i++)
	{
		read_symbol = string[i];
		if (read_symbol == ' ')continue;//���������� �������
		int k = 0;
		do
		{
			k++;
		} while (string[i + k] == ' ');//��� �������� ����� ���������� �� read_symbol, �� �������� �������(��� ����� ���������� ��� �������� �� ������)
		//��� ��� �� ��������� ��������� � ������ ����������, �� �������������� ������� ��� �� �����������
		char next_symbol = string[i + k];
		int g = 0;
		do
		{
			g++;
		} while (string[i - g] == ' ');//��� �������� ����� ������� �� read_symbol, �� �������� �������
		char previous_symbol = string[i - g];
		if (read_symbol >= '0'&&read_symbol <= '9' || read_symbol == '.')
		{
			continue;
		}
		if (read_symbol == '(')
		{
			if (i != 0 && previous_symbol != '+'&&previous_symbol != '-'&&previous_symbol != '*'
				&&previous_symbol != '/'&&previous_symbol != '^'&&previous_symbol != '(')
				//��� ��� �������� ������� ������ ����� ������������� �������
			{
				throw Exception(7, i - 1);//���� ������ ����� ������� ���, ��������� � �������������� ������
			}
			if (next_symbol == '*' ||
				next_symbol == '/' || next_symbol == '^')
			{
				if (i == 0)	throw Exception(13, i);//��������� ���������� �� ����� �������� ��������, - � + � �� ��������, ������ ��� �������� ����������� ������ ����� ������� � +,-
				else throw Exception(10, i - 1);//����� �������� ������� � ������ �������� �������� ����������� ���������
			}
		}
		else if (read_symbol == ')')
		{
			if (previous_symbol == '('&&i != length - 2)//���� ������ �� ����� ������������� ������ �� �������� �������,������ ���������� ��������� ����� ��������
				//��� ���� ����� ���������.��� ���� ������������� ������ � ����� ������, �� ��� ��� ������ ������, ���� ��������� �������������
				//������ ������������ �� �����
			{
				if (i - g == 0)	throw Exception(14, i - 1);//��������� ���������� � ������������� ������
				else throw Exception(9, i - 2);//��� ��������� ����� ��������, -2 ����� ��������� ������� ������������� ������, � �� �������������
			}
			if (i < length - 2 && next_symbol != '+'&&next_symbol != '-'&&next_symbol != '*'
				&&next_symbol != '/'&&next_symbol != '^'&&next_symbol != ')')
				//��� �������� ������� ������ ����� ������������� ������
			{
				if (next_symbol == '(')	throw Exception(12, i - 1);//������ ���������� ����� �������� �������� ����� ������������� � ������������� �������
				else throw Exception(8, i - 1);//����� ������������� ������� � ������ ���������� ���� �������� ��������
			}
			if (previous_symbol == '+' || previous_symbol == '-' || previous_symbol == '*' ||
				previous_symbol == '/' || previous_symbol == '^')
			{
				if (i == length - 2) throw Exception(15, i - 2);//��������� ������������� ������ �������� ��������
				else throw Exception(11, i - 1);//����������� ��������� ����� ������ �������� � ������� �������������
			}

		}
		else if (read_symbol != '*'&&read_symbol != '/'&&read_symbol != '^'&&read_symbol != '+'&&read_symbol != '-')
		{
			throw Exception(0, i - 1);//���� ���������� ������
		}
		else if (next_symbol == '*' || next_symbol == '/' || next_symbol == '^' || next_symbol == '+' || next_symbol == '-')//������,���� ������ ��� �����, � ��� �� ������������� ������
		{
			throw Exception(1, i - 1);
		}
	}
}

void Calculator::PrintOperations()
{
	Russian("������� �������������� ���������. ");
	for (int i = 0; i < 7; i++)
	{
		Russian(Calculator::List[i]);
		cout << '\n';
	}
}

void Calculator::MakeCalculations()
{
	char read_symbol;
	for (int i = 0; i < length - 1; i++)
	{
		read_symbol = string[i];
		if (read_symbol == ' ')continue;//���������� �������
		int k = 0;
		do
		{
			k++;
		} while (string[i + k] == ' ');//��� �������� ����� ���������� �� read_symbol, �� �������� �������(��� ����� ���������� ��� �������� �� ������)
		//��� ��� �� ��������� ��������� � ������ ����������, �� �������������� ������� ��� �� �����������
		char next_symbol = string[i + k];
		if (read_symbol >= '0'&&read_symbol <= '9' || read_symbol == '.')//����� ������� �� ����������, ��� ��� �����, ������ ������ ����� ����� ���������� � �����, ��� �������� 0.
		{
			if (construct_num == false)//���� ��������� ��������������� ����, ��� ����� ���� ����� ������ ����� ��� ��������� � ����, � ����� ����� ��� �� ������ ���������
			{
				if (read_symbol != '.')
				{
					number = read_symbol - '0';
					fractional = false;
				}
				else
				{
					fractional = true;//����� ������� �� ��������,��� ����� �������
				}
				construct_num = true;//��������,���������� ������� ����������� ������ �����
				tenth = 1.0;
			}
			else
			{
				if (read_symbol == '.')
				{
					if (fractional == true)//��� �������� ������� ��� ����� ����� � ���������� �����
					{
						throw Exception(16, i - 1);//��� �������� ����� ��������� � main ��� ��� ����� �� ����� ��� ��������� fractional
					}
					fractional = true;
					if (next_symbol == '.')//��������, �� ���� �� ������ ��� ���������� ����� � �����
					{
						throw Exception(2, i - 1);
					}
				}
				else
				{
					number = number*10.0 + (read_symbol - '0');
				}
			}
			if (fractional && read_symbol != '.')//���� ����� �������� ��� �������, ��� ����� �������� �� ����� ��������
			{
				tenth /= 10.0;
			}
		}
		else//���� ����������� ������ �� ����� � �� �����
		{
			char sign_symbol;//������ �� ����� �������� ���� ��������
			if (construct_num)
			{
				if (fractional)
				{
					number *= tenth;//����������� ��������������� ����� � ��������� ��� � ����
				}
				numbers.Push(number);
				number = 0;
				construct_num = false;//������ ��������� �������� ��������������� �����
			}
			if (read_symbol == '(')
			{
				signs.Push(read_symbol);//�������� ������ �� ����� �� ����� ������
				//��� ����� ���������� ��� ������� � ��������� � ���������� ������ ����� �������� ������
				if (next_symbol == '-' || next_symbol == '+')
				{
					numbers.Push(0);//����� ���������� �������� ����� � 0-, �������� ���� � 0+
				}
			}
			else if (read_symbol == ')')
			{
				bool state_of_operation;//��� �������� ������ ���������� ������������ ����� ��� ������� �������� ������ �� ����������� ����� ����� ��������
				do
				{
					state_of_operation = signs.Pop(sign_symbol);

					if (state_of_operation)
					{
						if (sign_symbol == '*')
						{
							Multiply();
						}
						else if (sign_symbol == '/')
						{
							Divide(i);
						}
						else if (sign_symbol == '^')
						{
							Pow(i);
						}

						else if (sign_symbol == '+')
						{
							Sum();
						}
						else if (sign_symbol == '-')
						{
							Minus();
						}

						else//������ ������� ���������, ����� � ������ ��������� 1 �����
						{
							numbers.Peek(result);
							//��� �����,����� ������ ��������, ��� ������� ��������� ����� ���� 1 �����,1 ����� ����������� � ������
							//��� ���������, ������� �������� � ���� 1 ����� ����������� � ������
						}
					}
					else
					{
						throw Exception(17, i - 2);//�������� �� ����������� ������������� � ������������� ������
						//� else ����� ���������, ���� ������������� ������ ����� ������, ��� �������������
					}
				} while (sign_symbol != '(');//������ �� ����� ����� � ����� �� ��� ���, ���� �� ����� �� ��������� ������������� ������

				if (i == length - 2 && !signs.IsEmpty())//���� i �� ������ ��������� ������������� ������ � ����� ���������, � ���� ������ �������� ��� �� ����
				{
					throw Exception(17, i - 2);//�� i ���������� 2,����� �� ��������� ������ ������, �������� ������� ���������� ������� � ������
				}
			}
			else if (read_symbol == '-' || read_symbol == '+')
			{
				do
				{
					if (signs.Peek(sign_symbol) && sign_symbol == '(')
					{
						signs.Push(read_symbol);//�� ������������� ������ ����� ������ ��� ����� � ����
					}
					else
					{
						if (signs.Pop(sign_symbol) && sign_symbol == '*')
						{
							Multiply();
						}
						if (sign_symbol == '/')
						{
							Divide(i);
						}
						if (sign_symbol == '^')
						{
							Pow(i);
						}
						if (sign_symbol == '+')
						{
							Sum();
						}
						if (sign_symbol == '-')
						{
							Minus();
						}
					}
				} while (sign_symbol != '(');
			}
			else if (read_symbol == '*' || read_symbol == '/')
			{
				do
				{
					if (signs.Peek(sign_symbol) && (sign_symbol == '(' || sign_symbol == '+' || sign_symbol == '-'))
					{
						signs.Push(read_symbol);
					}
					else
					{
						if (signs.Pop(sign_symbol) && sign_symbol == '^')
						{
							Pow(i);
						}
						if (sign_symbol == '/')
						{
							Divide(i);
						}
						if (sign_symbol == '*')
						{
							Multiply();
						}
					}
				} while (sign_symbol != '('&&sign_symbol != '+'&&sign_symbol != '-');
			}
			else if (read_symbol == '^')
			{
				signs.Push('^');
			}
		}
	}
}