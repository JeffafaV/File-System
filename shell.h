#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <string>
#include "filesys.h"

using namespace std;

// creation of shell which inherits the filesys
// the shell is the interface between the user and file system
// allows the user to add, delete, and copy files
// and can also edit a file's name
// the user can view the content of a file
// and can view all the files in the file system
class Shell : public Filesys 
{
public:
    Shell(string filename, int blocksize, int numberofblocks); // inherits from filesys
    int dir(); // lists all files in the file system
    int add(string file); // adds a new file and its content in the file system
    int del(string file); // deletes a file from the file system
	int edit(string file1, string newName); // change the name of file file1 to newName
    int type(string file); // lists the contents of a file
    int copy(string file1, string file2); // copies the content of file1 and stores it in file2
};

#endif
