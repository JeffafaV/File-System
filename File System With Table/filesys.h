#ifndef FILESYS_H
#define FILESYS_H

#include "sdisk.h"
#include <vector>
#include <string>

using namespace std;

class Filesys: public Sdisk
{
public:
    Filesys(string disk, int numberOfBlocks, int blockSize);
    int fsClose();
    int newFile(string file);
    int rmFile(string file);
    int getFirstBlock(string file);
    int addBlock(string file, string buffer);
    int delBlock(string file, int blockNumber);
    int readBlock(string file, int blockNumber, string& buffer);
    int writeBlock(string file, int blockNumber, string buffer);
    int nextBlock(string file, int blockNumber);
    vector<string> ls();
private:
    int fsSynch();
    int rootSize; // maximum number of entries in ROOT
    int fatSize; // number of blocks occupied by FAT
    vector<string> filename; // filenames in ROOT
    vector<int> firstBlock; // firstblocks in ROOT
    vector<int> fat; // FAT
    bool checkBlock(string file, int blockNumber);
};

#endif
