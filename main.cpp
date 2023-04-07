#include "hash.h"

int main(void){
    const std::string data ="Hello world";
    _hash hash;
    hash.sha(data);
    hash.print_hash();
    //hash.print_hash();
    return 0;
}