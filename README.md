# File-System
C++ project that tries to mimic a real file system. This system consists of four parts, a simulated disk drive, a file system with dynamic file management, a shell that interacts with the file system and a database table system that is inserted into the file system. This program can be compiled using GCC or MSVC. To run the code on Linux follow these steps.

1. Install the GCC g++ compiler
2. Install Make (optional)
3. Go into project directory and insert any main program and Makefile if you're using Make, from the test folders
4. If you're using Make then in the command line type "make" and then "./main" to run the program. If you're not using Make then compile each file as normal to run

To run the code on Windows follow these steps.

1. Install Visual Studio
2. Open project in Visual Studio and insert any main program from the test folders
3. Press Run to run the program

If you ran the Shell or Table main programs, you can do many different things in the file system.

1. type "dir" to look at all the current files in the directory
2. type "add " followed by the name you want to give a new file to add a new file
3. type "del " followed by the name of the file you want to delete
4. type "type " followed by the name of the file to see the contents of the file
5. type "copy " followed by two file names where the first file's content will be copied to the second (put a space between the two names)
6. type "edit " followed by two names where the first name is a file's name you want to replace with the second name (put a space between 
the two names)
7. type "search " followed by a year in the data.txt file to find a certain data entry in the database (note this is limited to the table)
8. type "quit" to terminate the program

The table files create two new files in the sdisk that are used for fetching data in the database which decreases the space in the file system. It is recommended to use Shell to play around with adding and deleting files and Table to play around with inserting and viewing table information.

## Future Features and Optimizations
I plan to implement more meta data to a file in the FAT such as when the file was created or how big is the file. I want to also update the the Sdisk. It currently has all blocks in one line in a text file which makes it difficult to understand how many blocks there are or how much space is used in a block. I want to make it such that each block takes up one line in a text file to increase readability.