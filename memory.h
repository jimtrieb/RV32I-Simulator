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

#ifndef MEMORY
#define MEMORY
#include "hex.h"
#include <vector>
#include <stdint.h>
#include <string>
class memory : public hex
{
	public:
		/**
		 * constructor to allocate siz bytes in mem vector initialize to 0xa5
		 *
		 * @param siz # of bytes to create in memory
		 *
		 * @note siz rounded up to next multiple of 16
		 *****************************************************************/
		memory(uint32_t siz);


		/**
		 * destructor to deallocate simulated memory
		 *****************************************************************/
		~memory();



		/**
		 * check if given address is legal
		 *
		 * @param addr address in the form of an unsigned 32-bit int
		 *
		 * @return true if addr is out of bounds, false if legal 
		 *
		 * @note formats error message using to_hex0x32 method
		 *****************************************************************/
		bool check_illegal(uint32_t addr) const;


		/**
		 * numbers of bytes (rounded up) of simulated memory
		 *
		 * @return number of bytes (rounded up) of simulated memory
		 *****************************************************************/
		uint32_t get_size() const;


		/**
		 * gets value of byte from memory at given address
		 *
		 * @param addr address used to find value of byte in vector
		 *
		 * @return value of byte at given address, returns 0 if addr not valid
		 *
		 * @note only method that reads from vector
		 * @note calls check_illegal to make sure addr is valid 
		 *****************************************************************/
		uint8_t get8(uint32_t addr) const;


		/**
		 * gets value of 16-bits from memory at given address
		 *
		 * @param addr address used to find value of 16-bits in vector
		 *
		 * @return value of the two bytes in little endian order
		 *
		 * @note calls get8() twice
		 *****************************************************************/
		uint16_t get16(uint32_t addr) const;


		/**
		 * get value of 32-bits from memory at given address
		 *
		 * @param addr address used to find value of 32 bits in vector
		 *
		 * @return value of 4 bytes in little endian order
		 *
		 * @note calls get16() twice
		 *****************************************************************/
		uint32_t get32(uint32_t addr) const;


	        /**
		 * sign extends value returned by get8() into a 32-bit signed int
		 *
		 * @param addr address used in get8() to get 8 bit value from memory
		 *
		 * @return sign extended value in 32-bits of what get8() returns
		 *
		 * @note calls get8()
		 *********************************************************************/
		int32_t get8_sx(uint32_t addr) const;


		/**
		 * sign extends value returned by get16() into a 32-bit signed int
		 *
		 * @param addr address used in get16() to get 16-bit value from memory
		 *
		 * @return sign extended value in 32-bits of what get16() returns
		 *
		 * @note calls get16()
		 *********************************************************************/
		int32_t get16_sx(uint32_t addr) const;


		/**
		 * call get32() to get a signed int
		 *
		 * @param addr address used in get32() to get 32-bit value from memory
		 *
		 * @return returns what get32() returns as a signed int
		 *
		 * @note calls get32()
		 *********************************************************************/
		int32_t get32_sx(uint32_t addr) const;


		/**
		 * sets byte in memory at address to the given 8-bit value
		 *
		 * @param addr address used to find byte in memory
		 * @param val value given to byte at the address passed in
		 *
		 * @note alond with constructor set8() are only code writing to mem vector
		 * @note calls check_illegal to determine validity of address
		 * @note if address not valid discards data and returns to caller
		 **************************************************************************/
		void set8(uint32_t addr, uint8_t val);


		/**
		 * sets 16-bits in memory at address to the given 16-bit value
		 *
		 * @param addr address used to find byte in memory
		 * @param val value given to the 16-bits at address passed in
		 *
		 * @note calls set8() twice
		 * @note little endian
		 ****************************************************************************/
		void set16(uint32_t addr, uint16_t val);


	        /**
		 * set 32-bits in memory at address to the given 32-bit value
		 *
		 * @param addr address used to find byte in memory
		 * @param val value given to the 32-bits at address passed in
		 *
		 * @note little endian
		 * @note calls set16() twice
		 ****************************************************************************/
		void set32(uint32_t addr, uint32_t val);


		/**
		 * dumps entire contents of simulated memory in hex
		 *
		 * @note corresponds to ASCII
		 * @note makes use of isprint to determine if show ASCII or a .
		 *****************************************************************************/
		void dump() const;


		/**
		 * opens file in binary mode and reads contents into simulated memory
		 *
		 * @param fname name of file to be read in. read in binary mode
		 *
		 * @return true on successful load of file, false if file cannot be opened or is to big
		 *
		 * @note calls check_illegal
		 ****************************************************************************/
		bool load_file(const std::string &fname);

	private:
		std::vector<uint8_t> mem; //vector representing simulated memory
};
#endif 
