//***************************************************************************
//
//  memory.cpp
//  CSCI 463 Assignment 4
//
//  Created by Ian Sullivan z1830506
//
//***************************************************************************

#include <iostream>
#include <iomanip>
#include "memory.h"
#include "hex.h"
#include <fstream>

using std::string;
using std::cin;
using std::cout;
using std::endl;

/**
 * Constructor for the memory class
 * 
 * This constructor rounds up to 16 bits if a non increment of 16 is given.
 * It then decalres a dynamic array and fills it with 0xa5
 *
 * @param siz a uint32_t which represents the size of memory to be simulated
 *
 *
 ********************************************************************************/
memory::memory(uint32_t siz) {

    //round siz up, mod -16
    siz = (siz+15)&0xfffffff0;

    //save the siz val into the size member.
    size = siz;

    //Dynamic array allocated
    mem = new uint8_t[siz];

    //Fill the array with 0xa5
    for (uint32_t i = 0; i < get_size() ; i++) {

        set8(i, 0xa5);
        
    }

}

/**
 * Deconstructor for the memory class
 *
 * Just frees all the memory taken by a memory object
 *
 ********************************************************************************/
memory::~memory() {

//Free the dynamic memory array
delete[] mem;

}

/**
 * Checks if the address given is legal
 * 
 * Compares the given address against the size and if the given value is smaller
 * then the address is legal.
 *
 * @param i a uint32_t to be checked as an address
 *
 * @return true if the address given is legal and false if the address is out of range.
 *
 ********************************************************************************/
bool memory::check_address(uint32_t i) const {

    //if the addr is less than the maximum size then it's valid
    if (i <= get_size()) {
        
        return true;

    } else {

        //print a warrning
        cout << "WARNING: Address out of range: " << hex0x32(i) << endl;
        return false;

    }

}

/**
 * a get function for the size member
 *
 * @return uint32_t representing the size member of the object.
 *
 ********************************************************************************/
uint32_t memory::get_size() const {

    return size;

}

/**
 * reads a byte of info from a given address
 * 
 * checks if the memory is in range then reads a byte form a given address
 *
 * @param addr a uint32_t that represents an address to read from
 *
 * @return either returns a byte from memory as a uint8_t or 0 if the byte doesn't exist
 *
 ********************************************************************************/
uint8_t memory::get8(uint32_t addr) const {

    //check validity of addr
    if(check_address(addr)) {

        return mem[addr];

    } else {

        return 0;
    }

}

/**
 * reads a uint16_t from memory
 * 
 * It just reads the bytes in little endian order then stores them into a uint16 using
 * a condensed line of code I referenced from stackoverflow
 *
 * @param addr a uint32_t representing an address to read from
 *
 * @return a uint16_t representing two bytes from memory in little endian order
 *
 ********************************************************************************/
uint16_t memory::get16(uint32_t addr) const {

    uint8_t temp1;
    uint8_t temp2; //stores u8's
    uint16_t temp16; //stores u16

    temp1 = get8(addr);
    temp2 = get8(addr + 1); //add 8 bits to the addr location for the next byte.

    //https://stackoverflow.com/questions/15249791/combining-two-uint8-t-as-uint16-t

    temp16 = (temp2 << 8) | temp1;

    return temp16;


}

/**
 * reads a uint32 from memory
 * 
 * uses essentially the same technique as above to read in little endian order then
 * stores that result as a uint32_t
 *
 * @param addr a uint32_t representing an address to read from
 *
 * @return a uint32_t of bytes in little endian order
 *
 ********************************************************************************/
uint32_t memory::get32(uint32_t addr) const {

    uint16_t temp1;
    uint16_t temp2; //stores the two u16's
    uint32_t temp32; //Val to be returned as a u32

    temp1 = get16(addr);
    temp2 = get16(addr + 2); //add 16 bits to the addr location for the next get

    temp32 = (temp2 << 16) | temp1;

    return temp32;

}

/**
 * sets a byte of memory
 * 
 * checks if the given address exists then stores the given val into that byte
 *
 * @param addr a uint32_t representing an address to store the byte at
 * @param val a uint8_t representing the byte of info to store
 *
 ********************************************************************************/
