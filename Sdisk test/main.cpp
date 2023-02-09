#include <iostream>
#include <string>
#include "sdisk.h"

using namespace std;

// The sample program for Lab 2
int main()
{
    Sdisk disk1("test1",16,32);
    string block1, block2, block3, block4;
    for (int i=1; i<=32; i++) block1=block1+"1";
    for (int i=1; i<=32; i++) block2=block2+"2";

    disk1.putBlock(4,block1);
    disk1.getBlock(4,block3);
    cout << "Should be 32 1s: ";
    cout << block3 << endl;
    disk1.putBlock(8,block2);
    disk1.getBlock(8,block4);
    cout << "Should be 32 2s: ";
    cout << block4 << endl;

}
