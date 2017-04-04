//-----------------------------------------------------------------------------
// Project: Letter Analyzer
// File:    main.cpp
//                                                      Основной файл программы
//-----------------------------------------------------------------------------
#include <iostream>
#include <string>
using namespace std;
//-----------------------------------------------------------------------------
#define FIRST_LOWER_SYM ('a')   // Первый символ алфавита    ( нижний регистр)
#define LAST_LOWER_SYM  ('z')   // Последний символ алфавита ( нижний регистр)
#define FIRST_UPPER_SYM ('A')   // Первый символ алфавита    (верхний регистр)
#define LAST_UPPER_SYM  ('Z')   // Последний символ алфавита (верхний регистр)
//-----------------------------------------------------------------------------
#define ERR_WORD ""                             // Слово по-умолчанию при сбое
#define ALPHA (LAST_LOWER_SYM-FIRST_LOWER_SYM)  // Кол-во символов алфавита
//-----------------------------------------------------------------------------
#define DEBUG_CRIT true   // Выводить (да/нет) критические отладочные сообщения
#define DEBUG_WARN true   // Выводить (да/нет) сообщения об обработанных сбоях
#define DEBUG_NORM true   // Выводить (да/нет) сообщения о успешных операциях
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
	 string str;           // Изначальное состояние слова
	 size_t count[ALPHA];  // Кол-во здаействованнных букв алфавита
 public:
	 Word(const string s); // Конструктор, заполняет объект данными
	 size_t size();        // Возвращает длину слова
	 size_t *cmp();        // Возвращает указатель на count (для сравнения)
	 string gets();        // Возвращает слово в изначальном состоянии
	 //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 void print(ostream &out = cout); // Распечатать вектор букв в поток out
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