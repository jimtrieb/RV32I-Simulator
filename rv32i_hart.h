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

#ifndef __HART__
#define __HART__

#include "rv32i_decode.h"
#include "memory.h"
#include "registerfile.h"
#include <cstdint>


class rv32i_hart : public rv32i_decode
{
 	public :
		/**
		 * constructor to initialize mem
		 *
		 * @param m memory
		 ******************************************************************/
		rv32i_hart (memory &m) : mem(m) {}


		/**
		 * a mutator for show_instructions
		 *
		 * @param b used to determine if reg value will be shown
		 *
		 * @note if b = true show each instruction executed dispaying what reg value was used
		 *************************************************************************************/
 		void set_show_instructions(bool b) {show_instructions = b;}


		/**
		 * mutator for show_registers
		 *
		 * @param b flag used to check
		 *
		 * @note if b = true dumps registers before instruction executed
		 *******************************************************************/
 		void set_show_registers(bool b) {show_registers = b;}


		/**
		 * accessor for halt
		 *
		 * @return true if hart has been halted
		 *******************************************************************/
 		bool is_halted() const {return halt;}


		/**
		 * gets the reason hart has halted
		 *
		 * @return a string indicating the reason of halt
		 *******************************************************************/
 		const std::string &get_halt_reason() const {return halt_reason;}


		/**
		 * gets instruction counter, accessor for insn_counter
		 *
		 * @return returns number of executed instructions since last reset
		 *******************************************************************/
 		uint64_t get_insn_counter() const {return insn_counter;}


		/**
		 * mutator for mhartid, set id value retuned by csrrs instruction
		 *
		 * @param i value returned by csrrs instruction
		 *
		 * @note csr reg # 0xf14
		 * @note always zero on processors w single hart
		 *******************************************************************/
 		void set_mhartid(int i) {mhartid = i;}


		/**
		 * how to tell simulator to execute instruction, simulated execution
		 *
		 * @param hdr header that will be printed to left of all output
		 *
		 * @note when hart is halted return immediately
		 * @note increments insn_counter
		 * @note dumps hart state if show_registers = true
		 * @note fetch and execute instruction
		 ***********************************************************************/
 		void tick(const std::string &hdr = "");

		/**
		 * dump entire state of hart
		 *
		 * @param hdr prefix for each dump line
		 ***********************************************************************/
 		void dump(const std::string &hdr = "") const;

		/**
		 * reset rv32i object and the registerfile
		 *
		 * @note sets pc reg, insn_counter to 0 and resets reg values
		 * @note set halt flag to flase and halt_reason to none
		 ***********************************************************************/
 		void reset();

 	private :
 		static constexpr int instruction_width = 35; //width for instructions

		/**
		 * exectute given rv32i instruction
		 *
		 * @param insn instruction that will be executed
		 * @param pos stream to print results
		 *
		 * @note calls exec_illegal_insn if encountes illegal insn
		 * @note makes use of get_xxx funtion to get required fileds
		 * @note invokes exec_xxx to execute instruction
		 **************************************************************/
 		void exec(uint32_t insn, std::ostream* pos);

		/**
		 * handles illegal insructions and prints proper error message
		 *
		 * @param insn instruction used in checking
		 * @param pos stream to print results
		 *
		 * @note set halt flag
		 *****************************************************************/
 		void exec_illegal_insn(uint32_t insn, std::ostream* pos);

		/**
		 * helper function used to simulate lui instruction
		 *
		 * @param insn instruction to be executed
		 * @param pos stream to print results
		 *****************************************************************/
		void exec_lui(uint32_t insn, std::ostream* pos);


		/**
		 * helper function used to simulate auipc instruction
		 *
		 * @param insn instruction to be executed
		 * @param pos stream to print results
		 *****************************************************************/
		void exec_auipc(uint32_t insn, std::ostream* pos);


		/**
		 * helper function used to simulate jal instruction
		 *
		 * @param insn instruction to be executed
		 * @param pos stream to print results
		 *****************************************************************/
		void exec_jal(uint32_t insn, std::ostream* pos);


		/**
		 * helper function used to simulate jalr instruction
		 *
		 * @param insn instruction to be executed
		 * @param pos stream to print results
		 *****************************************************************/
		void exec_jalr(uint32_t insn, std::ostream* pos);


