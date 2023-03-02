main: main.o sdisk.o
	c++ -o main main.o sdisk.o

main.o: main.cpp
	c++ -c main.cpp
sdisk.o: sdisk.cpp
	c++ -c sdisk.cpp

clean:
	rm *.o main test1

