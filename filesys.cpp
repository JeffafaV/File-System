#include "filesys.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

// builds or reads the file system in the disk
// the 0th block will be the boot block and will stay untouched (in memory this block actually holds the first free block)
// the 1st block will be the root block and will contain the root directory
// the 2nd to 2nd+fatSize block will be the fat blocks and will contain a block number for every block in the disk
Filesys::Filesys(string disk, int numberOfBlocks, int blockSize):Sdisk(disk, numberOfBlocks, blockSize)
{
	// each file in root has 13 characters: 8 chars for name, 3 digits for first block, and 2 spaces
	int charsPerFile = 13; // can change value
	
	// the rootSize is the max number of files that can be in the file system
    rootSize = getBlockSize()/charsPerFile;
	
	// converts the numberOfBlocks value into a string
	string numOfBlockStr = to_string(numberOfBlocks);
	
	// this gets the max number of space we need to store a block in the FAT
	// for 0-9 blocks we need space for 1 digit and 1 space (give 2 chars per block)
	// for 10-99 blocks we need space for 2 digits and 1 space (give 3 chars per block)
	// for 100-999 blocks we need space for 3 digits and 1 space (give 4 chars per block)
	// and so on
	// note that this can create wasted blocks on the FAT because if charsPerBlock is 4 
	// then we are giving extra and wasted space for blocks 0-9 and 10-99
	int charsPerBlock = numOfBlockStr.length()+1;
	
	// store charsPerBlock or other value in the multiplier
	double multiplier = charsPerBlock; // can change value
	
	// fatSize is an approximation of how many blocks are needed to store all the characters in the FAT
    fatSize = (multiplier * getNumberOfBlocks()) / getBlockSize() + 1;
	/*
		Note: the number of wasted blocks on the FAT can vary with this estimation and can be changed 
		to waste less blocks by changing the multiplier variable. Use the blocksNeededForFat.cpp 
		program to test values for the multiplier
	*/
	
	// used to check if file system exists
    string bufferFS;
	// used to hold first block of disk
	string bufferError;
	
	// check to see if disk exists
	int errorCode = getBlock(0,bufferError);
	if (errorCode == 0)
	{
		cout << "Disk doesn't exist" << endl;
		return;
	}
	
	// checking if the first real block is empty or not
    getBlock(1, bufferFS);

	// file system not present in the disk
    if (bufferFS[0] == '#')
	{
		// create file system
        buildFS();
    }

    else // file system present in the disk
	{
		// read file system
        readFS();
    }
}

// create file system on disk which consists of a root and fat
void Filesys::buildFS()
{
	// no filesys
	cout << "No file system exits. Creating file system..." << endl;
	
	// create root directory
	// initial root will have x's in place of file names and will have a first block of 0
	for (int i = 0; i < rootSize; i++)
	{
		filename.push_back("xxxxxxxx"); /* i think wrong number of x's?*/
		firstBlock.push_back(0);
	}
	
	// create filesys
	// should probably write a summary here of the different type of blocks which are reserved blocks, end-of blocks, and empty blocks
	// keep in mind that the 0th block in the disk file will be untouched
	// in memory the 0th block will be used to hold the first empty block available
	fat.push_back(fatSize + 2); // push the first free block to the fat vector
	fat.push_back(1); // pushing 1 for the root
	
	for (int i = 0; i < fatSize; i++) // pushing 1 for the fat blocks
	{
		fat.push_back(1);
	}
	
	// create link list of empty blocks in fat
	// each block's entry in the FAT will be the index of the next block
	for (int i = fatSize + 2; i < getNumberOfBlocks(); i++)
	{
		fat.push_back(i + 1);
	}
	
	// last block in FAT should have an entry of 0 to represent end of empty blocks
	fat[fat.size()-1] = 0;
	
	// everything above was done in memory, now we have to write our changes to the disk
	// write FS and Root to disk
	fsSynch();
	
	cout << "File system created." << endl;
}

