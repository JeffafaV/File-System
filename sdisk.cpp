#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "sdisk.h"

using namespace std;

// creation and formatting of the disk
// blocks are numbered through 0 to numberOfBlocks-1
// char spaces (bytes) are numbered through 0 to numberOfBlocks*blockSize-1
Sdisk::Sdisk(string diskName, int numberOfBlocks, int blockSize)
{
	// stream that will read the disk file
    fstream infile;
	// open disk file
    infile.open(diskName.c_str());
	
	// setting private member variables
	this->diskName = diskName;
	this->numberOfBlocks = numberOfBlocks;
	this->blockSize = blockSize;
	
	// disk file does not exist, create disk file
    if (!infile.good())
	{
		cout << "No disk found. Creating disk \"" << diskName << "\"" << endl;
		
		// stream that will write to the disk file
		ofstream outfile;
		// creating and then opening disk file
		outfile.open(diskName.c_str());
		
		// fill disk with # for numberOfBlocks*blockSize char spaces
        for (int i = 0; i < numberOfBlocks * blockSize; i++)
		{
            outfile.put('#');
        }
		
        cout << "Disk \"" << diskName << "\" created." << endl;
    }
    else // disk file exists
	{
        cout << "Disk \"" << diskName << "\" found." << endl;
    }
	
	// closing disk file reading stream
    infile.close();
	
    return;
}

// stores the content of block blockNumber in the buffer
// call by reference on buffer
/* should check if blockNumber is in range*/
int Sdisk::getBlock(int blockNumber, string& buffer)
{
	// stream that will read the disk file
    fstream iofile;
	// temporary buffer that will hold block blockNumber's content
    string tbuffer;
	
	// open disk file
    iofile.open(diskName.c_str(), ios::in | ios::out);
	
	// check if file did not open successfully
	if (iofile.fail())
	{
		return 0; // failure
	}
	
	// move get pointer to the correct block in stream/file
    iofile.seekg(blockNumber * blockSize);
	
	// used to store current char in the stream
    char c;
	// iterating through each char space in block blockNumber
    for (int i = 0; i < blockSize; i++)
	{
		// store the current char in the stream in c (reading the disk file)
		c = iofile.get();
		// push c in the temp buffer tbuffer
        tbuffer.push_back(c);
    }
	
	// set buffer to tbuffer
    buffer = tbuffer;
	// close disk file
    iofile.close();

    return 1; // success
}

// stores the content of buffer in block blockNumber
/* should check if blockNumber is in range*/
int Sdisk::putBlock(int blockNumber, string buffer)
{
	// stream that will write on the disk file
    fstream iofile;
	// open disk file
    iofile.open(diskName.c_str(), ios::in | ios::out);
	
	// check if file did not open successfully
	if (iofile.fail())
	{
		return 0; // failure
	}
	
	// move put pointer to the correct block in stream/file
    iofile.seekp(blockNumber * blockSize);
	
	// iterating through each char space in block blockNumber with an iterator
    for (string::iterator it = buffer.begin(); it < buffer.end(); it++)
	{
		// pushing current char in buffer to the stream (writing to the disk file)
        iofile.put(*it);
    }
	
	// close disk file
    iofile.close();

    return 1; // success
}

// access the number of blocks in the software disk drive
int Sdisk::getNumberOfBlocks()
{
    return numberOfBlocks;
}

// access the block size of each block in the disk drive
int Sdisk::getBlockSize()
{
    return blockSize;
}

// access the disk name of the software disk drive
string Sdisk::getDiskName()
{
    return diskName;
}
