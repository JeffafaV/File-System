#include <iostream>
#include "shell.h"
//#include "table.h"
#include <string>

using namespace std;

// inherits disk, numberOfBlocks, and blockSize from filesys
Shell::Shell(string disk, int numberOfBlocks, int blockSize):Filesys(disk, numberOfBlocks, blockSize)
{
	// there are no member variables to assign
}

// lists all files in the file system
int Shell::dir()
{
	// contains all the files in the file system
    vector<string> fileList = ls();
	
	// loops through and prints all the files
    for (int i = 0; i < fileList.size(); i++)
	{
        cout << fileList[i] << endl;
    }
	
    return 1;
}

// adds a new file and its content in the file system and disk
int Shell::add(string file)
{
	// empty file name
	if (file.empty())
	{
		cout << "No file name was given" << endl;
		
		return 0;
	}
	
	// get error code to check if new file was created successfully
    int code = newFile(file);
	
	// couldn't create new file
	if (code == 0)
	{
		cout << "Cannot add file, either it already exists or root directory is full" << endl;
		
		return 0;
	}
	
	// holds the data in the file entered by user
    string text;
    cout << "Enter text: " << endl;
    getline(cin, text);
	
	// separate the data into blockSize sized chunks
    vector<string> blocks = block(text, getBlockSize());
	
	// insert the data into the disk
    for (int i = 0; i < blocks.size(); i++)
	{
		// get error code and add data to a block
        int code2 = addBlock(file, blocks[i]);
		
		// couldn't add data, no space left in disk
		if (code2 == 0)
		{
			cout << "No space left in disk" << endl;
			
			return 0;
		}
    }
	
    return 1;
}

// deletes a file from the file system and disk
int Shell::del(string file)
{
	// empty file name
	if (file.empty())
	{
		cout << "No file name was given" << endl;
		
		return 0;
	}
	
	// get the first block of the file
    int block = getFirstBlock(file);
	
    // file doesn't exist
	if (block == -1)
	{
		cout << "File doesn't exist" << endl;
		
		return 0;
	}
	
	// loop through all the file's blocks
    while (block != 0)
	{
		// delete first block
        delBlock(file, block);
		
		// set block to the file's new first block
        block = getFirstBlock(file);
    }
	
	// delete file from the root directory
    rmFile(file);
	
	cout << "File successfully deleted" << endl;
	
    return 1;
}

// edits the name of a file in the root directory
int Shell:: edit(string file1, string newName)
{
	// empty file name
	if (file1.empty() || newName.empty())
	{
		cout << "No file name(s) was/were given" << endl;
		
		return 0;
	}
	
	// get error code to see if name change was successful
	int code = editName(file1, newName);
	
	if (code == 1)
	{
		cout << "Name successfully changed" << endl;
		
		return 1;
	}
	
	cout << "Couldn't change file name" << endl;
	
	return 0;
}

// displays the data of the file
int Shell::type(string file)
{
	// empty file name
	if (file.empty())
	{
		cout << "No file name was given" << endl;
		
		return 0;
	}
	
	// get first block of file
    int block = getFirstBlock(file);
    
	// file doesn't exist
	if (block == -1)
	{
        cout << "File does not exist" << endl;
		
        return 0;
    }
	
	// holds all the data of the file
    string output = "";
	
	// holds a block of data of the file
    string b = "";
	
	// loop through all the blocks of the file
    while (block != 0)
	{
		// reads the current block and stores its data in b
        readBlock(file, block, b);
		
		// move to the next block of file
        block = nextBlock(file, block);
		
		// insert the data to output
        output += b;
    }
	
	/* this also displays the empty character #, not sure how to deal with that*/
    cout << output << endl;

    return 1;
}

// copies the content of file1 then
// creates file2 and pastes the content to file2
int Shell::copy(string file1, string file2)
{
	// empty file name
	if (file1.empty() || file2.empty())
	{
		cout << "No file name(s) was/were given" << endl;
		
		return 0;
	}
	
	// get the error code of the first file
    int block = getFirstBlock(file1);
    
	// first file doesn't exist
	if (block == -1)
	{
        cout << "file does not exist" << endl;
        return 0;
    }
	
	// get the error code of the second file
    int code2 = getFirstBlock(file2);
	
	// second file already exists
    if (code2 != -1)
	{
        cout << "file 2 exists" << endl;
    }
	
	// get the error code and create file2 in the root
    int code3 = newFile(file2);
	
	// file2 was not created successfully
    if (code3 == 0)
	{
        cout << "no space in root" << endl;
		
        return 0;
    }
    
	// holds a block of data of the first file
	string b = "";
	
	// loop through the first file's blocks
	// and add their data to the second file
    while (block != 0)
	{
		// reads the current block of file1 and stores its data in b
        readBlock(file1, block, b);
		
		// get error code and add data to a block in file2
        int code4 = addBlock(file2, b);
		
		// couldn't add data, no space left in disk
        if(code4 == 0)
		{
            cout << "no space left" << endl;
			
            return 0;
        }
		
		// go to the next block of file1
        block = nextBlock(file1, block);
    }
    return 1;
}
