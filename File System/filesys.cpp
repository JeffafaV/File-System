#include "filesys.h"
#include <sstream>
#include <iostream>

using namespace std;

Filesys::Filesys(string disk, int numberOfBlocks, int blockSize):Sdisk(disk, numberOfBlocks, blockSize) {
    rootSize = getBlockSize()/13;
    fatSize = (4 * getNumberOfBlocks()) / getBlockSize() + 1; //getBlockSize()/4 + 1;
    string buffer;
    getBlock(1, buffer);

    if (buffer[0] == '#') {
        // no filesys
        cout << "No file system exits. Creating file system..." << endl;
        ostringstream outstream;

        /** Create Root Directory */
        for (int i = 1; i <= rootSize; i++) {
            filename.push_back("xxxxxxxx");
            firstBlock.push_back(0);
        }

        /** Create filesys */
        fat.push_back(fatSize + 1);
        for (int i = 1; i <= fatSize; i++) { // to fatSize
            fat.push_back(-1);
        }
        for (int i = fatSize + 1; i < getNumberOfBlocks(); i++) {
            fat.push_back(i + 1);
        }
        fat[fat.size()-1] = 0;

        /** Write FS and Root to disk */
        fsSynch();
        cout << "File system created." << endl;
    }

    else {
         // read in filesys
        cout << "File system exits. Reading in file system..." << endl;
        istringstream instream;
        buffer.clear();
        getBlock(1, buffer);

        /** Read in Root Directory */
        instream.str(buffer);
        for (int i = 0; i < rootSize; i++) {
            string f;
            int b;
            instream >> f >> b;
            filename.push_back(f);
            firstBlock.push_back(b);
        }

        /** Read in FAT */
        instream.str("");

        for (int i = 2; i <= fatSize + 2; i++) {
            getBlock(i, buffer);
            instream.str(instream.str() + buffer);
        }

        for(int s; instream >> s;) {
            fat.push_back(s);
        }
        cout << "Complete." << endl;
    }
}

int Filesys::fsClose() {
    return 0;
}

int Filesys::fsSynch() {
    string buffer;
    ostringstream outstream;

    /** Root Directory */
    for (int i = 0; i < rootSize; i++) {
        outstream << filename[i] << " " << firstBlock[i] << " ";
    }

    buffer = outstream.str();
    vector<string> blocks = block(buffer, getBlockSize());
    for (int i = 0; i < blocks.size(); i++) {
        putBlock(i + 1, blocks[i]);
    }
    outstream.str("");

    /** Filesys */
    for (int i = 0; i < fat.size(); i++) {
        outstream << fat[i] << " ";
    }

    buffer = outstream.str();
    blocks = block(buffer, getBlockSize());
    for (int i = 0; i < blocks.size(); i++) {
        putBlock(i + 2, blocks[i]);
    }
    return 0; // Success
}

int Filesys::newFile(string file) {

    for (int i = 0; i < rootSize; i++) {
        if (filename[i] == file) {
            return 0;
        }
    }
    for (int i = 0; i < rootSize; i++){
        if (filename[i] == "xxxxxxxx") {
            filename[i] = file;
            fsSynch();
            return 1;
        }
    }

    fsSynch();
    return 0;
}

int Filesys::rmFile(string file) {
    for (int i = 0; i < rootSize; i++) {
        if(filename[i] == file) {
            if (firstBlock[i] != 0) {
                cout << "file is not empty" << endl;
                return 0;
            }
            else {
                filename[i] = "xxxxxxxx";
                fsSynch();
                return 1;
            }
        }
    }
    return 0;
}

int Filesys::getFirstBlock(string file) {
    for (int i = 0; i < rootSize; i++) {
        if (filename[i] == file) {
            return firstBlock[i];
        }
    }
    return -1;
}

int Filesys::addBlock(string file, string buffer) {
    int first = getFirstBlock(file);
    if (first == -1) {
        return 0;
    }
    int allocate = fat[0];
    if (allocate == 0) {
        // no free blocks
        return 0;
    }
    fat[0] = fat[fat[0]];
    fat[allocate] = 0;
    if(first == 0) { // First block to be added to the file
        for (int i = 0; i < rootSize; i++) {
            if (filename[i] == file) {
                firstBlock[i] = allocate;
                fsSynch();
                putBlock(allocate, buffer);
                return allocate;
            }
        }
    }
    else { // find the previous last block
        int block = first;
        while(block != 0) {
            if(fat[block] != 0) {
                block = fat[block];
            }
            else break;
        }
        // update free list
        fat[block] = allocate;
        fsSynch();
        putBlock(allocate, buffer);
        return allocate;
    }
    return allocate;
}

int Filesys::delBlock(string file, int blockNumber) {
    if (!checkBlock(file,blockNumber)) {
        return 0;
    }
    int deallocate = blockNumber;
    // check to see if block to be deleted is the first block
    if(blockNumber == getFirstBlock(file)) {
        for (int i = 0; i < filename.size(); i++) {
            // check to see if block to be deleted has subsequent blocks
            // if so, connect previous block to next block
            if(file == filename[i]) {
                if(nextBlock(file, blockNumber) != -1) {
                    firstBlock[i] = nextBlock(file, blockNumber);
                }
                // if not, add blockNumber to free list
                else firstBlock[i] = fat[deallocate];
                break;
            }
        }
    }
    // otherwise, delete the block and add the block to the free list
    else {
        int iBlock = getFirstBlock(file);
        while (fat[iBlock] != deallocate) {
            iBlock = fat[iBlock];
        }
        fat[iBlock] = fat[deallocate];
    }
    fat[deallocate] = fat[0];
    fat[0] = deallocate;
    fsSynch();
    return 1;
}

int Filesys::readBlock(string file, int blockNumber, string& buffer) {
    if (checkBlock(file, blockNumber)) {
        getBlock(blockNumber, buffer);
        return 1;
    }
    else return 0;
}

int Filesys::writeBlock(string file, int blockNumber, string buffer) {
    if (checkBlock(file, blockNumber)) {
        putBlock(blockNumber, buffer);
        return 1;
    }
    else return 0;
}

int Filesys::nextBlock(string file, int blockNumber) {
    if(checkBlock(file, blockNumber)) {
        return fat[blockNumber];
    }
    else return -1;
}

bool Filesys::checkBlock(string file, int blockNumber) {
    int iBlock = getFirstBlock(file);
    while(iBlock != 0) {
        if(iBlock == blockNumber) {
            return true;
        }
        iBlock = fat[iBlock];
    }
    return false;
}

vector<string> Filesys::ls() {
    vector<string> fileList;
        for (int i = 0; i < filename.size(); i++) {
            if (filename[i] != "xxxxxxxx") {
                fileList.push_back(filename[i]);
            }
        }
    return fileList;
}

