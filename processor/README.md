1) Процессор со стековой памятью и регистрами, умеющий делать jump, в том числе условный.
2) Ассемблер переводит текст программы в структуру CompiledProgram, с которой умеет рабоать непосредственно процессор.
Так же у класса Assembler есть функция disassemble, которая переводит скомпилированную программу в её текст.
3) Примеры работающих программ на данном процессоре можно найти в programs (factorial и fibonacci)

Запуск:
1) mkdir tmp
2) cd tmp
3) cmake ..
4) make
5) mv processor ..
6) cd ..
7) ./processor ./programs/factorial #Сейчас выполнилась по умолчанию программа programs/factorial/source.txt - программа
принимает первым аргументом путь до папки с кодом программы в source.txt
