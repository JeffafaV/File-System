# File-System
C++ project for my advanced operating systems course. This system consists of four parts, a simulated disk drive, a 
file system with dynamic file management, a shell that utilizes the file system and a database system with a table.
This was written and compiled in a UNIX environment. To run the program type "make" in the command line. Then type 
"./main" in the command line. From there you can do different things in the file system.
1. type "dir" to look at all the current files in the directory
2. type "add " followed by the name you want to give a new file to add a new file
3. type "del " followed by the name of the file you want to delete
4. type "type " followed by the name of the file to see the contents of the file
5. type "copy " followed by two file names where the first file's content will be copied to the second
6. type "quit" to terminate the program
7. type "search " followed by a date in the data.txt file to find a certain data entry in the database (note this is limited to the table)

The table creates two new files in the sdisk that are used for fetching data in the database. The search feature allows the user to 
input a date in order to find the whole entry.