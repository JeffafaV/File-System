# File-System
C++ project that tries to mimic a real file system. This system consists of four parts, a simulated disk drive, a 
file system with dynamic file management, a shell that interacts with the file system and a database table system.
This was written and compiled in a UNIX environment. To run the program in Linux use the Makefile and main source file and 
type "make" in the command line. Then type "./main" in the command line. To run on Windows the Makefile is not necessary and 
can be compiled normally. From there you can do different things in the file system.
1. type "dir" to look at all the current files in the directory
2. type "add " followed by the name you want to give a new file to add a new file
3. type "del " followed by the name of the file you want to delete
4. type "type " followed by the name of the file to see the contents of the file
5. type "copy " followed by two file names where the first file's content will be copied to the second (put a space between the two names)
6. type "edit " followed by two names where the first name is a file's name you want to replace with the second name (put a space between 
the two names)
7. type "search " followed by a date in the data.txt file to find a certain data entry in the database (note this is limited to the table)
8. type "quit" to terminate the program

The table files create two new files in the sdisk that are used for fetching data in the database. The search feature allows the user to 
input a date in order to find the whole entry.