//-----------------------------------------------------------------------------
// Project: Letter Analyzer
// File:    dictionary.cpp
//                                      Файл реализации классов слова и словаря
//-----------------------------------------------------------------------------
#include "dictionary.h"
//-----------------------------------------------------------------------------
ostream *logs;          // Глобальный поток логов
bool isFileLog = false; // Логи открыты как файл (да/нет)
//-----------------------------------------------------------------------------
bool initLog(string fileName)
{
	if (fileName == "cout") // Исключительный случай - установить как iostream
	{
		logs = &cout;
		return 1;
	}
	logs = new ofstream(fileName); // Открыть лог как файл-поток
	isFileLog = true;
	return !logs->fail();          // Возвращаем состояние
}
//-----------------------------------------------------------------------------
void saveLog()
{
	if (isFileLog)
	{
		((ofstream*)logs)->close();
		delete logs;
	}
}
//-----------------------------------------------------------------------------
void showMsg(int type, string msg, ostream &out)
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
	if (type == 3 && DEBUG_INFO)
	{
		out << "[i] " << msg << endl;
		return;
	}
	#ifdef DEBUG
		out << "[ ] " << msg << endl;
	#endif
}
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
			showMsg(1, "Word '" + s + "' include invalid characters");
			return;
		}
	}
	showMsg(3, "Word '" + str + "' was created successfully");
}
//-----------------------------------------------------------------------------
size_t Word::size()
{
	return str.length();
}
//-----------------------------------------------------------------------------
size_t *Word::cmp()
{
	return count;
}
//-----------------------------------------------------------------------------
string Word::gets()
{
	return str;
}
//-----------------------------------------------------------------------------
void Word::print(ostream &out)
{
	if (str == ERR_WORD)
	{
		showMsg(1, "Attempt to print an invalid word");
		return;
	}
	for (size_t i = 0; i < ALPHA; ++i)
		if (count[i] > 0)
			out << (char)(i+FIRST_UPPER_SYM) << ": " << count[i] << endl;
	showMsg(2, "Word '" + str + "' vector successfully printed");
}
//-----------------------------------------------------------------------------
Dictionary::Dictionary(string fileName)
{
	if (readFile(fileName))
	{
		showMsg(2, "Successfully created dictionary from file: " + fileName);
	}
	else
	{
		showMsg(1, "Error reading file: " + fileName);
		clear();
		showMsg(1, "Created empty dictionary");
	}
}
//-----------------------------------------------------------------------------
Dictionary::~Dictionary()
{
	clear();
}
//-----------------------------------------------------------------------------
void Dictionary::clear()
{
	showMsg(3, "Clearing dictionary ...");
	for (size_t i = 0; i < words.size(); ++i)
	{
		showMsg(4, "Word '" + words[i]->gets() + "' is deleting");
		if (words[i] != NULL) delete words[i];
	}
	words.clear();
	showMsg(2, "Dictionary cleared");
}
//-----------------------------------------------------------------------------
size_t Dictionary::size()
{
	return words.size();
}
//-----------------------------------------------------------------------------
void Dictionary::addWord(string word)
{
	Word *w = new Word(word);      // Выделяем память под новое слово
	words.push_back(w);            // Временно добавляем в конец
	size_t idx = 0;                // Положение нового слова в векторе слов
	if (pos.count(w->size()) == 0) // Если блока с такой длиной слова еще нет
	{                              // То начинаем создавать новый блок
		size_t c = 0;
		auto it = pos.rbegin();
		for (it = pos.rbegin(); it != pos.rend(); ++it) // Ищем положение
		{
			++c;
			showMsg(4, to_string(w->size()) + ">" + to_string(it->first));
			if (w->size() > it->first)
			{
				showMsg(4, "Yes it->second=" + to_string(it->second));
				idx = it->second;
				break;                              // Положение найдено
			}
		}
		if (it == pos.rend()) idx = words.size()-1; // Не нашли положения
	}
	else
	{
		idx = pos[w->size()]; // Иначе - записываем слово в свой блок
	}
	showMsg(4, "idx = " + to_string(idx));
	for (size_t i = words.size()-1; i > idx; --i)        // Смещаем слова
		words[i] = words[i-1];
	for (auto it = pos.rbegin(); it != pos.rend(); ++it) // Смещаем блоки
	{
		if (it->second >= idx && w->size() != it->first) 
			it->second++;
	}
	words[idx] = w;       // Финальное обновление положения слова
	pos[w->size()] = idx; // Обновление позиции блока
}
//-----------------------------------------------------------------------------
bool Dictionary::readFile(string fileName)
{
	ifstream fin(fileName);
	if (fin.fail())
	{
		showMsg(1, "Couldn't open file: " + fileName);
		return 0;
	}
	string line;
	while (getline(fin, line))           // Организуем построчное чтение
	{
		if (line.length() < 1) continue; // Пропускаем пустые строки файла
		addWord(line);                   // Добавляем новое слово
	}
	fin.close();
	showMsg(2, "Dictionary successfully read");
	return 1;
}
//-----------------------------------------------------------------------------
void Dictionary::find(Word &w, ostream &out)
{
	showMsg(3, "Searching for '" + w.gets() + "' started");
	size_t idx = 0;
	if (pos.count(w.size()) != 0) // Если блок данной длины сущестует
		idx = pos[w.size()];      // То начинаем просматривать с него
	else 
	{
		for (auto it = pos.rbegin(); it != pos.rend(); ++it) // Ищем блок
		{
			if (w.size() > it->first) // Как только найдем меньшую длину
			{
				idx = it->second;     // Записываем индекс
				break;
			}
		}
	}
	for (size_t i = idx; i < words.size(); ++i)    // Просматриваем слова
	{
		size_t *b = w.cmp(), *a = words[i]->cmp(); // Сравниваем a и b
		bool allowed = true;
		for (size_t j = 0; (j < ALPHA)&&(allowed); ++j) // Если не совпали
			if (a[j] > b[j]) allowed = false;           // Хотя бы в одной
		if (allowed)                                    // То не выводим слово
			out << words[i]->gets() << endl;
	}
	showMsg(2, "Search for '" + w.gets() + "' complete");
}
//-----------------------------------------------------------------------------
void Dictionary::print(ostream &out)
{
	for (auto it = pos.begin(); it != pos.end(); ++it) // Разметка блоков
	{
		out << "len: " << setw(2) << it->first;
		out << " pos: " << it->second << endl;
	}
	for (size_t i = 0; i < words.size(); ++i)          // Сами слова
	{
		out << setw(5) << i << " ";
		out << words[i]->gets() << endl;
	}
	showMsg(2, "Dictionary printed");
}
//-----------------------------------------------------------------------------