		/**
		 * helper function used to simulate beq instruction
		 *
		 * @param insn instruction to be executed
		 * @param pos stream to print results
		 *****************************************************************/
		void exec_beq(uint32_t insn, std::ostream* pos);


		/**
		 * helper function used to simulate bne instruction
		 *
		 * @param insn instruction to be executed
		 * @param pos stream to print results
		 *****************************************************************/
		void exec_bne(uint32_t insn, std::ostream* pos);


		/**
		 * helper function used to simulate blt instruction
		 *
		 * @param insn instruction to be executed
		 * @param pos stream to print results
		 *****************************************************************/
		void exec_blt(uint32_t insn, std::ostream* pos);


		/**
		 * helper function used to simulate bge instruction
		 *
		 * @param insn instruction to be executed
		 * @param pos stream to print results
		 *****************************************************************/
		void exec_bge(uint32_t insn, std::ostream* pos);


		/**
		 * helper function used to simulate bltu instruction
		 *
		 * @param insn instruction to be executed
		 * @param pos stream to print results
		 *****************************************************************/
		void exec_bltu(uint32_t insn, std::ostream* pos);


		/**
		 * helper function used to simulate bgeu instruction
		 *
		 * @param insn instruction to be executed
		 * @param pos stream to print results
		 *****************************************************************/
		void exec_bgeu(uint32_t insn, std::ostream* pos);


		/**
		 * helper function used to simulate lb instruction
		 *
		 * @param insn instruction to be executed
		 * @param pos stream to print results
		 *****************************************************************/
		void exec_lb(uint32_t insn, std::ostream* pos);


		/**
		 * helper function used to simulate lh instruction
		 *
		 * @param insn instruction to be executed
		 * @param pos stream to print results
		 *****************************************************************/
		void exec_lh(uint32_t insn, std::ostream* pos);


		/**
		 * helper function used to simulate lw instruction
		 *
		 * @param insn instruction to be executed
		 * @param pos stream to print results
		 *****************************************************************/
		void exec_lw(uint32_t insn, std::ostream* pos);


		/**
		 * helper function used to simulate lbu instruction
		 *
		 * @param insn instruction to be executed
		 * @param pos stream to print results
		 *****************************************************************/
		void exec_lbu(uint32_t insn, std::ostream* pos);


		/**
		 * helper function used to simulate lhu instruction
		 *
		 * @param insn instruction to be executed
		 * @param pos stream to print results
		 *****************************************************************/
		void exec_lhu(uint32_t insn, std::ostream* pos);


		/**
		 * helper function used to simulate sb instruction
		 *
		 * @param insn instruction to be executed
		 * @param pos stream to print results
		 *****************************************************************/
		void exec_sb(uint32_t insn, std::ostream* pos);


		/**
		 * helper function used to simulate sh instruction
		 *
		 * @param insn instruction to be executed
		 * @param pos stream to print results
		 *****************************************************************/
		void exec_sh(uint32_t insn, std::ostream* pos);


		/**
		 * helper function used to simulate sw instruction
		 *
		 * @param insn instruction to be executed
		 * @param pos stream to print results
		 *****************************************************************/
		void exec_sw(uint32_t insn, std::ostream* pos);


		/**
		 * helper function used to simulate addi instruction
		 *
		 * @param insn instruction to be executed
		 * @param pos stream to print results
		 *****************************************************************/
		void exec_addi(uint32_t insn, std::ostream* pos);


		/**
		 * helper function used to simulate slti instruction
		 *
		 * @param insn instruction to be executed
		 * @param pos stream to print results
		 *****************************************************************/
		void exec_slti(uint32_t insn, std::ostream* pos);


		/**
		 * helper function used to simulate sltiu instruction
		 *
		 * @param insn instruction to be executed
		 * @param pos stream to print results
		 *****************************************************************/
		void exec_sltiu(uint32_t insn, std::ostream* pos);


		/**
		 * helper function used to simulate xori instruction
		 *
		 * @param insn instruction to be executed
		 * @param pos stream to print results
		 *****************************************************************/
		void exec_xori(uint32_t insn, std::ostream* pos);


		/**
		 * helper function used to simulate ori instruction
		 *
		 * @param insn instruction to be executed
		 * @param pos stream to print results
		 *****************************************************************/
		void exec_ori(uint32_t insn, std::ostream* pos);


