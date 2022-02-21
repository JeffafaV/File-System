#include <iostream>
#include "shell.h"
//#include "table.h"
#include <string>

using namespace std;

Shell::Shell(string disk, int numberOfBlocks, int blockSize):Filesys(disk, numberOfBlocks, blockSize) {
    // todo:
}
/** lists all files */
int Shell::dir() {
    vector<string> fileList = ls();
    for (int i = 0; i < fileList.size(); i++) {
        cout << fileList[i] << endl;
    }
    return 1;
}

/* add a new file using input from the keyboard */
int Shell::add(string file) {
    newFile(file);
    string text;
    cout << "Enter text: ";
    getline(cin, text);
    vector<string> blocks = block(text, getBlockSize());
    for (int i = 0; i < blocks.size(); i++) {
        addBlock(file, blocks[i]);
    }
    return 1;
}

/** deletes the file */
int Shell::del(string file) {
    int block = getFirstBlock(file);
    int next = nextBlock(file, block);
    while (block != 0) {
        delBlock(file, block);
        block = next;
        next = nextBlock(file, block);
    }
    rmFile(file);
    return 1;
}

/** lists the contents of file */
int Shell::type(string file) {
    int first = getFirstBlock(file);
    if (first == -1) {
        cout << "file does not exist\n";
        return 0;
    }

    int iBlock = first; // iBlock is firstBlock
    size_t pos;
    string output = "";
    string b = "";
    while (iBlock != 0) {
        readBlock(file, iBlock, b);
        iBlock = nextBlock(file, iBlock);
        output += b;
    }
    pos = output.find("#");
    output = output.substr(0, pos);

    cout << output << endl;

    return 1;
}

/** copies file1 to file2 */
int Shell::copy(string file1, string file2) {
    int code = getFirstBlock(file1);
    if (code == -1) {
        cout << "file does not exist" << endl;
        return 0;
    }
    int code2 = getFirstBlock(file2);
    if (code2 != -1) {
        cout << "file 2 exists" << endl;
    }
    int code3 = newFile(file2);
    if (code3 == 0) {
        cout << "no space on root" << endl;
        return 0;
    }
    int iBlock = code; // iBlock is firstBlock
    while (iBlock != 0) {
        string b;
        readBlock(file1, iBlock, b);
        int code4 = addBlock(file2, b);
        if(code4 == -1) {
            cout << "no space left" << endl;
            del(file2);
            return 0;
        }
        iBlock = nextBlock(file1, iBlock);
    }
    return 1;
}

