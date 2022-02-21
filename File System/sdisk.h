#ifndef SDISK_H
#define SDISK_H
#include <string>
#include <vector>

using namespace std;

class Sdisk {
public :
    Sdisk(string diskName, int numberOfBlocks, int blockSize);
    int getBlock(int blockNumber, string& buffer);
    int putBlock(int blockNumber, string buffer);
    vector<string> block(string buffer, int b);
    int getNumberOfBlocks(); // accessor function
    int getBlockSize(); // accessor function
    string getDiskName(); // accessor function
private :
    string diskName;        // file name of software-disk
    int numberOfBlocks;     // number of blocks on disk
    int blockSize;          // block size in bytes
};


#endif
