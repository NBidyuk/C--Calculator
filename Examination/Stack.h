#pragma once

template <typename T>
class Stack
{
	T *ptr;
	int top;
public:
	Stack();
	void Push(T symbol);// ���������� �������� � ����
	bool Pop(T&); //������������ �������� �� �����
	void Clear();//������� �����
	bool IsEmpty();
	int GetCount();//���������� ��������� � �����
	bool Peek(T&);//�������� �������� � ������� �����
	~Stack();
};


template <typename T>Stack<T>::Stack() :ptr(nullptr), top(-1)
{
}

template <typename T> void Stack<T>::Clear()
{
	delete[]ptr;
	ptr = nullptr;
}

template <typename T>bool Stack<T>::IsEmpty()
{
	return top == -1;
}

template <typename T>int Stack<T>::GetCount()
{
	return top + 1;
}

template <typename T>void Stack<T>::Push(T c)
{
	T* NewPtr = new T[top + 2];
	for (int i = 0; i < top+1; i++)
	{
		NewPtr[i] = ptr[i];
	}
	NewPtr[top + 1] = c;
	Clear();
	ptr = NewPtr;
	top++;
}

template <typename T>bool Stack<T>::Pop(T& symbol)
{
	if (!IsEmpty())
	{
		symbol = ptr[top--];
		return true;
	}
	else
		return false;
}

template <typename T>bool Stack<T>::Peek(T&symbol)
{
	if (!IsEmpty())
	{
		symbol = ptr[top];
		return true;
	}
	else
		return false;
}

template <typename T> Stack<T>::~Stack()
{
	Clear();
}





