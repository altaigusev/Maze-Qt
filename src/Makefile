.PHONY: all clean check tests gcov_report dvi install uninstall dist
XX=g++
APP=$(BD)Maze.app

FILES= cellular.cpp \
	   labyrinth.cpp \
	   row.cpp \

LDFLAGS= -Wall -Wextra -Werror -std=c++17 -g
OS := $(shell uname -s)
TEST_FLAGS= -lgtest -lgtest_main -lpthread
CFLAGS= -fprofile-arcs -ftest-coverage
BD= build/

ifeq ($(OS), Darwin)
	CC += -D OS_MAC
else
	CC += -D OS_LINUX
endif

all: build install tests

build: clean copy
	cd $(BD); qmake Maze.pro
	make -C $(BD)

copy:
	@mkdir build
	cp *.pro *.cpp *.png *.h *.ui $(BD)

install: uninstall build
	@mkdir $(HOME)/Applications/Maze/ 2>/dev/null
	cp -rf $(APP) $(HOME)/Applications/Maze/
	make clean

uninstall: 
	rm -rf $(HOME)/Applications/Maze

dist:
	rm -rf Maze
	mkdir Maze
	cp Makefile *.pro *.cpp *.user Maze/
	rsync -rupE $(DIRS)  Maze/
	tar -cvzf $(HOME)/Desktop/Maze.tgz Maze/
	rm -rf Maze/

dvi:
	open Doc/index.html

tests: clean
	$(XX) $(LDFLAGS) $(FLAGS) tests.cpp $(FILES) -lstdc++ $(TEST_FLAGS) -o test.out
	./test.out

clean:
	rm -rf build test *.a *.o *.cfg *.gcda *.gcno *.html *.info *.dSYM report

check: 
	cp ../materials/linters/CPPLINT.cfg CPPLINT.cfg
	cp ../materials/linters/suppressions.cfg suppressions.cfg
	python3 ../materials/linters/cpplint.py *.cpp *.h
	cppcheck -q --std=c++17 --enable=all --language=c++ --suppressions-list=suppressions.cfg *.cpp *.h

gcov_report: clean
	$(XX) $(FLAGS) $(CFLAGS) $(FILES) tests.cpp -lstdc++ $(TEST_FLAGS) -o test
	./test
	lcov --no-external -c -d . -o tests.info
	genhtml tests.info -o report
ifeq ($(OS), Darwin)
	open -a "Google Chrome" report/index.html
else
	xdg-open report/index.html
endif
