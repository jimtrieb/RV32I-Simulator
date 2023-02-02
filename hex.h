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

#ifndef HEX
#define HEX
#include <stdint.h>
#include <string>
class hex
{
	public:
		/**
		 * format 8-bit unsigned int into a 2 character hex string with leading zeros
		 *
		 * @param i 8-bit int that will be converted to hex
		 *
		 * @return a 2 character hex string
		 *******************************************************************************/
		static std::string to_hex8(uint8_t i);



		/**
		 * format 32-bit unsigned int to a string of 8 hex digits
		 *
		 * @param i 32-bit unsigned int will be converted to hex
		 *
		 * @return string of 8 hex digits
		 *******************************************************************************/
		static std::string to_hex32(uint32_t i);



		/**
		 * format 32-bit unsigned int to a string of hex digits beginning with 0x
		 *
		 * @param i 32-bit unsigned int converted to hex
		 *
		 * @return string of the 8 hex digits of i concatenated with 0x
		 *******************************************************************************/
		static std::string to_hex0x32(uint32_t i);


		/**
		 * format a 32-bit unsigned int to a string of hex digits beginning with 0x
		 *
		 * @param i 32-bit insigned int converted to hex
		 *
		 * @return string of 5 hex digits of i concatenated with 0x
		 *******************************************************************************/
		static std::string to_hex0x20(uint32_t i);



		/**
		 * format a 32-bit unsigned int to a string of hex digits beginning with 0x
		 *
		 * @param i 32-bit insigned int converted to hex
		 *
		 * @return string of 3 hex digits of i concatenated with 0x
		 *******************************************************************************/
		static std::string to_hex0x12(uint32_t i);
};
#endif
