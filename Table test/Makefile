main: main.o sdisk.o filesys.o shell.o table.o
	c++ -o main main.o sdisk.o filesys.o shell.o table.o

main.o: main.cpp
	c++ -c main.cpp
sdisk.o: sdisk.cpp
	c++ -c sdisk.cpp
filesys.o: filesys.cpp
	c++ -c filesys.cpp
shell.o: shell.cpp
	c++ -c shell.cpp
table.o: table.cpp
	c++ -c table.cpp
clean:
	rm *.o main sdisk.txt


