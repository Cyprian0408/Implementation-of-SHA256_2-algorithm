#include "hash.h"
#include <vector>
#include <string>
#include "boost/multi_array.hpp"
#include <bit>
#include <numeric>
#include <algorithm>
#include <bitset>


void _hash::sha(const std::string &data){
    std::string bits;
    //data_bit_string is a string that consists of data converted to bit notation
    std::string data_bit_string;
    //converting data which is string to bits 
    for (int i=0; i<data.size(); i++){
        bits=std::bitset<8>(data[i]).to_string();
        data_bit_string+=bits;
    }
    //size of original string - data - 8 is how many bites serve to save one char
    unsigned int original_size_of_data_string=data.size()*8;
    //std::cout<<data_bit_string<<std::endl;
    //appending a bit=1 at the LSB (least significant bit)
    data_bit_string.resize(data_bit_string.size()+1,'1');
    //std::cout<<data_bit_string<<std::endl;
    unsigned int data_bit_string_size=data_bit_string.size();
    unsigned int counter;
    //calculating how many 512 fit in data_bit_string_size
    if(data_bit_string_size<512){
        counter=1;
    }
    else{
        counter=(data_bit_string_size-(data_bit_string_size%512))/512;
    }
    //std::cout<<"Counter wynosi: "<<counter*512-data_bit_string.size()-64<<std::endl;
    
    //appending counter*512-data_bit_string_64 bits which are set to 0
    data_bit_string.resize(counter*512-64,'0');
    //std::cout<<"Po dodaniu zer"<<std::endl<<data_bit_string.size()<<std::endl;
    //std::cout<<"Początkowy rozmiar data_string: "<<original_size_of_data_string<<std::endl;

    //converting the original size of data string to binary notation where the size is writed on 64 bits 
    std::string binary_ogirinal_data_size=std::bitset<64>(original_size_of_data_string).to_string();
    //std::cout<<binary_ogirinal_data_size<<std::endl;
    
    //appending string with binary representation of data_size to the rest of the string
    data_bit_string.append(binary_ogirinal_data_size);
    //std::cout<<"Po dodaniu ciągu binarnego odpowiadającego rozmiarowi data w bitach"<<std::endl<<data_bit_string<<std::endl;
    std::string string_32_array[64];
    for(int n=0;n<data_bit_string.size()/512;n++){
        //creating the string_32_array which will consist of 32bits chunks of data in form of strings from the data_bit_string
        for(int i=0; i<data_bit_string.size()/32;i++){
            for(int j=0; j<32;j++){
                string_32_array[i]+=data_bit_string[i*32+j];
            }
        }
        std::string zeros_32="00000000000000000000000000000000";
        
        //adding 64-data_bit_string_size 32 long strings that consists only of zeros 
        for(int i=63; i>data_bit_string.size()/32-1;i--){
            string_32_array[i]+=zeros_32;
        }
        //for(int i=0;i<64;i++){
        //    std::cout<<string_32_array[i]<<std::endl;
        //}
        //std::string S0, S1;
        //std::cout<<"After rotation: "<<63-data_bit_string.size()/32<<std::endl;
        for(int i=data_bit_string.size()/32;i<64;i++){
            
            //creating strings that will store strings after rotation
            //std::cout<<"Dla i równego: "<<i<<std::endl<<string_32_array[i-15]<<std::endl;
            std::string str_right_rotated_7=string_32_array[i-15];
            _hash::rightrotate(str_right_rotated_7,7);
            //std::cout<<str_right_rotated_7<<std::endl;
            std::string str_right_rotated_18=string_32_array[i-15];
            _hash::rightrotate(str_right_rotated_18,18);
            std::string str_right_rotated_17=string_32_array[i-2];
            _hash::rightrotate(str_right_rotated_17,17);
            std::string str_right_rotated_19=string_32_array[i-2];
            _hash::rightrotate(str_right_rotated_19,19);
            std::string str_right_shifted_3=string_32_array[i-15];
            _hash::rightshift(str_right_shifted_3,3);
            //std::cout<<"Right shifted: "<<str_right_shifted_3<<std::endl;
            std::string str_right_shifted_10=string_32_array[i-2];
            _hash::rightshift(str_right_shifted_10,10);
            std::string S0=to_string(to_bitset(str_right_rotated_7)^to_bitset(str_right_rotated_18)^to_bitset(str_right_shifted_3));
            std::string S1=to_string(to_bitset(str_right_rotated_17)^to_bitset(str_right_rotated_19)^to_bitset(str_right_shifted_10));
            //std::cout<<"S0 dla i: "<<i<<" jest równe: "<<S0<<std::endl;
            string_32_array[i]=uint_to_string(to_uint32(string_32_array[i-16])+to_uint32(S0)+to_uint32(string_32_array[i-7])+to_uint32(S1));
            //std::cout<<"W[i] dla i: "<<i<<" jest równe: "<<string_32_array[i]<<std::endl;
        }
    //for(int i=0; i<64;i++){
    //    std::cout<<string_32_array[i]<<std::endl;
    //}

    //initializing a,b,c.. values from hash_array
    std::string a=uint_to_string(hash_array[0]);
    //std::cout<<"A to: "<<a<<std::endl;
    std::string b=uint_to_string(hash_array[1]);
    std::string c=uint_to_string(hash_array[2]);
    std::string d=uint_to_string(hash_array[3]);
    std::string e=uint_to_string(hash_array[4]);
    std::string f=uint_to_string(hash_array[5]);
    std::string g=uint_to_string(hash_array[6]);
    std::string h=uint_to_string(hash_array[7]);
    
    //mutating a,b,c.. values in loop 64 times
    for(int i=0; i<64;i++){
        std::string e_rightrrotated_6=e;
        rightrotate(e_rightrrotated_6,6);
        std::string e_rightrrotated_11=e;
        rightrotate(e_rightrrotated_11,11);
        std::string e_rightrrotated_25=e;
        rightrotate(e_rightrrotated_25,25);
        std::string s1=to_string(to_bitset(e_rightrrotated_6)^to_bitset(e_rightrrotated_11)^to_bitset(e_rightrrotated_25));
        //std::cout<<"To jest s1: "<<s1<<std::endl;
        std::string ch=to_string(((to_bitset(e))&(to_bitset(f)))^((~(to_bitset(e)))&(to_bitset(g))));
        //std::cout<<"To jest ch: "<<ch<<std::endl;
        std::string temp1=uint_to_string(to_uint32(h)+to_uint32(s1)+to_uint32(ch)+k256[i]+to_uint32(string_32_array[i]));
        //std::cout<<"Hash constants: "<<k256[i]<<std::endl;
        //std::cout<<"W[i]: "<<string_32_array[i]<<std::endl;
        //std::cout<<"To jest temp1: "<<temp1<<std::endl;
        std::string a_rightrrotated_2=a;
        rightrotate(a_rightrrotated_2,2);
        std::string a_rightrrotated_13=a;
        rightrotate(a_rightrrotated_13,13);
        std::string a_rightrrotated_22=a;
        rightrotate(a_rightrrotated_22,22);
        std::string s0=to_string(to_bitset(a_rightrrotated_2)^to_bitset(a_rightrrotated_13)^to_bitset(a_rightrrotated_22));
        //std::cout<<"To jest s0: "<<s0<<std::endl;
        std::string maj=to_string((to_bitset(a)&=to_bitset(b))^(to_bitset(a)&=to_bitset(c))^(to_bitset(b)&=to_bitset(c)));
        //std::cout<<"To jest maj: "<<maj<<std::endl;
        std::string temp2=uint_to_string(to_uint32(s0)+to_uint32(maj));
        //std::cout<<"To jest temp2: "<<temp2<<std::endl;
        h=g;
        //std::cout<<"To jest h: "<<h<<std::endl;
        g=f;
        //std::cout<<"To jest g: "<<g<<std::endl;
        f=e;
        //std::cout<<"To jest f: "<<f<<std::endl;
        e=uint_to_string(to_uint32(d)+to_uint32(temp1));
        //std::cout<<"To jest e: "<<e<<std::endl;
        d=c;
        //std::cout<<"To jest d: "<<d<<std::endl;
        c=b;
        //std::cout<<"To jest c: "<<c<<std::endl;
        b=a;
        //std::cout<<"To jest b: "<<b<<std::endl;
        a=uint_to_string(to_uint32(temp1)+to_uint32(temp2));
        //std::cout<<"To jest a: "<<a<<std::endl;
    }
    //modyfying final values
    hash_array[0]=hash_array[0]+to_uint32(a);
    hash_array[1]=hash_array[1]+to_uint32(b);
    hash_array[2]=hash_array[2]+to_uint32(c);
    hash_array[3]=hash_array[3]+to_uint32(d);
    hash_array[4]=hash_array[4]+to_uint32(e);
    hash_array[5]=hash_array[5]+to_uint32(f);
    hash_array[6]=hash_array[6]+to_uint32(g);
    hash_array[7]=hash_array[7]+to_uint32(h);    
    }
    std::stringstream hash_hex;
    for(int i=0; i<8;i++){
        hash_hex<<std::hex<<std::uppercase<<hash_array[i];
        //std::cout<<hash_hex.str()<<std::endl;
        hash+=hash_hex.str();
        hash_hex.str(std::string());
    }

    //std::cout<<hash<<std::endl;
}
void _hash::print_hash(){
    std::cout<<_hash::hash<<std::endl;
}
//function to leftrotate a binary string
void _hash::leftrotate(std::string &str, int r){
    std::reverse(str.begin(), str.begin()+r);
    std::reverse(str.begin()+r, str.end());
    std::reverse(str.begin(), str.end());
}
//function to rightrotate a binary string
void _hash::rightrotate(std::string &str, int r){
    leftrotate(str, str.length()-r);
}
//function to rightshift a binary string
void _hash::rightshift(std::string &str, int sh){
    std::bitset<32> bits(str);
    int bit_int=(int)(bits.to_ulong());
    bit_int=bit_int>>sh;
    str.clear();
    std::bitset<32>bs(bit_int);
    str=bs.to_string();
}
//function to convert bitset to string
std::string _hash::to_string(std::bitset<32> bits){
    return bits.to_string();
}
//function to convert string to bitset
std::bitset<32> _hash::to_bitset(std:: string str){
    return std::bitset<32>(str);
}
//function to convert string bits representation to uint32_t
uint32_t _hash::to_uint32(std::string str){
    std::bitset<32>bits(str);
    uint32_t str_to_uint_32=(uint)bits.to_ulong();
    return str_to_uint_32;
}
std::string _hash::uint_to_string(u_int32_t u_int_bits){
    std::bitset<32> bits((ulong)u_int_bits);
    std::string bits_string=bits.to_string();
    return bits_string;
}


