#ifndef table_h
#define table_h

#include "filesys.h"
#include "shell.h"
#include <stdio.h>
#include <string>

// the table class utilizes the file system (it is not part of the system)
// it is specific to the data.txt file and its format
// two files are created and stored in the software disk
// the flatfile holds the actual records of data.txt and each record is stored in an individual block
// the indexfile holds the records' date as keys and also the block the records are located at
// the user is able to search a record using its key to get the whole record
class Table : public Shell
{
public:
    Table(string diskname,int numberOfBlocks,int blockSize, string flatfile, string indexfile, string inputFile); // constructor
    //int buildTable(string inputFile);
    int search(string value); // lets the user search a record using dates as keys
private:
	int indexSearch(string value); // returns the block the record is located at
	int buildTable(string inputFile); // builds the table
	int readTable(string inputFile); // reads the table
    string flatfile; // used to create flatfile
    string indexfile; // used to create indexfile
	string inputFile; // holds the data.txt file
    int count; // counts the number of records
};

#endif
