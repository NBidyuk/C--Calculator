#include "Calculator.h"

char Calculator::List[7][100] =
{
	"Перечень допустимых операций и функций:\n",
	"\'-\'\t- унарный минус",
	"\'+\'\t- сложение",
	"\'-\'\t- вычитание",
	"\'*\'\t- умножение",
	"\'/\'\t- деление",
	"\'^\'\t- возведение в степень"
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
	string[length - 2] = ')';//нужно добавить в начале и в конце скобки
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
			throw Exception(18, symbol_number - k - 1);//делить на ноль нельзя, выводиться будет позиция конца строки
			//минус 1 чтобы не учитывать первую открывающуюся скобку при расчёте позиция ошибки
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
			throw Exception(19, symbol_number - k - 1);//отнимаем 1 чтобы не учитывать самую первую открывающуюся скобку (которую сами добавляем) при расчёте позиции ошибки
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
		if (read_symbol == ' ')continue;//пропускаем пробелы
		int k = 0;
		do
		{
			k++;
		} while (string[i + k] == ' ');//для проверки знака следующего за read_symbol, не учитывая пробелы(это будет необходимо для проверок на ошибки)
		//так как всё выражение заключено в скобки изначально, то дополнительных условий нам не потребуется
		char next_symbol = string[i + k];
		int g = 0;
		do
		{
			g++;
		} while (string[i - g] == ' ');//для проверки знака идущего до read_symbol, не учитывая пробелы
		char previous_symbol = string[i - g];
		if (read_symbol >= '0'&&read_symbol <= '9' || read_symbol == '.')
		{
			continue;
		}
		if (read_symbol == '(')
		{
			if (i != 0 && previous_symbol != '+'&&previous_symbol != '-'&&previous_symbol != '*'
				&&previous_symbol != '/'&&previous_symbol != '^'&&previous_symbol != '(')
				//это для проверки наличия знаков перед открывающейся скобкой
			{
				throw Exception(7, i - 1);//если знаков перед скобкой нет, сообщение о соответсвующей ошибке
			}
			if (next_symbol == '*' ||
				next_symbol == '/' || next_symbol == '^')
			{
				if (i == 0)	throw Exception(13, i);//выражение начинается со знака бинарной операции, - и + я не учитываю, потому что допускаю возможность записи числа начиная с +,-
				else throw Exception(10, i - 1);//между открытой скобкой и знаком бинарной операции отсутствует выражение
			}
		}
		else if (read_symbol == ')')
		{
			if (previous_symbol == '('&&i != length - 2)//если символ до этого открывающаяся скобка не учитывая пробелы,ошибка отсутствия выражения между скобками
				//при этом нужно учитывать.что если открывающаяся скобка в конце строки, то это уже другая ошибка, ведь последняя закрывающаяся
				//скобка пользователю не видна
			{
				if (i - g == 0)	throw Exception(14, i - 1);//выражение начинается с закрывающейся скобки
				else throw Exception(9, i - 2);//нет выражения между скобками, -2 чтобы учитывать позиция открывающейся скобки, а не закрывающейся
			}
			if (i < length - 2 && next_symbol != '+'&&next_symbol != '-'&&next_symbol != '*'
				&&next_symbol != '/'&&next_symbol != '^'&&next_symbol != ')')
				//для проверки наличия знаков после закрывающейся скобки
			{
				if (next_symbol == '(')	throw Exception(12, i - 1);//ошибка отсутствия знака бинарной операции между закрывающейся и открывающейся скобкой
				else throw Exception(8, i - 1);//между закрывающейся скобкой и числом отсутсвует знак бинарной операции
			}
			if (previous_symbol == '+' || previous_symbol == '-' || previous_symbol == '*' ||
				previous_symbol == '/' || previous_symbol == '^')
			{
				if (i == length - 2) throw Exception(15, i - 2);//выражение закансивается знаком бинарной операции
				else throw Exception(11, i - 1);//отсутствует выражение между знаком операции и скобкой закрывающейся
			}

		}
		else if (read_symbol != '*'&&read_symbol != '/'&&read_symbol != '^'&&read_symbol != '+'&&read_symbol != '-')
		{
			throw Exception(0, i - 1);//если незнакомый символ
		}
		else if (next_symbol == '*' || next_symbol == '/' || next_symbol == '^' || next_symbol == '+' || next_symbol == '-')//ошибка,идут подряд два знака, и это не открывающаяся скобка
		{
			throw Exception(1, i - 1);
		}
	}
}

