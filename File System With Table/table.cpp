#include "table.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

Table::Table(string diskname,int numberOfBlocks,int blockSize, string flatfile, string indexfile):Shell(diskname, numberOfBlocks, blockSize)
{
    this->flatfile = flatfile;
    this->indexfile = indexfile;
	
    count = 0;
}
int Table::Build_Table(string input_file)
{
    ifstream infile;
    infile.open(input_file.c_str());
    int b = newFile(flatfile);
    int c = newFile(indexfile);
	string record;
	getline(infile, record);
	
    if (b == 0 || c == 0)
    {
        while (infile.good())
        {
                count++;
                getline(infile, record);
        }
        cout << "Files exist" << endl;
        return 1;
    }

    vector<string> key;
    vector<string> iBlock;
    ostringstream outstream;

    while(infile.good())
	{
        // Process Record
        string primaryKey;
        primaryKey = record.substr(0, 5);
        vector<string>blocks = block(record, getBlockSize());
        int blockID = addBlock(flatfile, blocks[0]);
        outstream << primaryKey << " " << blockID << " ";
        count++;
        //cout << count;
        getline(infile, record);
    }

	//cout << outstream.str();
	vector<string> blocks2 = block(outstream.str(), getBlockSize());
	for (int i = 0; i < blocks2.size(); i++)
	{
		addBlock(indexfile, blocks2[i]);
	}
    return 1;
}

int Table::Search(string value)
{
    string date = value;
    if(value == "")
	{
        cout << "What date would you like to search for: ";
        cin >> date;
    }
    date = date.substr(0,5);
    cout << "Searching for record with date: " << date << "." << endl;
    string buffer;
    int block = IndexSearch(date);
    readBlock(flatfile, block, buffer);
    cout << buffer << endl;
    cin.clear();
    return 1;
}

int Table::IndexSearch(string value)
{
   istringstream instream;
   string k;
   int b;
   string bigBuffer;
    int blockID = getFirstBlock(indexfile);
	
    while (blockID != 0)
	{
        string buffer;
        readBlock(indexfile, blockID, buffer);
        bigBuffer += buffer;
        blockID = nextBlock(indexfile, blockID);
    }
	instream.str(bigBuffer);
	for (int i = 0; i <= count; i++)
	{
		instream >> k >> b;
		if (k == value) return b;
	}
	
    return 0;
}
