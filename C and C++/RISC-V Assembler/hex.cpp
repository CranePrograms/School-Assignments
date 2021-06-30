//***************************************************************************
//
//  hex.cpp
//  CSCI 463 Assignment 4
//
//  Created by Ian Sullivan z1830506
//
//***************************************************************************

#include "hex.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdint.h>
#include <string>

using std::cout;
using std::cin;
using std::string;
using std::ostringstream;
using std::setfill;
using std::setw;

/**
 * outputs a uint8 as a string of hex chars
 *
 * @param i a uint8_t to convert to a string
 *
 * @return a string that represents the uint8 in hex
 *
 ********************************************************************************/
string hex8(uint8_t i) {

    ostringstream os;
    os << std::hex << setfill('0') << setw(2) << static_cast<uint16_t>(i);
    return os.str();

}

/**
 * outputs a uint32 as a string of hex chars
 *
 * @param i a uint32_t to convert to a string
 *
 * @return a string that represents the uint32 in hex
 *
 ********************************************************************************/
string hex32(uint32_t i) {

    ostringstream os;
    os << std::hex << setfill('0') << setw(8) << i;
    return os.str();

}

/**
 * outputs a uint32 as a string of hex chars with a 0x leading it
 *
 * @param i a uint32_t to convert to a string
 *
 * @return a string that represents the uint32 in hex with a 0x leading
 *
 ********************************************************************************/
string hex0x32(uint32_t i) {

    return string("0x") + hex32(i);

}
