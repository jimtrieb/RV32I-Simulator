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
#ifndef __REG__
#define __REG__

#include <vector>
#include <cstdint>
#include <string>
#include "hex.h"
using namespace std;

class registerfile : public hex
{
	public:
		/**
		 * constructor calls reset to initialize values
		 *****************************************************************/
		registerfile ();

		/**
		 * reset will initialize all registers
		 *
		 * @note sets x0 to 0 and all others regs to 0xf0f0f0f0
		 *****************************************************************/
		void reset();

		/**
		 * set the values of a register
		 *
		 * @param r register to be set
		 * @param val value used to set
		 *
		 * @note does nothing if r = 0
		 ****************************************************************/
		void set(uint32_t r, int32_t val);

		/**
		 * return values of register
		 *
		 * @param r register used, will return r's value
		 *
		 * @return signed 32 bit int value of register r
		 *
		 * @note returns 0 if r is 0
		 ***************************************************************/
		int32_t get(uint32_t r) const;

		/**
		 * a dump of registers
		 *
		 * @param hdr a string printed out before each line of dump
		 ***************************************************************/
		void dump(const std::string &hdr) const;

	private:
		/**
		 * helper function used to print line of dump
		 *
		 * @param startReg starting register to print from
		 * @param hdr string printed out before each dump line
		 *******************************************************************/
		void printDumpLine(uint32_t startReg, const std::string &hdr) const;


		vector<int32_t> myvector; //vector of registers
};

#endif
