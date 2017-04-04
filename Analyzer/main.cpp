//-----------------------------------------------------------------------------
// Project: Letter Analyzer
// File:    main.cpp
//                                                      �������� ���� ���������
//-----------------------------------------------------------------------------
#include <iostream>
#include <string>
using namespace std;
//-----------------------------------------------------------------------------
#define FIRST_LOWER_SYM ('a')   // ������ ������ ��������    ( ������ �������)
#define LAST_LOWER_SYM  ('z')   // ��������� ������ �������� ( ������ �������)
#define FIRST_UPPER_SYM ('A')   // ������ ������ ��������    (������� �������)
#define LAST_UPPER_SYM  ('Z')   // ��������� ������ �������� (������� �������)
//-----------------------------------------------------------------------------
#define ERR_WORD ""                             // ����� ��-��������� ��� ����
#define ALPHA (LAST_LOWER_SYM-FIRST_LOWER_SYM)  // ���-�� �������� ��������
//-----------------------------------------------------------------------------
#define DEBUG_CRIT true   // �������� (��/���) ����������� ���������� ���������
#define DEBUG_WARN true   // �������� (��/���) ��������� �� ������������ �����
#define DEBUG_NORM true   // �������� (��/���) ��������� � �������� ���������
//-----------------------------------------------------------------------------
void showMsg(int type, string msg, ostream &out = cout)
{
	if (type == 0 && DEBUG_CRIT)
	{
		out << "[!] " << msg << endl;
		return;
	}
	if (type == 1 && DEBUG_WARN)
	{
		out << "[-] " << msg << endl;
		return;
	}
	if (type == 2 && DEBUG_NORM)
	{
		out << "[+] " << msg << endl;
		return;
	}
	out << msg << endl;
}
//-----------------------------------------------------------------------------
class Word
{
 private:
	 string str;           // ����������� ��������� �����
	 size_t count[ALPHA];  // ���-�� ���������������� ���� ��������
 public:
	 Word(const string s); // �����������, ��������� ������ �������
	 size_t size();        // ���������� ����� �����
	 size_t *cmp();        // ���������� ��������� �� count (��� ���������)
	 string gets();        // ���������� ����� � ����������� ���������
	 //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 void print(ostream &out = cout); // ����������� ������ ���� � ����� out
};
//-----------------------------------------------------------------------------
Word::Word(const string s): str(s)
{
	for (size_t i = 0; i < ALPHA; ++i) count[i] = 0;
	for (size_t i = 0; i < str.length(); ++i)
	{
		if (str[i] >= FIRST_LOWER_SYM && str[i] <= LAST_LOWER_SYM)
		{
			++count[str[i]-FIRST_LOWER_SYM];
		} 
		else if (str[i] >= FIRST_UPPER_SYM && str[i] <= LAST_UPPER_SYM)
		{
			++count[str[i]-FIRST_UPPER_SYM];
		}
		else 
		{
			str = ERR_WORD;
			showMsg(1, "Word " + s + " include invalid characters");
			return;
		}
	}
	showMsg(2, "Word " + str + " was read successfully");
}
//-----------------------------------------------------------------------------
void Word::print(ostream &out)
{
	if (str == ERR_WORD)
	{
		showMsg(1, "Attempt to print an ERR_WORD");
		return;
	}
	for (size_t i = 0; i < ALPHA; ++i)
		if (count[i] > 0)
			out << (char)(i+FIRST_LOWER_SYM) << ": " << count[i] << endl;
	showMsg(2, "Word " + str + " vector successfully printed");
}
//-----------------------------------------------------------------------------
int main()
{
	Word test("Hello");
	test.print();
	return 0;
}
//-----------------------------------------------------------------------------