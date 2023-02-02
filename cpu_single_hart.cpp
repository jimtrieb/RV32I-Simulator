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

#include "cpu_single_hart.h"
#include <cstdint>
#include <iostream>
#include <string>


void cpu_single_hart::run(uint64_t exec_limit)
{
	regs.set(2, mem.get_size()); //sets r2 to mem size
	
	if (exec_limit == 0)
	{
		while (!is_halted())
		{
			tick(""); //call tick until is_halted is true
		}
	}

	else
	{
		while (!is_halted() && (get_insn_counter() < exec_limit))
		{
			tick(""); //call tick until is_halted = true or exec_limit reached
		}
	}
	
	if (is_halted())
	{
		cout << "Execution terminated. Reason: " << get_halt_reason() << endl; //print out messages
	}

	cout << get_insn_counter() << " instructions executed" << endl;
}
