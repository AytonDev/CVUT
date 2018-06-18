PROGRAM=frajtluk
CC=g++
CFLAGS=-std=c++11 -Wall -pedantic -Wextra -g -Wno-long-long -O0 -ggdb


all: compile doc

clean:
	rm -rf $(PROGRAM) objs/ doc/ 2>/dev/null	

doc:
	mkdir doc;cd src;doxygen Doxyfile
	
compile: $(PROGRAM)	

run: compile	
	./$(PROGRAM)

$(PROGRAM): objs/main.o objs/appManager.o objs/baseGame.o objs/classicGame.o objs/fileManager.o objs/fileManagerCSV.o objs/fileManagerTXT.o objs/baseQuestion.o objs/multiTextQuestion.o objs/orderQuestion.o objs/pickManyQuestion.o objs/pickOneQuestion.o objs/textQuestion.o objs/trueFalseQuestion.o objs/player.o objs/quizManager.o
	$(CC) $(CFLAGS) $^ -o  $@

objs/main.o: src/main.cpp src/appManager.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/appManager.o: src/appManager.cpp src/appManager.h src/quizManager.h src/fileManagerCSV.h src/fileManagerTXT.h src/textQuestion.h src/multiTextQuestion.h src/orderQuestion.h src/pickManyQuestion.h src/pickOneQuestion.h src/trueFalseQuestion.h src/classicGame.h  | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/baseGame.o : src/baseGame.cpp src/player.h src/baseQuestion.h src/quizManager.h src/fileManagerTXT.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/classicGame.o: src/classicGame.cpp src/baseGame.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/fileManager.o : src/fileManager.cpp src/fileManager.h src/quizManager.h src/player.h src/textQuestion.h src/multiTextQuestion.h src/orderQuestion.h src/pickManyQuestion.h src/pickOneQuestion.h src/trueFalseQuestion.h src/classicGame.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/fileManagerCSV.o : src/fileManagerCSV.cpp src/fileManager.h src/quizManager.h src/baseQuestion.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/fileManagerTXT.o : src/fileManagerTXT.cpp src/fileManager.h src/quizManager.h src/baseQuestion.h src/player.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/baseQuestion.o : src/baseQuestion.cpp src/baseQuestion.h  src/quizManager.h  objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/multiTextQuestion.o : src/multiTextQuestion.cpp src/baseQuestion.h  src/quizManager.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/orderQuestion.o : src/orderQuestion.cpp src/baseQuestion.h src/quizManager.h  | objs
	$(CC) $(CFLAGS) -c $< -o $@	

objs/pickManyQuestion.o : src/pickManyQuestion.cpp src/baseQuestion.h src/quizManager.h | objs
	$(CC) $(CFLAGS) -c $< -o $@	

objs/pickOneQuestion.o : src/pickOneQuestion.cpp src/baseQuestion.h src/quizManager.h | objs
	$(CC) $(CFLAGS) -c $< -o $@	

objs/textQuestion.o : src/textQuestion.cpp src/baseQuestion.h src/quizManager.h | objs
	$(CC) $(CFLAGS) -c $< -o $@	

objs/trueFalseQuestion.o : src/trueFalseQuestion.cpp src/baseQuestion.h src/quizManager.h | objs
	$(CC) $(CFLAGS) -c $< -o $@	

objs/player.o : src/player.cpp | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/constants.o : src/constants.cpp | objs
	$(CC) $(CFLAGS) -c $< -o $@



objs/quizManager.o : src/quizManager.cpp src/baseQuestion.h | objs
	$(CC) $(CFLAGS) -c $< -o $@	

objs: 
	mkdir objs