// read the file system and store the root and fat in memory
void Filesys::readFS()
{
	// read in filesys
	cout << "File system exits. Reading in file system..." << endl;
	// stream used to read the file system from the disk
	istringstream instreamR;
	
	// holds the root from the disk
	string bufferR;
	// insert the root from the disk into the buffer
	getBlock(1, bufferR);
	
	// read in Root Directory
	// loading the buffer into the istringstream
	instreamR.str(bufferR);
	for (int i = 0; i < rootSize; i++) // inserting the root into memory
	{
		string f; // used to store the file name
		int b; // used to store the first block of the file name
		instreamR >> f >> b; // inserting file name and first block to the variables from the stream
		filename.push_back(f); // inserting file name to the filename vector
		firstBlock.push_back(b); // inserting first block to the firstBlock vector
	}
	
	// read in FAT
	istringstream instreamF;
	string bufferF;
	
	// reading in the FAT from the disk and inserting it into memory
	for (int i = 0; i < fatSize; i++)
	{
		getBlock(i+2, bufferF); // inserting one of the blocks from the FAT into the buffer
		
		instreamF.str(instreamF.str() + bufferF); // adding all fat blocks together in the stream
	}
	
	// inserting the content of the stream into the fat vector
	for(int s; instreamF >> s;)
	{
		fat.push_back(s);
	}
	
	cout << "Complete." << endl;
}

int Filesys::fsClose()
{
    return 0;
}

// writes the FAT and root directory into the disk file using the root and fat from memory
// note that the root consists of two parts, filename and firstBlock
int Filesys::fsSynch()
{
	// stream used to hold the root
	// note that we use a string stream because it's simpler than concatenating a string
    ostringstream outstreamR;

    // Root Directory
	// inserting the root in memory into the ostringstream
	// rootSize is the max files we can have in the file system
    for (int i = 0; i < rootSize; i++)
	{
        outstreamR << filename[i] << " " << firstBlock[i] << " ";
    }
	
	// buffer holding the root
    string bufferR = outstreamR.str();
	
	// blocking the root
	// the root is only one block so we're just filling in the remaining empty spaces if there are any
    vector<string> blocksR = block(bufferR, getBlockSize());
	
	//inserting the root into block 1 of the disk
	putBlock(1,blocksR[0]);
	
	// stream used to hold the fat
	ostringstream outstreamF;
	//string bufferF;

    // Filesys
	// insert the fat in memory into the ostringstream
    for (int i = 0; i < fat.size(); i++)
	{
        outstreamF << fat[i] << " "; // space is necessary and is accounted for in the fat in the disk
    }
	
	// buffer holding the fat
    string bufferF = outstreamF.str();
	
	// blocking the fat, blocks.size should be the same as fatsize or less?
    vector<string>blocksF = block(bufferF, getBlockSize());
    for (int i = 0; i < blocksF.size(); i++)
	{
        putBlock(i + 2, blocksF[i]); // inserting the fat from memory into the disk
    }
    return 0; // Success
}

// inserts a new file entry to the root directory
// fails if root is full or file already exists
int Filesys::newFile(string file)
{
	// check if the file already exists in the root
    for (int i = 0; i < filename.size(); i++)
	{
        if (filename[i] == file)
		{
			cout << "file already exists" << endl;
            return 0;
        }
    }
	
	// check if there is space in the root
    for (int i = 0; i < filename.size(); i++)
	{
        if (filename[i] == "xxxxxxxx") /* I want to change the number of x's*/
		{
			// add file and starting block 0 to the root memory
            filename[i] = file;
			
			// write to the root disk the new file
            fsSynch();
            return 1;
        }
    }
	
	cout << "no room left in the root for a new file" << endl;
    
    return 0;
}

// deletes an empty file from the root directory
// returns 1 if file is successfully deleted
// returns 0 if file is not empty or file doesn't exist
int Filesys::rmFile(string file)
{
	// scan each entry in root
    for (int i = 0; i < filename.size(); i++)
	{
        if(filename[i] == file)
		{
			// checks if file is not empty
            if (firstBlock[i] != 0)
			{
                cout << "file is not empty" << endl;
                return 0;
            }
            else // file is empty
			{
				// removing file
                filename[i] = "xxxxxxxx";
				// writing the changed root to the disk
                fsSynch();
                return 1;
            }
        }
    }
	
	cout << "file doesn't exist" << endl;
	
    return 0;
}