void memory::set8(uint32_t addr, uint8_t val) {

    //if the address is in range.
    if(check_address(addr)) {

        //set the value to val
        mem[addr] = val;

    }
    //otherwise do nothing and return

}

/**
 * sets 2 bytes of memory
 * 
 * calls set8 in the correct order to store the given bytes in little endian order
 *
 * @param addr a uint32_t representing an address to store the bytes at
 * @param val a uint16_t representing the bytes of info to store
 *
 ********************************************************************************/
void memory::set16(uint32_t addr, uint16_t val) {

    uint8_t temp1;
    uint8_t temp2;
    
    temp1 = (val & 0x00ff);
    temp2 = ((val >> 8) & 0x00ff); //Shift then convert for the higher byte

    set8(addr, temp1); //Little-endian order
    set8(addr + 1, temp2);

}

/**
 * sets 4 bytes of memory
 * 
 * calls set16 in the correct order to store the given bytes in little endian order
 *
 * @param addr a uint32_t representing an address to store the bytes at
 * @param val a uint32_t representing the bytes of info to store
 *
 ********************************************************************************/
void memory::set32(uint32_t addr, uint32_t val) {

    uint16_t temp1;
    uint16_t temp2;

    temp1 = (val & 0x0000ffff);
    temp2 = ((val >> 16) & 0x0000ffff);

    set16(addr, temp1);
    set16(addr + 2, temp2); //call set 16 2 bytes in


}

/**
 * dumps the simulated memory
 * 
 * A very complicated function that essentially checks the size of the dynamic array that simulates memory
 * It then uses multiple if statements that check how long the loops been running and formats the output respectivly
 * Within it contains an array of chars that represent the ascii values of the simulated memory. dump checks that those
 * values are printable then prints them if they are or a * if they aren't
 *
 *
 ********************************************************************************/
void memory::dump() const {
    
    for (uint32_t i = 0; i < get_size(); i++) {

        char ascii[16]; //ascii printing array
        uint8_t ch; //stores current byte

        ch = get8(i);

        if (i % 16 == 0 && i != 0) {

            //now to print ascii
            cout << "*";
            for (int x = 0; x < 16; x++) {

                cout << ascii[x];

            }
            cout << "*" << endl;

        }

        ascii[i%16] = std::isprint(ch) ? ch : '.'; //store either a printable char or .
        
        //at the very start or every 16 loops print this
        if (i == 0 || (i % 16 == 0)) {

            cout << std::hex << std::setfill('0') << std::setw(8) << i << ":"; //this should print the hex value of our memory location
            cout << std::setfill(' ') << std::setw(0); //Change things back to normal

        }

        cout << " " << hex8(ch); //This might have to be geti ??? this is mostly logic rn anyway
       

        //every 8 chars cout an extra space
        if ((i + 1) % 8 == 0 && i != 0) {

            cout << " "; 

        }

        if (i == (get_size() - 1)) {

            //now to print ascii
            cout << "*";
            for (int x = 0; x < 16; x++) {

                cout << ascii[x];

            }
            cout << "*" << endl;

        }


    }

    cout << std::dec;
    
}

/**
 * loads a file and reads it
 * 
 * It opens a file in binaray and then reads it byte by byte and checks the
 * validity of the memory before reading.
 *
 * @return It returns a bool. It can fail if the file can't be opened or the file
 * is too large for the amount of bytes dedicated. It succedes if it sucessfully
 * reads the whole file into memory.
 *
 ********************************************************************************/
bool memory::load_file(const std::string &fname) {

    std::ifstream infile(fname, std::ios::in|std::ios::binary);
    uint32_t tempaddr = 0x0000; //keep track of the current location in memory

    if (infile) {

        uint8_t i;
        infile >> std::noskipws; //don't ruin my input please
        while (infile >> i) {

            

            if(check_address(tempaddr)) {

                set8(tempaddr, i);
                
            } else {

                //error
                std::cerr << "Program too big." << endl;
                infile.close(); //clsoe the file
                return false;
            }

            tempaddr++; //use next location
            

        }



    } else {

        std::cerr << "Can't open file '" << fname << "' for reading." << endl;
        return false;

    }

    return true;

}