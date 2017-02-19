# Letter Analyzer #


## Оглавление ##
1. Постановка задачи
2. Выбор методов и технологий
3. Исследование данных
4. Предварительная обработка данных
5. Разработка алгоритма и спецификации
6. Анализ результатов


## 1. Постановка задачи ##

Вводятся натуральные числа N и M.  
Затем вводится матрица символов (char) английского алфавита размером N*M.  

На основе данной матрицы необходимо сформировать и вывести все существующие слова, 
которые можно составить из данных букв, начиная с самых длинных и заканчивая самыми короткими.

Пример:
```
Input:
3 2
h e t
l o l

Output:
Букв: 5
hello helot hotel thole 
Букв: 4
hell hole holt lote tell toll 
Букв: 3
hel ell elt het hoe hot leo let lot the tho toe lol
Букв: 2
oh ho
```


## 2. Выбор методов и технологий ##

Разобьем данное задание на основные подзадачи.
Нам необходимо обеспечить:  
1) Ввод и анализ матрицы букв  
2) Формирование и вывод слов  

Первая подзадача может решаться различными способами, в зависимости от требований алгоритма второй. Но в любом случае это будет довольно тривиальное решение. Останавливаться на нем не будем.

Для второй же подзадачи рассмотрим три основных подхода:  

1) Использование уже готовых сторонних интернет-сервисов или библиотек для составления существующих в данном языке слов.  
Плюсы:  
Быстро реализуемо и в теории должно показать относительно неплохую эффективность работы.  
Минусы:  
Низкая устойчивость решения из-за внешних зависимостей и отсутсвие должного уровня гибкости.  

2) Использование локальной базы данных существующих слов и осуществление поиска по ней.  
Плюсы:  
Средняя сложность, эффективность и устойчивость решения зависит только от внутренних факторов.  
Минусы:  
Потребуется система хранения и поиска данных - уже существующая или же специальная.  

3) Машинное обучение, нестандартные алгоритмы и искуственные нейронные сети, обученные формировать языковые конструкции на основе уже известных.  
Плюсы:  
Нестандартный подход, не требует поиска по большому объему данных.  
Минусы:  
Высокая сложность, требует обучения и низкая точность, зависящая от входного набора данных.  

Первый подход, вероятно, достаточно хорош, но нестабилен и безынтересен. А третий требует слишком обширных исследований и будет давать часть ложных ответов.  
Остановимся на втором подходе. Причем не будем забивать гвозди микроскопом - составим свою локальную базу данных, не приходя к помощи громоздких СУБД.

## 3. Исследование данных ##

Итак, нам потребуется база данных английских слов, строго упорядоченная для нашего алгоритма.

По мнению составителей словарей Webster (Third International Dictionary) и Oxford English Dictionary (Second Edition, 1993), английский язык насчитывает около 470 тысяч слов. [(Источник)][how-many-words-1]  
А если учитывать недавно вошедшие в употребление слова, используемые в интернете, то по подсчётам компании Global Language Monitor (GLM) на 2015 год в английском языке более 1,03 млн слов. [(Источник)][how-many-words-2]  

Однако даже это не предел. По данным того же GLM - новое слово в английском языке появляется каждые 98 минут. И это с учетом того что "новым" слово становится после того, как оно было использовано в литературе, СМИ, интернете не менее 25000 раз.

Разумеется, поиск по такой базе данных занимал бы слишком большое кол-во времени, да и занимала она бы слишком большой объем памяти. Поэтому придется ограничить её объемы и пренебречь частью слов.
Но тогда перед нами встает вопрос - сколько слов мы должны хранить, чтобы обеспечить достаточно малый уровень неточности?

Что ж, это вопрос для строгой математической статистики. К счастью, для английского языка (как и для любого естественного языка) выполняется [закон Ципфа][zipf's-law]. Если кратко, то это означает, что если упорядочить весь словарь по частоте употребления слов - каждое последующее слово встречается примерно в N раз реже, где N - его номер. Т.е. второе встречается в 2 раза реже, третье в 3, и т.д. Это продемонстрированно на картинке "ранг - частота".

![words][img-words]

Исследуем самое используемое слово английского языка - "the". Для текста лицензии GNU AGPL 3.0 частота слова "the" составляет `341 / 5495 ~= 0.062`, а для случайно выбранной [статьи][news] с новостного сайта `23 / 296 ~= 0.078`. На больших объемах данных, по данным с сайта [wordfrequency.info][wordfr] для всех слов с википедии это значение будет `120277503 / 1900000000 ~= 0.063`.

Таким образом, эта величина довольно сильно отличается для разных текстов, так что возьмем среднее значение по выборкам: `h = 0.065` частота появления слова на единицу текста. На основании этих данных и закона Ципфа можем составить ряд формул.  

Кол-во известных слов из текста объемом `s` слов, если мы знаем только `n` самых частых слов:  
`k = (h + h/2 + h/3 + ... + h/n) * s = (1 + 1/2 + 1/3 + ... + 1/n) * h * s`  
Найдем показатель известных нам слов в тексте в процентах:  
`p = k * 100 / s =  (1 + 1/2 + 1/3 + ... + 1/n) * h * s * 100 / s = (1 + 1/2 + 1/3 + ... + 1/n) * h * 100`  

Численно решим данное выражение для `h = 0.065` с помощью простой программки на C++:  
```cpp
  #include <iostream>
  using namespace std;
  int main()
  {
    const double h = 0.065;
    const double x[] = {100, 200, 500, 1000, 5000, 10000, 15000, 20000, 30000};
    for (double n : x)
    {
        double r = 0;
        for (double i = 1; i <= n; ++i) r += 1 / i;
        cout << n << ": \t" << r * h * 100 << "%\n";
    }
  }
```
Результат:
```
100:    33.7180%
200:    38.2072%
500:    44.1534%
1000:   48.6556%
5000:   59.1143%
10000:  63.6194%
15000:  66.2549%
20000:  68.1247%
30000:  70.7602%
```
На практике все оказывается еще лучше, чем при рассчетах. Закон Ципфа оказывается недостаточно точным для первых элементов списка. Показатель известных слов для БД на 10 тысяч слов - порядка 80%. 
А исследование сайта [native-english][native-research] показывают что 3000-4000 на самом деле покрывают 95% всего активного словаря.

![Books][img-books]

Также стоит учесть, что активный словарный запас человека примерно 15-25 тысяч слов.  
Таким образом, приемлимым будет размер базы данных в 15-20 тысяч, при этом показатель известных слов будет на уровне 80-85%.  

Допустим, что у нас есть база данных слов. Разумно будет сразу упорядочить все слова по убыванию их длины.

[how-many-words-1]:<https://www.merriam-webster.com/help/faq-how-many-english-words>
[how-many-words-2]:<http://www.languagemonitor.com/trending-words/1000000th-english-word-announced>
[zipf's-law]:<https://ru.wikipedia.org/wiki/%D0%97%D0%B0%D0%BA%D0%BE%D0%BD_%D0%A6%D0%B8%D0%BF%D1%84%D0%B0>
[img-words]:<https://pp.vk.me/c638430/v638430039/27545/KDFoTXV3mvg.jpg>
[img-books]:<https://pp.vk.me/c638430/v638430039/2774c/HVIUdysd3Y0.jpg>
[news]:<https://www.cnet.com/news/neil-degrasse-tyson-snowden-science-rsa/>
[wordfr]:<http://corpus.byu.edu/>
[native-research]:<https://www.native-english.ru/articles/words>
[words-data]:<https://github.com/first20hours/google-10000-english>
