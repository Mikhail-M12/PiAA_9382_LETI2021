
all: main

main: SquarePaver.o SquarePavingMap.o
	g++ -o main SquarePaver.o SquarePavingMap.o

SquarePaver.o:
	g++ -c ./src/SquarePaver/SquarePaver.cpp

SquarePavingMap.o:
	g++ -c ./src/SquarePaver/SquarePavingMap/SquarePavingMap.cpp

clean.o:
	rm *.o
clean: clean.o
	rm main