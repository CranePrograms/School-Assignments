//***************************************************************************
//
//  hex.h
//  CSCI 463 Assignment 4
//
//  Created by Ian Sullivan z1830506
//
//***************************************************************************

#ifndef hex_H
#define hex_H

#include <string>
#include <stdint.h>

//Declarations for hex.cpp
std::string hex8(uint8_t i);
std::string hex32(uint32_t i);
std::string hex0x32(uint32_t i);

#endif