// edits the name of a file in the root directory of the sdisk
// returns 0 if file doesn't exist
// return 1 if name is successfully changed
int Filesys:editName(string file, string newName)
{
	// loop through the file names in the root directory
	for (int i = 0; i < filename.size(); i++)
	{
		// finding the file we want to rename
		if (filename[i] == file)
		{
			cout << "Changing file name of " << filename[i] << " to " << newName << endl;
			// changing file name
			filename[i] == newName;
			
			return 1;
		}
	}
	
	// file entered does not exist in sdisk
	cout << "File " << file << " does not exist" << endl;
	
	return 0;
}

// returns the first block of a file
// if the first block is 0 that just means the file is empty
// if file is not found then return -1
int Filesys::getFirstBlock(string file)
{
	// scan each entry in root
	for (int i = 0; i < filename.size(); i++)
	{
        if (filename[i] == file)
		{
            return firstBlock[i];
        }
    }
	cout << "no such file exists" << endl;
	
    return -1;
}

// adds a block to a file with the data in buffer
int Filesys::addBlock(string file, string buffer)
{
	/*perhaps I should add a check for if buffer.size != getBlockSize*/
	
	// stores error code -1 or first block of file
    int first = getFirstBlock(file);
	// file doesn't exist
    if (first == -1)
	{
        return 0;
    }
	// stores original first free block or 0 for no free blocks
    int allocate = fat[0];
	// no more free space in the disk
    if (allocate == 0)
	{
        // no free blocks
        return 0;
    }
	// update the free list in the fat in memory
	// new first free block is obtained by using the original first free block 
	// as an index and copying its element
    fat[0] = fat[fat[0]];
	// set the original first free block's element to 0 signifying end of file
    fat[allocate] = 0;
	
	// first block of file is 0 (empty file), update the root and fat
    if(first == 0) // First block to be added to the file
	{
		// find the file in the root
		for (int i = 0; i < filename.size(); i++)
		{
			// found the file
            if (filename[i] == file)
			{
				// update the first block of that file in the root in memory
                firstBlock[i] = allocate;
            }
        }
    }
	// first block of file is not 0 (non-empty file), update the fat
    else // find the previous last block
	{	
		// used to traverse through the file's blocks
        int block = first;
		
		// loop through the file's blocks until the original last block is found
        while(fat[block] != 0)
		{
			// move to the next block in the file
			block = fat[block];
        }
		
		// link the old last block to the new last block
        // i.e. the previous last block's element is set to the new last block
        fat[block] = allocate;
    }
	
	// putting the content of buffer into the new last block of file
	putBlock(allocate, buffer);
	
	// update the fat and root in the disk
	fsSynch();
	
	return allocate;
}

// removes block blockNumber from a file if that block is part of that file
int Filesys::delBlock(string file, int blockNumber)
{
	// check if block belongs to the file
    if (!checkBlock(file,blockNumber))
	{
        return 0;
    }
	/*can keep this for clarity but it is unnecessary, we can just use blockNumber*/
    int deallocate = blockNumber;
	
    // block to be deleted is the first block
    if(blockNumber == getFirstBlock(file))
	{
		// go through the root to find and change the first block of file
        for (int i = 0; i < filename.size(); i++)
		{
			// found the file in the root
            if(file == filename[i])
			{
				// the next block in the file is the new first block
				firstBlock[i] = fat[blockNumber];
            }
        }
    }
	/*I'm not sure when this else will ever run if we delete a file starting with its first block
	since the next block will then be the new first block and the if will run again*/
    else // block to be deleted is not the first block
	{
        int iBlock = getFirstBlock(file);
		// search for the block who's element is the block we want to delete
        while (fat[iBlock] != deallocate)
		{
            iBlock = fat[iBlock];
        }
		// the block that links with the block we want to delete 
		// will hold that soon to be deleted block's element
        fat[iBlock] = fat[deallocate];
    }
	// the soon to be deleted block now links to the first free block (fat[0] block)
    fat[deallocate] = fat[0];
	// the new first free block (fat[0] block) is now the deleted block
    fat[0] = deallocate;
	// update the fat and root in the disk
    fsSynch();
    return 1; // success
}