void Calculator::PrintOperations()
{
	Russian("Введите арифметическое выражение. ");
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
		if (read_symbol == ' ')continue;//пропускаем пробелы
		int k = 0;
		do
		{
			k++;
		} while (string[i + k] == ' ');//для проверки знака следующего за read_symbol, не учитывая пробелы(это будет необходимо для проверок на ошибки)
		//так как всё выражение заключено в скобки изначально, то дополнительных условий нам не потребуется
		char next_symbol = string[i + k];
		if (read_symbol >= '0'&&read_symbol <= '9' || read_symbol == '.')//таким образом мы определяем, что это цифра, причём запись числа может начинаться с точки, что означает 0.
		{
			if (construct_num == false)//если состояние конструирования ложь, что может быть когда старое число уже добавлено в стек, а новое число ещё не начало строиться
			{
				if (read_symbol != '.')
				{
					number = read_symbol - '0';
					fractional = false;
				}
				else
				{
					fractional = true;//таким образом мы отмечаем,что число дробное
				}
				construct_num = true;//отмечаем,запустился процесс построления нового числа
				tenth = 1.0;
			}
			else
			{
				if (read_symbol == '.')
				{
					if (fractional == true)//для проверки наличия ещё одной точки в десятичной дроби
					{
						throw Exception(16, i - 1);//эту проверку нужно размещать в main так как после неё сразу идёт изменение fractional
					}
					fractional = true;
					if (next_symbol == '.')//проверка, не идут ли подряд две десятичные точки в числе
					{
						throw Exception(2, i - 1);
					}
				}
				else
				{
					number = number*10.0 + (read_symbol - '0');
				}
			}
			if (fractional && read_symbol != '.')//если число отмечено как дробное, его нужно поделить на число разрядов
			{
				tenth /= 10.0;
			}
		}
		else//если прочитанный символ не цифра и не точка
		{
			char sign_symbol;//взятый из стека символов знак операции
			if (construct_num)
			{
				if (fractional)
				{
					number *= tenth;//заканчиваем конструирование числа и добавляем его в стек
				}
				numbers.Push(number);
				number = 0;
				construct_num = false;//меняем состояние процесса конструирования числа
			}
			if (read_symbol == '(')
			{
				signs.Push(read_symbol);//открытую скобку мы кладём на любой символ
				//нам нужно пропустить все пробелы и заглянуть в предыдущий символ перед открытой скобки
				if (next_symbol == '-' || next_symbol == '+')
				{
					numbers.Push(0);//нужно превратить бинарный минус в 0-, бинарный плюс в 0+
				}
			}
			else if (read_symbol == ')')
			{
				bool state_of_operation;//для простоты вводим переменную отображающую успех или неуспех операции взятия из символьного стека знака операции
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

						else//данный вариант учитывает, когда в скобки заключено 1 число
						{
							numbers.Peek(result);
							//это нужно,чтобы учесть ситуацию, при которой выражение может быть 1 число,1 число заключённое в скобки
							//или выражение, которое включает в себя 1 число заключённое в скобки
						}
					}
					else
					{
						throw Exception(17, i - 2);//проверка на соотвествие закрывающихся и открывающихся скобок
						//в else зайдёт программа, если закрывающихся скобок будет больше, чем открывающихся
					}
				} while (sign_symbol != '(');//достаём из стека числа и знаки до тех пор, пока не дойдём до ближайшей открывающейся скобки

				if (i == length - 2 && !signs.IsEmpty())//если i на уровне последней закрывающейся скобки в конце выражения, а стек знаков операций ещё не пуст
				{
					throw Exception(17, i - 2);//от i отнимается 2,чтобы не учитывать первую скобку, выдавать позицию последнего символа в строке
				}
			}
			else if (read_symbol == '-' || read_symbol == '+')
			{
				do
				{
					if (signs.Peek(sign_symbol) && sign_symbol == '(')
					{
						signs.Push(read_symbol);//на открывающуюся скобку можно класть все знаки в стек
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