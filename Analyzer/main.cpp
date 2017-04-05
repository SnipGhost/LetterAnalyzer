//-----------------------------------------------------------------------------
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
	if (n == 0 || m == 0)
	{
		showMsg(0, "Invalid N or M values!");
		return NULL;
	}
	cout << "Input matrix:\n";
	char *res = new char[n*m];
	for (size_t i = 0; i < n*m; i++)
	{
		cin >> res[i];
	}
	cout << "Please wait, analyze started\n";
	return res;
}
//-----------------------------------------------------------------------------
int main()
{
	if (!initLog("log.txt"))   // Поднимаем поток логов
		return 1;

	ofstream fout("out.txt");  // Открываем поток для записи объемного вывода
	if (fout.fail()) return 1;

	Word test1("characterized"); // [ПРИМЕР] Создаем слово из строки
	test1.print();               // [ПРИМЕР] Выводим по-умолчанию (на экран)

	const char *s = inputMat();  // [ПРИМЕР] Вводим матрицу в s
	if (s == NULL) return 1;     // [ПРИМЕР] Если при вводе произошли сбои

	Word test2(s);               // [ПРИМЕР] Создаем слово из вектора символов
	test2.print();               // [ПРИМЕР] Выводим по-умолчанию (на экран)

	Dictionary dic("test.txt");  // [ПРИМЕР] Создаем словарь
	dic.print(fout);             // [ПРИМЕР] Выводим в файл

	dic.find(test1);             // [ПРИМЕР] Решить задачу для test1

	dic.find(test2);             // [ПРИМЕР] Решить задачу для test2

	return 0;
}
//-----------------------------------------------------------------------------