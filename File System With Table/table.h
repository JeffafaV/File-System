#ifndef table_h
#define table_h

#include "filesys.h"
#include "shell.h"
#include <stdio.h>
#include <string>


class Table : public Shell
{
public:
    Table(string diskname,int numberOfBlocks,int blockSize, string flatfile, string indexfile);
    int Build_Table(string input_file);
    int Search(string value);
private:
    string flatfile;
    string indexfile;
    int IndexSearch(string value);
    int count;
};

#endif
