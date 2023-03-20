main: main.o sdisk.o filesys.o shell.o
	g++ -o main main.o sdisk.o filesys.o shell.o

main.o: main.cpp
	g++ -c main.cpp
sdisk.o: sdisk.cpp
	g++ -c sdisk.cpp
filesys.o: filesys.cpp
	g++ -c filesys.cpp
shell.o: shell.cpp
	g++ -c shell.cpp
clean:
	rm *.o main disk1.txt
