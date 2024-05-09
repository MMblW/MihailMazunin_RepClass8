#pragma once
#include <iostream>
#include <string>

using namespace std;

const int evNothing = 0;
const int evMessage = 100;

const int cmCreate = 1;
const int cmAdd = 2;
const int cmDelete = 3;
const int cmPrint = 4;
const int cmPrintNames = 5;
const int cmQuit = 6;

class TEvent
{
public:
	int What;
	int Command;
};

class Object
{
public:
	virtual ~Object() {}
	Object() {}
	virtual void Show(bool OnlyNames = false) = 0;
	virtual void PutIn() = 0;
	virtual string GetName() = 0;
};

class Print : public Object
{
protected:
	string Author;
	string Name;
public:
	Print() {}
	Print(string N, string A)
	{
		SetName(N);
		SetAuthor(A);
	}
	virtual ~Print() {}
	string GetAuthor()
	{
		return Author;
	}
	string GetName()
	{
		return Name;
	}
	void SetAuthor(string A)
	{
		Author = A;
	}
	void SetName(string N)
	{
		Name = N;
	}
	void Show(bool OnlyNames = false) override
	{
		if (OnlyNames)
		{
			cout << GetName() << endl;
			return;
		}
		cout << "�����: " << Author << endl << "��������: " << Name << endl;
	}
	void PutIn() override
	{
		cout << "������� ������: " << endl;
		getline(cin, Author);
		cout << "������� ��������: " << endl;
		getline(cin, Name);
	}
	Print& operator = (Print& P)
	{
		Name = P.Name;
		Author = P.Author;
		return *this;
	}
};

class Magazine : public Print
{
protected:
	int Pages;
public:
	Magazine() : Print() {}
	Magazine(string N, string A, int P) : Print(N, A)
	{
		SetPages(P);
	}
	virtual ~Magazine() {}
	void SetPages(int P)
	{
		Pages = P;
	}
	int GetPages()
	{
		return Pages;
	}
	void Show(bool OnlyNames = false)
	{
		Print::Show(OnlyNames);

		if (!OnlyNames)
		{
			cout << "�������: " << Pages << endl;
		}
	}
	void PutIn() override
	{
		Print::PutIn();
		cout << "������� ���������� �������: " << endl;
		cin >> Pages;
	}
	Magazine& operator = (Magazine& M)
	{
		Print::operator = (M);
		Pages = M.Pages;
		return *this;
	}
};

class Vector
{
protected:
	Object** elements;
	int Length;
	int cur_Length;
public:
	Vector()
	{
		elements = 0;
		Length = 0;
		cur_Length = 0;
	}
	Vector(int L)
	{
		elements = new Object * [L];
		Length = L;
		cur_Length = 0;
	}
	virtual ~Vector()
	{
		if (elements == 0)
		{
			return;
		}
		for (int i = 0; i < cur_Length; i++)
		{
			delete elements[i];
		}
		delete[] elements;
		Length = 0;
		cur_Length = 0;
	}
	void Recreate(int L)
	{
		if (elements != 0)
		{
			for (int i = 0; i < cur_Length; i++)
			{
				delete elements[i];
			}
			delete[] elements;
		}
		Length = L;
		elements = new Object * [L];
		cur_Length = 0;
	}
	bool TryAdd(Object* E)
	{
		if (cur_Length >= Length)
		{
			cout << "��� �����!" << endl;
			return false;
		}
		elements[cur_Length] = E;
		cur_Length++;
		return true;
	}
	bool TryRemove(string N)
	{
		if (cur_Length == 0)
		{
			cout << "������ �����!" << endl;
			return false;
		}
		bool Flag = false;
		for (int i = 0; i < cur_Length && !Flag; i++)
		{
			Flag = elements[i]->GetName() == N;
			if (Flag)
			{
				Object* tmp = elements[i];
				for (int j = i; j < cur_Length; j++)
				{
					elements[j] = elements[j + 1];
				}
				delete tmp;
				cur_Length--;
			}
		}
		if (!Flag)
		{
			cout << "������ �������� ���!" << endl;
		}
		return Flag;
	}
	void PrintGroup(bool OnlyNames = false)
	{
		if (cur_Length == 0)
		{
			cout << "������ �����!" << endl;
			return;
		}
		elements[0]->Show(OnlyNames);
		for (int i = 1; i < cur_Length; i++)
		{
			cout << endl;
			elements[i]->Show(OnlyNames);
		}
	}
};

class Dialog : public Vector
{
protected:
	bool isOver;
public:
	Dialog()
	{
		isOver = false;
	}
	~Dialog() {};
	void Execute()
	{
		cout << "m - ������� ������, + - �������� �������, - - ������� �������, s - ������� ������ ���������, z - ������� ������ ���, q - ���������� ������" << endl;

		TEvent Event;

		while (!isOver)
		{
			GetEvent(Event);
			HandleEvent(Event);
		}
	}
	void GetEvent(TEvent& Event)
	{
		char CommandSymbol;
		cout << "> ";
		cin >> CommandSymbol;

		Event.What = evMessage;

		switch (CommandSymbol)
		{
		case 'm':
		{
			Event.Command = cmCreate;
			break;
		}
		case '+':
		{
			Event.Command = cmAdd;
			break;
		}
		case '-':
		{
			Event.Command = cmDelete;
			break;
		}
		case 's':
		{
			Event.Command = cmPrint;
			break;
		}
		case 'z':
		{
			Event.Command = cmPrintNames;
			break;
		}
		case 'q':
		{
			Event.Command = cmQuit;
			break;
		}
		default:
		{
			Event.What = evNothing;
		}
		}
	}
	void HandleEvent(TEvent& Event)
	{
		if (Event.What == evNothing)
		{
			cout << "�� ���� ����� ������!" << endl;
			return;
		}
		string Name;
		switch (Event.Command)
		{
		case cmCreate:
		{
			int Size;
			do
			{
				cout << "������� ������ ������: " << endl;
				cin >> Size;
			} while (Size <= 0);

			Recreate(Size);
			cout << "������. ������� ��������� �������" << endl;
			break;
		}
		case cmAdd:
		{
			if (elements == 0)
			{
				cout << "������ �� �������!" << endl;
				break;
			}

			int Number;

			do
			{
				cout << "1 - ������� �������� �������, 2 - ������� ����������� �����" << endl;
				cin >> Number;
			} while (Number != 1 && Number != 2);

			cin.ignore();

			if (Number == 1)
			{
				Print* Prt = new Print;
				Prt->PutIn();
				TryAdd(Prt);
			}
			else
			{
				Magazine* Mag = new Magazine;
				Mag->PutIn();
				TryAdd(Mag);
			}
			cout << "������. ������� ��������� �������" << endl;
			break;
		}
		case cmDelete:
		{
			if (elements == 0)
			{
				cout << "������ �� �������!" << endl;
				break;
			}
			cin.ignore();
			cout << "������� ��� ������: " << endl;
			getline(cin, Name);
			TryRemove(Name);
			cout << "������. ������� ��������� �������" << endl;
			break;
		}
		case cmPrint:
		{
			PrintGroup();
			cout << endl;
			cout << "������. ������� ��������� �������" << endl;
			break;
		}
		case cmPrintNames:
		{
			PrintGroup(true);
			cout << endl;
			cout << "������. ������� ��������� �������" << endl;
			break;
		}
		case cmQuit:
		{
			isOver = true;
			break;
		}
		default:
		{
			break;
		}
		}
		Event.What = evNothing;
	}
};