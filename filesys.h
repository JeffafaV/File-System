#ifndef FILESYS_H
#define FILESYS_H

#include "sdisk.h"
#include <vector>
#include <string>

using namespace std;

// creation of file system which inherits sdisk
// creates a new file system or reads the current one in the disk
// file system can add or remove files from the disk
// block functions are used to insert and remove files block by block
// file system consists of a root, a fat, and an untouched 0th block
class Filesys: public Sdisk
{
public:
    Filesys(string disk, int numberOfBlocks, int blockSize); // creates or reads the root and FAT
    int fsClose(); /*never used might delete*/
	/*I would like to create a function that edits file names*/
	/*add a way to add folders to the filesys*/
    int newFile(string file); // adds a new file to the root directory
    int rmFile(string file); // deletes a file from the root directory
    int getFirstBlock(string file); // returns the first block of a file
    int addBlock(string file, string buffer); // adds a block to a file
    int delBlock(string file, int blockNumber); // deletes a block from a file
    int readBlock(string file, int blockNumber, string& buffer); // stores content of a block into the buffer
	/*write block is never used, however, I can perhaps use it if I add a function to edit files*/
    int writeBlock(string file, int blockNumber, string buffer); // writes to a block using the content of buffer
    int nextBlock(string file, int blockNumber); // finds the next block that's after block blockNumber of file
    vector<string> ls();
	vector<string> block(string buffer, int b);
private:
    int fsSynch(); // writes the FAT and root directory to the disk file using the fat and root from memory
    int rootSize; // maximum number of entries (files) in ROOT
    int fatSize; // number of blocks the FAT will use
    vector<string> filename; // filenames in ROOT, first vector of root /*fileName*/
    vector<int> firstBlock; // firstblocks in ROOT, second vector of root
    vector<int> fat; // file allocation table (FAT) will be of size numberOfBlocks and will hold an entry for each block in the disk
    bool checkBlock(string file, int blockNumber); // checks if a certain block is in a file
	void readFS(); // reads a previously created file system in the disk
	void buildFS(); // creates a new file system in the disk
};

#endif
