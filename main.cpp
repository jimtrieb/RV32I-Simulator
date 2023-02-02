//***************************************************************************
//
//  Jim Trieb
//  z1912658
//  csci463 PE1
//
//  I certify that this is my own work and where appropriate an extension 
//  of the starter code provided for the assignment.
//
//***************************************************************************

#include <iostream>
#include <sstream>
#include <stdint.h>
#include <unistd.h>
#include <iomanip>
#include "memory.h"
#include "rv32i_decode.h"
#include "registerfile.h"
#include "rv32i_hart.h"
#include "cpu_single_hart.h"

using namespace std;

/**
 * usage statement to specify command line options
 *
 * @return none
 *************************************************************************/
static void usage()
{
	cerr << "Usage : rv32i [-d ] [ -i] [-r] [- z] [-l exec - limit ] [-m hex - mem - size ] infile" << endl;
	cerr << "     -d show disassembly before program execution" << endl;
	cerr << "     -i show instruction printing during execution" << endl;
	cerr << "     -l maximum number of instructions to exec" << endl;
	cerr << "     -m specify memory size ( default = 0 x100 )" << endl;
	cerr << "     -r show register printing during execution" << endl;
	cerr << "     -z show a dump of the regs & memory after simulation" << endl;

	exit(1);
}

/**
 * uses address and size of memory to decode each line in file
 *
 * @param mem simulated memory file is stored into
 *
 * @return none
 ******************************************************************************/
static void disassemble(const memory & mem)
{
        for ( uint32_t addr = 0; addr < mem.get_size(); addr+=4)
	{

		cout << hex::to_hex32(addr) << ": ";
		cout << hex::to_hex32(mem.get32(addr)) << "  "; 
		cout << rv32i_decode::decode(addr, mem.get32(addr));
		cout << endl;
	}
}



/**
 * main function to dissasemble rv32i file calls methods from other classes
 *
 * @param argc # of command line arguemnts
 * @param argv array of character pointers binary file
 *
 * @return 0 on successful run
 ********************************************************************************/
int main (int argc, char **argv)
{

	uint32_t memory_limit = 0x100;	// default memory size = 256 bytes

  	int opt;

	bool dissasembleFlag = false; //initialize flags for command line options
	bool instructionFlag = false;
	bool dumpFlag = false;
	int executionLimit = 0;
	bool statusFlag = false;

  	while ((opt = getopt (argc, argv, "dizrl:m:")) != -1)
    	{
      		switch (opt)
		{
			case 'z':
				dumpFlag = true;
				break;
			case 'd':
				dissasembleFlag = true;      //set flags
				break;
			case 'i':
				instructionFlag = true;
				break;
			case 'l':
			{
				std::istringstream iss (optarg);
				iss >> executionLimit;
			}
			break;
			case 'r':
				statusFlag = true;
				break;
			case 'm':
	  		{
	    			std::istringstream iss (optarg);
	    			iss >> std::hex >> memory_limit;
	  		}
	  		break;
			default: /* '?' */
	  		usage ();
		}
    	}

  	if (optind >= argc)
	{
    		usage ();	// missing filename
	}

  	memory mem(memory_limit);

  	if (!mem.load_file (argv[optind]))
	{
    		usage ();
	}

	if (dissasembleFlag)
	{
  		disassemble(mem);
	}

	cpu_single_hart cpu(mem);

	cpu.reset();

	cpu.set_show_instructions(instructionFlag);	//execute insturctions if flag = true	
	
	cpu.set_show_registers(statusFlag);

  	cpu.run(executionLimit);

	if (dumpFlag)
	{
		if (cpu.is_halted())
		{
			cpu.dump("");	
			mem.dump();
		}

	}

  	return 0;
}
