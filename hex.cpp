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

#include "hex.h"
#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;

std::string hex::to_hex8(uint8_t i)
{
	std::ostringstream os;
	os << std::hex << std::setfill('0') << std::setw(2) << static_cast<uint16_t>(i); //formats i into 8-bit hex string
	return os.str();

}
std::string hex::to_hex32(uint32_t i)
{
	std::ostringstream os;
	os << std::hex << std::setfill('0') << std::setw(8) << i; //formats i into 32-bit hex string
	return os.str();

}
std::string hex::to_hex0x32(uint32_t i)
{
	return std::string("0x")+to_hex32(i); //uses 32-bit hex string and concatenates i with 0x

}

std::string hex::to_hex0x20(uint32_t i)
{
	std::ostringstream os;
	os << std::hex << std::setfill('0') << std::setw(5) << (i & 0xfffff); //mask to make sure string is 5 digits
	return (std::string("0x")+os.str());
 

}

std::string hex::to_hex0x12(uint32_t i)
{
	std::ostringstream os;
	os << std::hex << std::setfill('0') << std::setw(3) << (i & 0xfff); //mask to make sure string is 3 digits
	return (std::string("0x")+os.str());
}
