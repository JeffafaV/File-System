#include "table.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

// sets flatfile, indexfile and count
Table::Table(string diskname,int numberOfBlocks,int blockSize, string flatfile, string indexfile, string inputFile):Shell(diskname, numberOfBlocks, blockSize)
{
	// setting class member variables
    this->flatfile = flatfile;
    this->indexfile = indexfile;
	this->inputFile = inputFile;
    count = 0;
	
	/* TODO
	change error codes of newFile
	0 can either mean full root or file already exists*/
	// get error codes to see if flatfile and indexfile exist
	// if not then it creates them in the root
	int b = newFile(flatfile);
    int c = newFile(indexfile);
	
	// files exist, reading table
	if (b == 0 && c == 0)
	{
		readTable(inputFile);
	}
	// files don't exist, creating table
	else
	{
		buildTable(inputFile);
	}
}

// builds a table which consists of the indexfile and flatfile
int Table::buildTable(string inputFile)
{
	// stream used to read the data.txt file
    ifstream infile;
	// opens the data.txt file
    infile.open(inputFile.c_str());
	
	// check if data.txt was opened successfully
	if (!infile.good())
	{
		cout << "Unable to open text file" << endl;
		
		return 0;
	}
	
	// stores each individual record
	string record;
	
	// used to hold every records' key (date) and block
    ostringstream outstream;
	
	// reads each record in the data.txt file and writes to the flatfile
    while(getline(infile, record))
	{
		// holds the date/key of the record
        string primaryKey;
		// the date/key is always the first 5 characters (or less) of each record
        primaryKey = record.substr(0, 5);
		
		// blocking the record
        vector<string>blocks = block(record, getBlockSize());
		
		// inserts the blocked record to a block in the flatfile
		// blockID holds the block number that holds the record
        int blockID = addBlock(flatfile, blocks[0]);
		/*should probably check if blockID is a valid block number and not an error code*/
		
		// inserts the key/date and block number of a record for each record
        outstream << primaryKey << " " << blockID << " ";
		
		// counts the number of records
        count++;
    }
	
	// blocks all the records' block number and date/key
	vector<string> blocks2 = block(outstream.str(), getBlockSize());
	
	// writes the dates/keys and block numbers of each record to the indexfile
	for (int i = 0; i < blocks2.size(); i++)
	{
		addBlock(indexfile, blocks2[i]);
		/*should probably check if addBlock is a valid block number and not an error code*/
	}
	
	cout << "Creating files" << endl;
	
    return 1;
}

// reads the data.txt file since table was built already
int Table::readTable(string inputFile)
{
	// stream used to read the data.txt file
	ifstream infile;
	// opens the data.txt file
    infile.open(inputFile.c_str());
	
	// check if data.txt was opened successfully
	if (!infile.good())
	{
		cout << "Unable to open text file" << endl;
		
		return 0;
	}
	
	// stores each individual record
	string record;
	
	/* instead of counting records through the data.txt file we can 
	just count blocks in the flatfile since the table was already built*/
	// counting the records in data.txt
	while (getline(infile, record))
	{
		count++;
	}
	
	cout << "Files exist" << endl;
	
	return 1;
}

// prints a record by using the indexSearch function
int Table::search(string value)
{
	/*might get rid of this, if the user entered nothing then
	nothing should happen like if user entered wrong key*/
	// asks the user to enter date/key in case they didn't at first
    if(value == "")
	{
        cout << "What date would you like to search for: ";
        
		cin >> value;
    }
    
    cout << "Searching for record with date: " << value << "." << endl;
    
	// buffer will hold the block that has the record
	string buffer;
	
	// finds the block the record is located at using the date/key
	// if date/key is not associated with a record then block is 0
    int block = indexSearch(value);
	
	// check if block has a valid block number
	if (block == 0)
	{
		return 0;
	}
	
	/*I think I need to do this in the case that
	the user changes the name of the flatfile*/
	// get the first block of the flatfile
	int blockID = getFirstBlock(flatfile);
	
	// check to see nothing is wrong with indexFile
	// 0 or -1 means either the file is empty or doesn't exist
	// this shouldn't be possible unless the user changed the file's name
	// or deleted the file
	if (blockID <= 0)
	{
		cout << "Warning, file is corrupted. ";
		cout << "Perhaps you changed the name of the file. Rename it to flatfile and try again" << endl;
		
		return 0;
	}
	
	// reads the block and stores it in buffer
	// if the block is 0 then buffer is empty
    readBlock(flatfile, block, buffer);
    
	// output buffer
	cout << buffer << endl;
	
    return 1;
}

// returns the block that holds a record by using its key (date)
int Table::indexSearch(string value)
{
	// stream that holds keys and block numbers of all records
	istringstream instream;
	// holds key
	string k;
	// holds block number associated with key
	int b;
   
	// holds all records' keys and block numbers
	string bigBuffer;
   
	// get the first block of the indexfile
	int blockID = getFirstBlock(indexfile);
	
	// check to see nothing is wrong with indexFile
	// 0 or -1 means either the file is empty or doesn't exist
	// this shouldn't be possible unless the user changed the file's name
	// or deleted the file
	if (blockID <= 0)
	{
		cout << "Warning, file is corrupted. ";
		cout << "Perhaps you changed the name of the file. Rename it to indexfile and try again" << endl;
		
		return 0;
	}
	
	// loop through all the blocks of indexfile
    while (blockID != 0)
	{
		// used to hold current block's data
		// which is multiple keys and block numbers
        string buffer;
		
		// reads the block and stores it in the buffer
        readBlock(indexfile, blockID, buffer);
		
		// adds the current block's data to the bigBuffer
        bigBuffer += buffer;
		
		// move to the next block
        blockID = nextBlock(indexfile, blockID);
    }
	
	// inserts the bigBuffer into the string stream
	instream.str(bigBuffer);
	
	// iterate through each record's block number and date/key
	for (int i = 0; i < count; i++)
	{
		// get the key and block number of the current record
		instream >> k >> b;
		
		// found the key, return the record's block number
		if (k == value)
		{
			return b;
		}
	}
	
	cout << "Record with this key does not exist in the database" << endl;
	
    return 0;
}
