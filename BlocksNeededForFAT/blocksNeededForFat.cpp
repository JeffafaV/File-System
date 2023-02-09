#include <iostream>
#include <string>

using namespace std;

/*
    I was making a big deal of this but it turns out that regardless of the number
    of wasted blocks used for the FAT, there will still be a huge number of 
    remaining blocks left so it doesn't really matter. Plus this is only a concern 
	when dealing with a huge number of blocks. For our default number of blocks 
	(256) we only waste one block on the FAT. Just make sure that the FAT can 
	fit within all the blocks given for it.
*/

// recursive version
// not great for a large n value
int findCharsRec(int n)
{
    if (n == 1)
    {
        
        return 1;
    }
	
	if (n < 1)
	{
		return 0;
	}
     
    // change number to string
    string s = to_string(n);
    // add length of number to the total sum
    return findCharsRec(n - 1) + s.length();
}

// this finds a very close approximation of the total number 
// of chars (not including spaces) in the FAT but not exact 
// I believe that this caculates more chars than the real amount in the FAT
int findChars(int n)
{
    int sum = 0;
	
    while (n > 0)
    {
        string s = to_string(n);
        sum += s.length();
        n--;
    }
    
    return sum;
}


int main()
{
    // numberofblocks
    int numOfBlocks = 256;
    // sizeofblocks
    int sizeOfBlocks = 128;
	
	string s = to_string(numOfBlocks);
	int charsPerBlock = s.length()+1;
	double multiplier = charsPerBlock; // can change
    
    // near exact total number of chars in FAT (including space)
    int c = findChars(numOfBlocks) + numOfBlocks;
	
    cout << "Chars calculated: " << c << endl;
    
    // fat blocks needed 
    cout << "Blocks needed for near exact: " << c/sizeOfBlocks+1 << endl;
    
    // approximate
    cout << "Our formula approximation result: " << int((multiplier*numOfBlocks)/sizeOfBlocks+1);
    
    return 0;
}