main: main.o sdisk.o filesys.o
	g++ -o main main.o sdisk.o filesys.o

main.o: main.cpp
	g++ -c main.cpp
sdisk.o: sdisk.cpp
	g++ -c sdisk.cpp
filesys.o: filesys.cpp
	g++ -c filesys.cpp

clean:
	rm *.o main disk1.txt
