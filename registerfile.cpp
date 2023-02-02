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

#include <cstdint>
#include <string>
#include <iostream>
#include <vector>
#include <iomanip>
#include "registerfile.h"
#include "rv32i_decode.h"
using namespace std;

registerfile::registerfile():myvector(32) 
{ 
	reset(); //constructor calls reset 
}


void registerfile::reset()
{
	myvector[0] = 0; //reg 0

	for (long unsigned int i =1; i < myvector.size(); i++)
	{
		myvector[i] = 0xf0f0f0f0; //all other regs
	}
}


void registerfile::set(uint32_t r, int32_t val)
{
	if ( r > 0 && r < 32)
	{
		myvector[r] = val; //assigns reg r to val
	}

}

int32_t registerfile::get(uint32_t r) const
{
	if (r > 0 && r < 32)
	{
		return myvector[r];
	}
	
	else 
	{
		return 0; //reg 0
	}


}
void registerfile::dump(const std::string &hdr) const
{
	printDumpLine(0, hdr);
	printDumpLine(8, hdr);
	printDumpLine(16, hdr);
	printDumpLine(24, hdr);

}

void registerfile::printDumpLine(uint32_t startReg, const std::string &hdr) const
{
      
	cout << hdr; //prints header

	if (startReg < 10)
	{
		cout << " ";
	}

	cout << rv32i_decode::render_reg(startReg) << " ";

	for (uint32_t i = startReg; i < startReg + 8; i++) //loop to print each reg in line
	{


		if (i == startReg + 4)
		{
			cout << " ";
		}

		cout << to_hex32(get(i));

		if (i != startReg + 7)
		{
			cout << " ";
		}
	}
	
	cout << endl;
}
