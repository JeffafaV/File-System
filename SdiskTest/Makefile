main: main.o sdisk.o
	g++ -o main main.o sdisk.o

main.o: main.cpp
	g++ -c main.cpp
sdisk.o: sdisk.cpp
	g++ -c sdisk.cpp

clean:
	rm *.o main test1
