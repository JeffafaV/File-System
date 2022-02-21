main: main.o sdisk.o filesys.o shell.o
	c++ -o main main.o sdisk.o filesys.o shell.o

main.o: main.cpp
	c++ -c main.cpp
sdisk.o: sdisk.cpp
	c++ -c sdisk.cpp
filesys.o: filesys.cpp
	c++ -c filesys.cpp
shell.o: shell.cpp
	c++ -c shell.cpp
clean:
	rm *.o main disk1.txt


