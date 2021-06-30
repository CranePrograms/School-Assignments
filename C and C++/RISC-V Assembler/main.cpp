//***************************************************************************
//
//  main.cpp
//  CSCI 463 Assignment 4
//
//  Created by Ian Sullivan z1830506
//
//***************************************************************************

#include <iostream>
#include "memory.h"
#include "hex.h"
#include "rv32i.h"
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

using std::cerr;
using std::endl;
using std::cout;

/**
* Print a usage message and abort the program.
// NEED TO UPDATE THIS LATER
*********************************************************************/
static void usage()
{
	cerr << "Usage: rv32i [-m hex-mem-size] infile" << endl;
	cerr << "    -m specify memory size (default = 0x10000)" << endl; 
	exit(1);
}

/**
* Read a file of RV32I instructions and execute them.
********************************************************************/
int main(int argc, char **argv)
{
	uint32_t memory_limit = 0x10000;		// default memory size = 64k
	uint64_t insn_limit = 0x99999999;   //essentially limitless
	int opt;
	bool showdiss = false;
	bool showdump = false;
	bool showinstr = false;
	bool showhart = false;

	while ((opt = getopt(argc, argv, "dil:m:rz")) != -1) 
	{
		switch (opt) 
		{
		case 'd':
			//cout << "CASE d USED" << endl;
			showdiss = true;
			break;
		case 'i':
			//cout << "CASE i USED" << endl;
			showinstr = true;
			break;
		case 'l':
			//cout << "CASE l USED" << endl;
			insn_limit = std::stoul(optarg,nullptr,16); // I think this is correct?
			cout << insn_limit << " this is the lim" << endl;
			break;
		case 'm':
			memory_limit = std::stoul(optarg,nullptr,16);
			break;
		case 'r':
			//cout << "CASE r USED" << endl;
			showhart = true;
			break;
		case 'z':
			//cout << "CASE z USED" << endl;
			showdump = true;
			break;
		default: /* '?' */
			usage();
		}
	}

	if (optind >= argc) 
		usage();	// missing filename

	memory mem(memory_limit);

	if (!mem.load_file(argv[optind]))
		usage();

	rv32i sim(&mem);

	if (showinstr == true) {
		sim.set_show_insturctions(true);
	}

	if (showhart == true) {
		sim.set_show_registers(true);
	}

	if (showdiss == true) {
		sim.disasm();
		sim.reset();
	}

	sim.run(insn_limit);

	if (showdump == true) {
		sim.dump();
		mem.dump();
	}

	return 0;
}