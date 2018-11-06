cmake_minimum_required(VERSION 3.7)	 # Проверка версии CMake.
# Если версия установленой программы
# старее указаной, произайдёт аварийный выход.

project(square_equation)			# Название проекта

#include_directories("/home/ilya/programs/googletest/googletest/include")

set(SOURCE_EXE main.cpp square_equation.cpp) #test.cpp)		# Установка переменной со списком исходников для исполняемого файла

set(SOURCE_LIB square_equation.cpp)			# Тоже самое, но для библиотеки

add_library(foo STATIC ${SOURCE_LIB})	# Создание статической библиотеки с именем foo

add_executable(main ${SOURCE_EXE})	# Создает исполняемый файл с именем main

target_link_libraries(main foo)		# Линковка программы с библиотекой