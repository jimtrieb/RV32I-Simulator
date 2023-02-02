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
//
#include <iostream>
#include <fstream>
#include "memory.h"
#include "hex.h"
using namespace std;

memory::memory(uint32_t siz)
{
	siz = (siz+15)&0xfffffff0; // round the length up, mod-16
	mem.resize(siz,0xa5); //initialize vector

}

memory::~memory()
{
}

bool memory::check_illegal(uint32_t addr) const
{
	if (addr < mem.size())
	{
		return false; //addr valid
	}

	else //addr out of range
	{
		cout << "WARNING: Address out of range: " << to_hex0x32(addr);
		cout << endl;
		return true;
	}
}

uint32_t memory::get_size() const 
{
	return mem.size(); //number of bytes in simulated memory

}

uint8_t memory::get8(uint32_t addr) const
{
	if (check_illegal(addr) == true)
	{
		return 0;
	}

	return mem[addr]; //value of byte if addr legal
}

uint16_t memory::get16(uint32_t addr) const
{
	return (get8(addr) + (get8(addr + 1) << 8)); //get8(addr) gets first 8 bits, get8(addr + 1) gets bits 8-15

}

uint32_t memory::get32(uint32_t addr) const
{
	return (get16(addr) + (get16(addr + 2) << 16)); //get16(addr) gets bytes 0 and 1, addr + 2 gets bytes 2 and 3

}

int32_t memory::get8_sx(uint32_t addr) const
{
	uint32_t myVal = get8(addr);
	if (myVal & 0x80) //checks if high order bit is set
	{
		return (static_cast<int32_t>(myVal | 0xffffff00)); //sign extending 1		
	}

	return static_cast<int32_t>(myVal);


}

int32_t memory::get16_sx( uint32_t addr ) const
{
	uint32_t myVal = get16(addr);
	if (myVal & 0x8000) //chekcs high order bit
	{
		return (static_cast<int32_t>(myVal | 0xffff0000)); //sign extend
	}

	return static_cast<int32_t>(myVal);


}

int32_t memory::get32_sx(uint32_t addr) const
{
	return get32(addr);
}

void memory::set8(uint32_t addr, uint8_t val)
{
	if (check_illegal(addr)) 
	{
		return; //check_illegal true
	}

	mem.at(addr) = val; //check_illegal false, sets byte at addr to val


}

void memory::set16(uint32_t addr, uint16_t val)
{
	set8(addr, val & 0xff); //low order byte
	set8(addr + 1, (val & 0xff00) >> 8); //high order byte
	
}

void memory::set32(uint32_t addr, uint32_t val)
{
	set16(addr, val & 0xffff); //lower 16 bits
	set16(addr + 2, (val & 0xffff0000) >> 16); //upper 16 bits
}

void memory::dump() const
{
	for (uint32_t i = 0; i < mem.size(); i+=16) //loop through whole vector
	{
		cout << to_hex32(i) << ": "
	                       	<< to_hex8(get8(i)) //each time we add to i it gives the next 2 hex digits for the dump printout
				<< " "
	                       	<< to_hex8(get8(i+1))
				<< " "
	                       	<< to_hex8(get8(i+2))
				<< " "
	                       	<< to_hex8(get8(i+3))
				<< " "
	                       	<< to_hex8(get8(i+4))
				<< " "
	                       	<< to_hex8(get8(i+5))
				<< " "
	                       	<< to_hex8(get8(i+6))
				<< " "
	                       	<< to_hex8(get8(i+7))
				<< "  "
	                       	<< to_hex8(get8(i+8))
				<< " "
	                       	<< to_hex8(get8(i+9))
				<< " "
	                       	<< to_hex8(get8(i+10))
				<< " "
	                       	<< to_hex8(get8(i+11))
				<< " "
	                       	<< to_hex8(get8(i+12))
				<< " "
	                       	<< to_hex8(get8(i+13))
				<< " "
	                       	<< to_hex8(get8(i+14))
				<< " "
	                       	<< to_hex8(get8(i+15))
				<< " "
				<< "*";
		for (int j = 0; j < 16; j++)
		{
			uint8_t ch = get8(i + j);
			ch = isprint(ch) ? ch : '.'; //determine if character is printable
			cout << ch;


		}

		cout << "*" << endl;			
	}

	
}

bool memory::load_file(const std::string &fname)
{
	std::ifstream infile(fname, std::ios::in|std::ios::binary);

	if (infile.fail()) //file can not open
	{
		cerr << "Can't open file '" << fname << "' for reading." << endl;
		return false;
	}
	
	uint8_t i;
	infile >> std::noskipws; //reads file contents
	for (uint32_t addr = 0; infile >> i; ++addr)
	{

		if (check_illegal(addr)) //addrrss is illegal
		{
			cerr << "Program too big." << endl;
			infile.close();
			return false;
		}

		set8(addr, i);
	}
	infile.close();
	return true;
}
