//***************************************************************************
//
//  registerfile.cpp
//  CSCI 463 Assignment 5
//
//  Created by Ian Sullivan z1830506
//
//***************************************************************************

#include "registerfile.h"
#include <stdint.h>
#include <iostream>
#include <iomanip>
#include "hex.h"

/**
 * registerfile constructor
 * 
 * constructs a registerfile object using reset().
 * 
 *
 ********************************************************************************/
registerfile::registerfile() {

    reset(); //calls reset to construct

    //std::cout << "end of rf" << std::endl;

}

/**
 * execute reset
 * 
 * resets the hart setting register 0 to 0x0. The other registers to 0xf0f0f0f0.
 * 
 *
 ********************************************************************************/
void registerfile::reset() {

    
    hart[0] = 0x0;
    
    for (int i = 1; i < 32; i++) {

        //std::cout << i << std::endl;
        set(i, 0xf0f0f0f0);

    }
    
    //std::cout << "end of reset" << std::endl;

}

/**
 * execute dump
 * 
 * dumps the registers in a formatted manner using a for loop.
 * 
 *
 ********************************************************************************/
void registerfile::dump() const {

    std::cout << std::right << std::setw(1) << " x0 ";

    for (int i = 0; i < 32; i++) {

        if (i % 8 == 7 && i != 31 && i < 10) {

            std::cout << hex32(hart[i]);
            std::cout << std::endl << std::right << std::setw(2) << "x" << i+1 << std::setw(0) << " ";

        } else if (i % 8 == 7 && i != 31) {

            std::cout << hex32(hart[i]);
            std::cout << std::endl << std::right << std::setw(1) << "x" << i+1 << std::setw(0) << " "; 

        } else {

            std::cout << hex32(hart[i]) << " ";

        }


    }

    std::cout << std::endl;

}

/**
 * get for register file
 * 
 * @param r a uint32_t containing a register to retrieve
 * 
 * @return a uint32_t from the register retrieved
 *
 ********************************************************************************/
int32_t registerfile::get(uint32_t r) const {

    if (r == 0) {

        return 0;

    } else {

        return hart[r];

    }
    
}

/**
 * set for register file
 * 
 * @param r uint32_t that represents a register to set
 * @param val int32_t that represents a val to store at a register
 *
 ********************************************************************************/
void registerfile::set(uint32_t r, int32_t val) {

    //std::cout << "beginning of set" << std::endl;

    if (r > 0 && r < 32) {

        hart[r] = val;

    }

    //std::cout << "end of set" << std::endl;

}
