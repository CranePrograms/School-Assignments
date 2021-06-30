//***************************************************************************
//
//  registerfile.h
//  CSCI 463 Assignment 5
//
//  Created by Ian Sullivan z1830506
//
//***************************************************************************

#ifndef registerfile_H
#define registerfile_H

#include <iostream>
#include <stdint.h>

class registerfile {

    private:

        int32_t hart[32];

    public:

        registerfile(); //constructor
        void reset();
        void set(uint32_t r, int32_t val);
        int32_t get(uint32_t r) const;
        void dump() const;



};

#endif 