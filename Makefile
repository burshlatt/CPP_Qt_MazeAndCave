CC = g++
FLAGS = -Wall -Werror -Wextra -std=c++17
TEST_FLAGS = -lgtest -lgmock -pthread

HEADER_FILES = controller/*.h model/*.h view/*.h
CC_FILES = view/*.cc main.cc
TEST_FILES = tests/*.cc

.PHONY: all
all: clang tests install


#	== УСТАНОВКА ПРОГРАММЫ ==
.PHONY: install
install:
	@echo -------------------- INSTALL --------------------
	make uninstall --quiet
	mkdir build
	cd build && cmake ../CMakeLists.txt && make && rm -rf CMakeFiles cmake_install.cmake CMakeCache.txt Makefile
	@echo -------------------- SUCCESS --------------------


#	== УСТАНОВКА ПРОГРАММЫ ==
.PHONY: open
open:
	@echo --------------------- OPEN ----------------------
	cd build && open Maze.app
	@echo -------------------- SUCCESS --------------------


#   === СТИЛЬ ===
.PHONY: clang
clang:
	@echo --------------------- CLANG ---------------------
	cp ../materials/linters/.clang-format .
	clang-format -n $(HEADER_FILES) $(CC_FILES) $(TEST_FILES)
	rm -rf .clang-format
	@echo -------------------- SUCCESS --------------------


#	== УДАЛЕНИЕ ПРОГРАММЫ ==
.PHONY: uninstall
uninstall:
	@echo ------------------- UNINSTALL -------------------
	rm -rf build
	@echo -------------------- SUCCESS --------------------


#	== ЗАПУСК UNIT-ТЕСТОВ ==
.PHONY: tests
tests:
	@echo --------------------- TESTS ---------------------
	cd ./tests/ && $(CC) $(FLAGS) unit_tests.cc -o unit_tests $(TEST_FLAGS)
	cd ./tests/ && ./unit_tests
	@echo -------------------- SUCCESS --------------------


#	=== ПРОВЕРКА НА УТЕЧКИ ПАМЯТИ ===
leaks: leaks
	@echo --------------------- LEAKS ---------------------
	cd ./tests/ && leaks -atExit -- ./unit_tests
	@echo ---------------------- END ----------------------


#	== ОЧИСТКА ФАЙЛОВ ==
.PHONY: clean
clean:
	@echo --------------------- CLEAN ---------------------
	rm -rf tests/unit_tests tests/maze.txt Maze.tar.gz


#	== СОЗДАНИЕ АРХИВА ПРОЕКТА ==
.PHONY: dist
dist: clean uninstall
	@echo --------------------- DIST ----------------------
	tar czvf Maze.tar.gz .
	@echo -------------------- SUCCESS --------------------


#	== DVI ==
.PHONY: dvi
dvi:
	@echo --------------------- DVI -----------------------
	open index.html
	@echo -------------------- SUCCESS --------------------