#include"table.h"
#include "shell.h"
#include <sstream>
#include <iostream>
#include <fstream>

Table::Table(string diskname, int numberOfBlocks, int blockSize, string flatfile, string indexfile)
        : Shell(diskname, numberOfBlocks, blockSize)
{
        //Set the flat file name
        this->flatfile = flatfile;

        //Set the index file name
        this->indexfile = indexfile;

        //Set number of records to be 0
        count = 0;

        //Create the flat file and index file
        int b = newFile(flatfile);
        int c = newFile(indexfile);
        if (b == 0 || c == 0)
        {
                cout << "Could not create index or flat file" << endl;
        }
}

int Table::Build_Table(string input_file)
{
        //Open the input file
        ifstream infile;
        infile.open(input_file.c_str());

        //Could not open the file
        if (infile.bad())
        {
                return 0;
        }

        //Reads the first line of the file
        string s;
        getline(infile, s);

        //Output buffer used to write to index file
        ostringstream indexstream;

        while (infile.good())
        {

                //Pull off first 5 bytes (our key - the date)
                string key = s.substr(0, 5);

                
                vector<string> blocks = block(s, getBlockSize());

                //Add first block into flat file
                int b = addBlock(flatfile, blocks[0]);

                //File does not exist or block sizes do not match
                if (b <= 0)
                {
                        return 0;
                }

                
                indexstream << key << " " << b << " ";

                //Update the number of records
                count++;

                //Read the next line
                getline(infile, s);
        }

        //Convert indexstream to a string, write it to buffer
        string buffer = indexstream.str();

        //Break the buffer (what we're going to write to the index file) into blocks
        vector<string> blocks = block(buffer, getBlockSize());

        //Write our index file blocks to our index file
        for (int i = 0; i < blocks.size(); i++)
        {
                int b = addBlock(indexfile, blocks[i]);

                //File does not exist or block sizes do not match
                if (b == 0)
                {
                        return 0;
                }
        }

        //Success
        return 1;
}

int Table::IndexSearch(string value)
{
        //Read all blocks that are in the index file

        istringstream instream;

        //Used to hold contents of all blocks of index file
        string bigBuffer;

        //Returns the first block of the index file
        int b = getFirstBlock(indexfile);

        //Error ocurred
        if (b <= 0)
        {
                return 0;
        }

        //Read contents of index file, store it in bigBuffer
        while (b != 0)
        {
                //Buffer to hold contents of first block of index file
                string buffer;

                //Read the b block number of indexFile, store that block's contents into buffer
                int error = readBlock(indexfile, b, buffer);

                //Add contents of b block to our bigger buffer
                bigBuffer += buffer;

                //Get next block of index file
                b = nextBlock(indexfile, b);
        }

        //Process the big buffer

        //Load bigBuffer into instream
        instream.str(bigBuffer);

        for (int i = 0; i < count; i++)
        {
                string key;

                int keyBlock;

                //Read key and keyBlocks from instream
                instream >> key >> keyBlock;

                //We found the key we are looking for
                if (key == value)
                {
                        return keyBlock;
                }
        }

        cout << "Record does not exist in the database" << endl;
        return -1;
}

int Table::Search(string value)
{
        
        int b = IndexSearch(value);

        //Block does not exist
        if (b <= 0)
        {
                return -1;
        }

        //Buffer used to read the block
        string buffer;

        //Read the b block in flatFile, store it to buffer
        b = readBlock(flatfile, b, buffer);

        //Output what's in the buffer
        cout << buffer << endl;

        return b;
}

