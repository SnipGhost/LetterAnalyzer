﻿//-----------------------------------------------------------------------------
// Project: Letter Analyzer
// File:    main.cpp
//                                                      Основной файл программы
//-----------------------------------------------------------------------------
#include "dictionary.h"
//-----------------------------------------------------------------------------
const char *inputMat()
{
	size_t n, m;
	cout << "Input N, M: ";
	cin >> n >> m;
	if (n == 0 || m == 0) // Обрабатываем вырожденность матрицы
	{
		showMsg(0, "Invalid N or M values!");
		return NULL;
	}
	cout << "Input matrix:\n";
	char *res = new char[n*m+1];       // Создаем представление матрицы
	for (size_t i = 0; i < n*m; i++)   // в виде вектора и вводим её
		cin >> res[i];
	res[n*m] = '\0';
	cout << "Please wait, analyze started\n";
	return res;
}
//-----------------------------------------------------------------------------
int main()
{
	if (!initLog("log.txt"))     // Поднимаем поток логов
		return 1;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	ofstream fout("out.txt");    // Открываем файл для объемного вывода
	if (fout.fail()) return 1;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	const char *s = inputMat();  // Вводим матрицу в s (вектор)
	if (s == NULL) return 1;     // Если при вводе произошли сбои
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Word word1("mother");        // Создаем слово из строки
	word1.print();               // Выводим по-умолчанию (на экран)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Word word2(s);               // Создаем слово из вектора символов
	word2.print(fout);           // Выводим в fout (в файл)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Dictionary *dic = new Dictionary("test.txt"); // Создаем словарь из файла
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// P.S. Можно и без указателя - Dictionary dic("test.txt");
	// Но тогда нельзя закрывать логи (они упадут при заверщении проги)
	// Это не критично, но лучше все же так не делать, а происходит
	// все это из-за того, что деструктор dic будет вызван уже после
	// завершения функции main - когда выйдет из области видимости.
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Заполняем словарь словами из 3-х файлов
	dic->readFile("google-10000-english-usa-no-swears-long.txt");
	dic->readFile("google-10000-english-usa-no-swears-medium.txt");
	dic->readFile("google-10000-english-usa-no-swears-short.txt");
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	dic->print(fout);            // Выводим словарь в файл
	dic->find(word1);            // Решить задачу для test1
	dic->find(word2);            // Решить задачу для test2
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	delete dic;                  // Очищаем память от словаря
	saveLog();                   // Закрываем лог
	return 0;
}
//-----------------------------------------------------------------------------