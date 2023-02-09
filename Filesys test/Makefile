main: main.o sdisk.o filesys.o
	c++ -o main main.o sdisk.o filesys.o

main.o: main.cpp
	c++ -c main.cpp
sdisk.o: sdisk.cpp
	c++ -c sdisk.cpp
filesys.o: filesys.cpp
	c++ -c filesys.cpp

clean:
	rm *.o main disk1.txt