		/**
		 * helper function used to simulate andi instruction
		 *
		 * @param insn instruction to be executed
		 * @param pos stream to print results
		 *****************************************************************/
		void exec_andi(uint32_t insn, std::ostream* pos);


		/**
		 * helper function used to simulate slli instruction
		 *
		 * @param insn instruction to be executed
		 * @param pos stream to print results
		 *****************************************************************/
		void exec_slli(uint32_t insn, std::ostream* pos);


		/**
		 * helper function used to simulate srli instruction
		 *
		 * @param insn instruction to be executed
		 * @param pos stream to print results
		 *****************************************************************/
		void exec_srli(uint32_t insn, std::ostream* pos);


		/**
		 * helper function used to simulate srai instruction
		 *
		 * @param insn instruction to be executed
		 * @param pos stream to print results
		 *****************************************************************/
		void exec_srai(uint32_t insn, std::ostream* pos);


		/**
		 * helper function used to simulate add instruction
		 *
		 * @param insn instruction to be executed
		 * @param pos stream to print results
		 *****************************************************************/
		void exec_add(uint32_t insn, std::ostream* pos);


		/**
		 * helper function used to simulate sub instruction
		 *
		 * @param insn instruction to be executed
		 * @param pos stream to print results
		 *****************************************************************/
		void exec_sub(uint32_t insn, std::ostream* pos);


		/**
		 * helper function used to simulate sll instruction
		 *
		 * @param insn instruction to be executed
		 * @param pos stream to print results
		 *****************************************************************/
		void exec_sll(uint32_t insn, std::ostream* pos);


		/**
		 * helper function used to simulate slt instruction
		 *
		 * @param insn instruction to be executed
		 * @param pos stream to print results
		 *****************************************************************/
		void exec_slt(uint32_t insn, std::ostream* pos);


		/**
		 * helper function used to simulate sltu instruction
		 *
		 * @param insn instruction to be executed
		 * @param pos stream to print results
		 *****************************************************************/
		void exec_sltu(uint32_t insn, std::ostream* pos);


		/**
		 * helper function used to simulate xor instruction
		 *
		 * @param insn instruction to be executed
		 * @param pos stream to print results
		 *****************************************************************/
		void exec_xor(uint32_t insn, std::ostream* pos);


		/**
		 * helper function used to simulate srl instruction
		 *
		 * @param insn instruction to be executed
		 * @param pos stream to print results
		 *****************************************************************/
		void exec_srl(uint32_t insn, std::ostream* pos);


		/**
		 * helper function used to simulate sra instruction
		 *
		 * @param insn instruction to be executed
		 * @param pos stream to print results
		 *****************************************************************/
		void exec_sra(uint32_t insn, std::ostream* pos);


		/**
		 * helper function used to simulate or instruction
		 *
		 * @param insn instruction to be executed
		 * @param pos stream to print results
		 *****************************************************************/
		void exec_or(uint32_t insn, std::ostream* pos);


		/**
		 * helper function used to simulate and instruction
		 *
		 * @param insn instruction to be executed
		 * @param pos stream to print results
		 *****************************************************************/
		void exec_and(uint32_t insn, std::ostream* pos);


		/**
		 * helper function used to simulate csrrs instruction
		 *
		 * @param insn instruction to be executed
		 * @param pos stream to print results
		 *****************************************************************/
		void exec_csrrs(uint32_t insn, std::ostream* pos);


		/**
		 * helper function used to simulate ecall instruction
		 *
		 * @param insn instruction to be executed
		 * @param pos stream to print results
		 *****************************************************************/
		void exec_ecall(uint32_t insn, std::ostream* pos);


		/**
		 * helper function used to simulate ebreak instruction
		 *
		 * @param insn instruction to be executed
		 * @param pos stream to print results
		 *****************************************************************/
		void exec_ebreak(uint32_t insn, std::ostream* pos);


 		bool halt = {false}; //flag to stop hart from executing

 		std::string halt_reason = {"none"}; // string containing reason for halt stop

 		uint64_t insn_counter = {0}; //instruction counter

 		uint32_t pc = {0}; //address of instruction being decoded

 		uint32_t mhartid = {0}; // csr register value

		bool show_instructions = {false}; // flag used to determine if simulator should print instructions

		bool show_registers = {false}; //flag used to determine if simulator should print hart state

 	protected :
 		registerfile regs; //general purpose registers

 		memory &mem; //reference to memory object


};
#endif
