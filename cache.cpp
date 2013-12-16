#include <iostream>

using namespace std;

enum cacheResType {MISS=0, HIT=1};

struct DM_Obj 
{
	bool Valid = false; // Default Invalid
	int Tag;
};

// Create the cache of Direct Mapped Objects 
// The size of the cache: 2^14 ( 16 KByte )
// Bytes / Line: 2^3 ( 8 bytes )
// # of lines : ( 2^14 / 2^3 ) = 2^11 lines
bool dm_cache_valid[2048];
int dm_cache_tag[2048];

// Create the cache of Fully Associative
int fa_cache_tag[2048];
bool fa_cache_valid[2048];
// char fa_cache_words[2048][8]; 

int fa_last_tag_idx = 0;
int fa_lru_idx = 0;

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
	// We're supposing the address to be 1MBytes ( 2^20 bits )
	int index = (addr >> 2) & 0x7FF;
	int tag = (addr >> 13) & 0xFF;

	if(dm_cache_valid[index]) {
		if(dm_cache_tag[index] == tag)
			return HIT;
		else {
			dm_cache_tag[index] = tag;

			return MISS;
		}
	} else {
		dm_cache_valid[index] = true;
		dm_cache_tag[index] = tag;

		return MISS;
	}
	// This function accepts the memory address for the read and 
	// returns whether it caused a cache miss or a cache hit

	// The current implementation assumes there is no cache; so, every transaction is a miss
}

// Fully Associative Cache Simulator
cacheResType cacheSimFA(unsigned int addr)
{	
	int byte_offset = addr & 0x7;
	int tag = (addr >> 3) & 0x1FFFF;
	int index = 0;
	// cout << "byte_offset : " << byte_offset << endl << "Tag : " << tag << endl; 
	for (int i = 0; i < 2048; i++)
	{
		if (fa_cache_tag[i] == tag)
		{
			index = i;
			break;
		} else if(i == fa_last_tag_idx) {
			index = ++i;
			break;
		}
	}
	// cout << "Index : " << index << endl;

	if(index > fa_last_tag_idx) { // Tag wasn't found
		if(index >= 2048) { // This should never be greater than 2048 ( but no harm in making sure )
			// Using Random Replacement
			// index = rand() % 2048;

			// Using FIFO
			// index = 0;

			// Using LRU
			index = fa_lru_idx;
		}

		fa_last_tag_idx = index;
		fa_cache_tag[index] = tag;
		fa_cache_valid[index] = true;

		// Here, we should the load bytes of the memory
		
		fa_lru_idx = index;

		return MISS;
	}
	else {
		fa_lru_idx = index;

		if(fa_cache_valid[index]) {
			return HIT;
		} else {
			fa_cache_valid[index] = true;
			// load the 8 bytes of the memory
			return MISS;
		}
	}

	// In order for LRU replacement policy to be successful! 
	
	// This function accepts the memory address for the read and 
	// returns whether it caused a cache miss or a cache hit

	// The current implementation assumes there is no cache; so, every transaction is a miss
}
char *msg[2] = {"Miss","Hit"};

int main()
{
	int inst=0;
	cacheResType r;
	
	int cnt_hit = 0;
	int cnt_miss = 0;

	cout << "Please Choose Address Gen. Func." << endl << "0 : Random" << endl << "1 : Sequential" << endl << "2 : Loop1" << endl << "3 : Loop2" << endl;
	int func;
	cin >> func;

	cout << "Please Choose Cache Type" << endl << "0 : Direct Mapped" << endl << "1 : Fully Associative" << endl;
	int type;
	cin >> type;

	if(type < 0 || type > 1 || func < 0 || func > 3) {
		cout << "Invalid Parameters" << endl;
		return 0;
	}

	unsigned int addr;
	cout << (type == 0 ? "Direct Mapped" : "Fully Associative") << " Cache Simulator\n";

	for(;inst<100000;inst++)
	{
		if(func == 0)
				addr =  memGenRandom();
		else if(func == 1)
				addr = memGenSeq();
		else if(func == 2)
				addr = memGenLoop1();
		else if(func == 3)
				addr = memGenLoop2();

		if(type == 0)
				r = cacheSimDM(addr);
		else if(type == 1)
				r = cacheSimFA(addr);
		
		if(r == MISS) 
			cnt_miss++;
		else 
			cnt_hit++;

		cout << addr <<" ("<< msg[r] <<")\n";
	}
	cout << "type : " << type << " - func : " << func << endl;
	cout << "Hit Count : " << cnt_hit << endl << "Miss Count : " << cnt_miss << endl;
}