// stores the content of block blockNumber into the buffer
int Filesys::readBlock(string file, int blockNumber, string& buffer)
{
	// check if blockNumber belongs to the file
    if (checkBlock(file, blockNumber))
	{
		// read the block and store the data into buffer
        getBlock(blockNumber, buffer);
        return 1; // success
    }
	
	cout << "block doesn't belong to the file" << endl;
    return 0;
}

// writes to the disk using the data of buffer
int Filesys::writeBlock(string file, int blockNumber, string buffer)
{
	// check if blockNumber belongs to the file
    if (checkBlock(file, blockNumber))
	{
		// write to the block with the data in buffer
        putBlock(blockNumber, buffer);
        return 1; // success
    }
	
	cout << "block doesn't belong to the file" << endl;
    return 0;
}

// gets the next block after block blockNumber of a file
int Filesys::nextBlock(string file, int blockNumber)
{
	// check if blockNumber belongs to the file
    if(checkBlock(file, blockNumber))
	{
		// return the next block
        return fat[blockNumber]; // success
    }
	
	cout << "block doesn't belong to the file" << endl;
    return -1;
}

// checks if block blockNumber is associated with a certain file
bool Filesys::checkBlock(string file, int blockNumber)
{
	// get the first block of the file
    int iBlock = getFirstBlock(file);
	
	// file doesn't exist
	if (iBlock == -1)
	{
		cout << "file doesn't exist" << endl;
		return false;
	}
	
	// traverse the blocks of the file
    while(iBlock != 0)
	{
		// check if the current block is the same as blockNumber
        if(iBlock == blockNumber)
		{
            return true;
        }
		// set iBlock to the next linked block of the file
        iBlock = fat[iBlock];
    }
	
	// block does not belong to the file
	cout << "block doesn't belong to the file" << endl;
    return false;
}

// separates the buffer into blockSize chunks and is stored in a vector
// any empty spaces in a block chunk will be filled with # signs
// int b is blockSize
vector<string> Filesys::block(string buffer, int b)
{
    // holds the buffer in block chunks
    vector<string> blocks;
	
	// used to find the number of blocks needed to hold the buffer
    int numberofblocks = 0;
	
	// checking if the number of characters in the buffer divide evenly into each block
	// if not then add one block to numberofblocks since there is not enough blocks to hold the buffer
    if (buffer.length() % b == 0)
	{
        numberofblocks = buffer.length() / b;
    }
    else // adding one block to numberofblocks
	{
        numberofblocks = buffer.length() / b + 1;
    }
	
	// tempblock used to hold a substring of buffer
    string tempblock;
	// splitting the string buffer into numberofblocks chunks
    for (int i=0; i<numberofblocks; i++)
	{
		// set tempblock to a chunk (substring) of buffer
        tempblock = buffer.substr(b * i, b);
		// push tempblock to the vector blocks
        blocks.push_back(tempblock);
    }
	
	// finding the last block of the vector
    int lastblock = blocks.size() - 1;
	
	// filling in any empty spaces of the last block in the vector with #
    for (int i = blocks[lastblock].length(); i < b; i++)
	{
        blocks[lastblock] += "#";

    }
	
	// return the vector
    return blocks;
}

// returns a vector of every file in the file system
vector<string> Filesys::ls()
{
	// holds all the files in the root
    vector<string> fileList;
	
	// loop through the whole root space and insert files to fileList
	for (int i = 0; i < filename.size(); i++)
	{
		if (filename[i] != "xxxxxxxx")
		{
			fileList.push_back(filename[i]);
		}
	}
	
    return fileList;
}
