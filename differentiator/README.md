Дифференциатор функции

Использование:
1) Клонируете репозиторий
2) Запускаете run.sh input.txt # Где input.txt - пусть до файла с выражением, которое хотите продифференцировать
3) В выводе видите продифференцированное выражение

Пример: Результаты работы на input.txt смотри в output.txt

Структура:
1) Parser переводит входную строку во внутренее представление Operation (абстрактный класс)
2) Оно уже имеет метод derivative, возвращающий производную по переданной переменной данного выражения
3) Также Operation и имеет метод to_string() переводящий выражение обратно в строку.

Будущее развитие:
1) Увеличение числа принимающихся функций. Сейчас можно в input использовать пробелы, скобки, различные переменные,
некоторые бинарные и унарные функции. Можно добавить больше унарных функций (sqrt, exp, sh, ...)
2) Улучшать вид получаемого выражания, уменьшать кол-во ненужных скобок, убирать нулевые слагаемые (хотя частично это
уже делается), и сокращать дроби.