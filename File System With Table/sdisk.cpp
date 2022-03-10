#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "sdisk.h"

using namespace std;

Sdisk::Sdisk(string diskName, int numberOfBlocks, int blockSize)
{
    fstream infile;
    infile.open(diskName.c_str());
    if (!infile.good())
	{
		cout << "No disk found. Creating disk \"" << diskName << "\"" << endl;
		ofstream outfile;
		outfile.open(diskName.c_str());
        this->diskName = diskName;
        this->numberOfBlocks = numberOfBlocks;
        this->blockSize = blockSize;

        string buffer;
        ostringstream outstream;

        /** Fill disk with "#" */
        for (int i = 0; i <= numberOfBlocks * blockSize; i++)
		{
            outstream << "#";
        }
        buffer = outstream.str();
        vector<string> blocks = block(buffer, getBlockSize());
        for (int i = 0; i < numberOfBlocks; i++)
		{
            putBlock(i, blocks[i]);
        }

        outfile.close();
        cout << "Disk \"" << diskName << "\" created." << endl;
    }
    else
	{
        cout << "Disk \"" << diskName << "\" found." << endl;
        this->diskName = diskName;
        this->numberOfBlocks = numberOfBlocks;
        this->blockSize = blockSize;
    }
    infile.close();
    return;
}

int Sdisk::getBlock(int blockNumber, string& buffer)
{
    fstream iofile;
    string tbuffer;
    iofile.open(diskName.c_str(), ios::in | ios::out);
    iofile.seekg(blockNumber * blockSize);
    char c;
    for (int i = 0; i < blockSize; i++)
	{
		c = iofile.get();
        tbuffer.push_back(c);
    }

    buffer = tbuffer;
    iofile.close();

    return 1;
}

int Sdisk::putBlock(int blockNumber, string buffer)
{
    fstream iofile;
    iofile.open(diskName.c_str(), ios::in | ios::out);
    iofile.seekp(blockNumber * blockSize);
    for (string::iterator it = buffer.begin(); it < buffer.end(); it++)
	{
        iofile.put(*it);
    }
    iofile.close();

    return 1;
}

vector<string> Sdisk::block(string buffer, int b)
{
    // blocks the buffer into a list of blocks of size b

    vector<string> blocks;
    size_t numberofblocks = 0;

    if (buffer.length() % b == 0)
	{
        numberofblocks = buffer.length() / b;
    }
    else
	{
        numberofblocks = buffer.length() / b + 1;
    }

    string tempblock;
    for (int i=0; i<numberofblocks; i++)
	{
        tempblock = buffer.substr(b * i, b);
        blocks.push_back(tempblock);
    }

    size_t lastblock = blocks.size() - 1;

    for (size_t i = blocks[lastblock].length(); i < b; i++)
	{
        blocks[lastblock] += "#";

    }

    return blocks;
}



int Sdisk::getNumberOfBlocks()
{
    return numberOfBlocks;
}

int Sdisk::getBlockSize()
{
    return blockSize;
}

string Sdisk::getDiskName()
{
    return diskName;
}
