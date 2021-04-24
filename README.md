# cpp-database
**Final project for cpp yellow course on Coursera**

https://www.coursera.org/learn/c-plus-plus-yellow

Улучшенная версия базы данных https://github.com/luboshnikov/cpp-white-project. База хранит даты и события. 

Умеет обрабатывать запросы:

`Add` date event — добавить пару (Дата, Событие)

`Print` — вывести всё содержимое базы данных;

`Find` condition — вывести все записи, содержащиеся в базе данных, которые удовлетворяют условию condition;

`Del` condition — удалить из базы все записи, которые удовлетворяют условию condition;

`Last` date — вывести запись с последним событием, случившимся не позже данной даты.

Код состоит из файлов:

`main.cpp` - функция main

`condition_parser.h/cpp` - обработка событий для Find и Del

`token.h/cpp` - токенизатор обрабатывающий запросы событий

`date.h/cpp` - объявления и определения класса Date, функции ParseDate работающие с датами

`database.h/cpp` - объявление и определение класса Database, хранящего даты и события

`node.h` - объявления и описание класса представляющего собой узлы абстрактного синтаксического дерева
