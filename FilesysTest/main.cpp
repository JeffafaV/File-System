#include "filesys.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;
int main()
{
Sdisk disk1("disk1.txt",256,128);
 Filesys fsys("disk1.txt",256,128);
 fsys.newFile("file1.txt");
 fsys.newFile("file2.txt");
 string bfile1;
 string bfile2;
 for (int i=1; i<=1024; i++) {
     bfile1+="1";
 }
 vector<string> blocks = fsys.block(bfile1, 128);
 int blocknumber = 0;
 for (int i=0; i < blocks.size(); i++) {
     blocknumber = fsys.addBlock("file1.txt", blocks[i]);
 }
 fsys.delBlock("file1.txt",fsys.getFirstBlock("file1.txt"));
 for (int i=1; i<=2048; i++)
    {
      bfile2+="2";
    }
 blocks = fsys.block(bfile2, 128);
 for (int i=0; i< blocks.size(); i++)
    {
      blocknumber = fsys.addBlock("file2.txt",blocks[i]);
    }

 fsys.delBlock("file2.txt",blocknumber);
}


