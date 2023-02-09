#include <iostream>
//#include "sdisk.h"
//#include "filesys.h"
#include "shell.h"
#include "table.h"

using namespace std;

int main() {

// This main program inputs commands to the shell.
// It inputs commands as : command op1 op2
// You should modify it to work for your implementation.

    //Sdisk sdisk = Sdisk("sdisk.txt",256,128);
    //Filesys fsys = Filesys("sdisk.txt",256,128);
    //Shell shell = Shell("sdisk.txt",256,128);
    Table table = Table("sdisk.txt",256,128,"flatfile","indexfile","data.txt");
    //table.Build_Table("data.txt");

    string s;
    string command = "go";
    string op1, op2;

    while (command != "quit") {
        cin.clear();
        command.clear();
        op1.clear();
        op2.clear();
        cout << "$ ";
        getline(cin,s);

        size_t firstBlank = s.find(' ');
        if (firstBlank < s.length()) s[firstBlank] = '#';
        size_t secondBlank = s.find(' ');

        command = s.substr(0, firstBlank);

        if (firstBlank < s.length()) {
            op1 = s.substr(firstBlank + 1, secondBlank - firstBlank - 1);
        }

        if (secondBlank < s.length()) {
            op2 = s.substr(secondBlank + 1);
        }

        if (command == "dir") {
            table.dir();
            continue;
        }

        if (command == "add") {
            // The variable op1 is the new file
            table.add(op1);
            continue;
        }

        if (command == "del") {
            // The variable op1 is the file
            table.del(op1);
            continue;
        }

        if (command == "type") {
            // The variable op1 is the file
            table.type(op1);
            continue;
        }

        if (command == "copy") {
            // The variable op1 is the source file and the variable op2 is the destination file.
            table.copy(op1, op2);
            continue;
        }
        if (command == "search") {
            // The variable op1 is the date
            table.search(op1);
            continue;
        }
        if (command == "quit") {}
        else {cout << "command not found" << endl;}

    }
    return 0;
}
