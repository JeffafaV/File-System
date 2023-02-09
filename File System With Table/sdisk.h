#ifndef SDISK_H
#define SDISK_H
#include <string>
#include <vector>

using namespace std;

// creation of software disk drive
// the disk drive will have numberOfBlocks many blocks
// where each block has blockSize many char spaces (bytes)
// able to extract info from the disk and insert info to the disk
class Sdisk
{
public:
    Sdisk(string diskName, int numberOfBlocks, int blockSize); // creation/formatting of the disk drive
    int getBlock(int blockNumber, string& buffer); // stores the data of block blockNumber in buffer
    int putBlock(int blockNumber, string buffer); // stores the data in buffer into block blockNumber
    int getNumberOfBlocks(); // access numberOfBlocks
    int getBlockSize(); // access blockSize
    string getDiskName(); // access diskName
private:
    string diskName; // file name of software disk
    int numberOfBlocks; // number of blocks on disk
    int blockSize; // block size in char spaces
};

#endif
