#include <iostream>

using namespace std;

enum cacheResType {MISS=0, HIT=1};

unsigned int memGenSeq()
{
	static unsigned int addr=0;
	return (addr++)%(1024*1024);
}

unsigned int memGenRandom()
{
	return rand()%(1024*1024);
}

unsigned int memGenLoop1()
{
	static unsigned int addr=0;
	return (addr++)%(1024*4);
}

unsigned int memGenLoop2()
{
	static unsigned int addr=0;
	return (addr++)%(1024*24);
}

// Direct Mapped Cache Simulator
cacheResType cacheSimDM(unsigned int addr)
{	
	// This function accepts the memory address for the read and 
	// returns whether it caused a cache miss or a cache hit

	// The current implementation assumes there is no cache; so, every transaction is a miss
	return MISS;
}

// Fully Associative Cache Simulator
cacheResType cacheSimFA(unsigned int addr)
{	
	// This function accepts the memory address for the read and 
	// returns whether it caused a cache miss or a cache hit

	// The current implementation assumes there is no cache; so, every transaction is a miss
	return MISS;
}
char *msg[2] = {"Miss","Hit"};

int main()
{
	int inst=0;
	cacheResType r;
	
	unsigned int addr;
	cout << "Direct Mapped Cache Simulator\n";
	for(;inst<100;inst++)
	{
		addr = memGenRandom();
		r = cacheSimDM(addr);
		cout << addr <<" ("<< msg[r] <<")\n";
	}
}