#include <cstdlib>
#include <iostream>
#include "hash.h"
class Block{
    public:
        std::string previous_block_hash; // each block must have a hash of previous block so we can check the correctness
        Block(int index, const std::string &data); // constructor of a block 


    private:
        int index;
        std::string data; //data which will be encrypted using hash function 
        std::string hash; //data which is encrypted using -- algorithm creates hashes which are string that are always the same length and are unique 
        std::string Create_hash(); 
};