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

#ifndef __RV32I_DECODE__
#define __RV32I_DECODE__
#include "hex.h"

class rv32i_decode : public hex
{
public:

	/**
	 * decodes each instruction in memory at addr
	 *
	 * @param addr address where insn is stored
	 * @param insn instruction that will be decoded
	 *
	 * @return string of decoded fields printed to std out
	 **********************************************************************/
	static std::string decode(uint32_t addr, uint32_t insn);



        /**
	 * render into a string name of register starting with an x
	 *
	 * @param r given register number
	 *
	 * @return a string starting with x and register number
	 **********************************************************************/
	static std::string render_reg(int r);

protected:

	//all decoder constants
	static constexpr int mnemonic_width             = 8;

	static constexpr uint32_t opcode_lui			= 0b0110111;
	static constexpr uint32_t opcode_auipc			= 0b0010111;
	static constexpr uint32_t opcode_jal			= 0b1101111;
	static constexpr uint32_t opcode_jalr			= 0b1100111;
	static constexpr uint32_t opcode_btype			= 0b1100011;
	static constexpr uint32_t opcode_load_imm		= 0b0000011;
	static constexpr uint32_t opcode_stype			= 0b0100011;
	static constexpr uint32_t opcode_alu_imm		= 0b0010011;
	static constexpr uint32_t opcode_rtype			= 0b0110011;
	static constexpr uint32_t opcode_system			= 0b1110011;

	static constexpr uint32_t funct3_beq			= 0b000;
	static constexpr uint32_t funct3_bne			= 0b001;
	static constexpr uint32_t funct3_blt			= 0b100;
	static constexpr uint32_t funct3_bge			= 0b101;
	static constexpr uint32_t funct3_bltu			= 0b110;
	static constexpr uint32_t funct3_bgeu			= 0b111;

	static constexpr uint32_t funct3_lb				= 0b000;
	static constexpr uint32_t funct3_lh				= 0b001;
	static constexpr uint32_t funct3_lw				= 0b010;
	static constexpr uint32_t funct3_lbu			= 0b100;
	static constexpr uint32_t funct3_lhu			= 0b101;

	static constexpr uint32_t funct3_sb				= 0b000;
	static constexpr uint32_t funct3_sh				= 0b001;
	static constexpr uint32_t funct3_sw				= 0b010;

	static constexpr uint32_t funct3_add			= 0b000;
	static constexpr uint32_t funct3_sll			= 0b001;
	static constexpr uint32_t funct3_slt			= 0b010;
	static constexpr uint32_t funct3_sltu			= 0b011;
	static constexpr uint32_t funct3_xor			= 0b100;
	static constexpr uint32_t funct3_srx			= 0b101;
	static constexpr uint32_t funct3_or				= 0b110;
	static constexpr uint32_t funct3_and			= 0b111;

	static constexpr uint32_t funct7_srl			= 0b0000000;
	static constexpr uint32_t funct7_sra			= 0b0100000;

	static constexpr uint32_t funct7_add			= 0b0000000;
	static constexpr uint32_t funct7_sub			= 0b0100000;

	static constexpr uint32_t insn_ecall			= 0x00000073;
	static constexpr uint32_t insn_ebreak			= 0x00100073;

	static constexpr uint32_t funct3_csrrw			= 0b001;
	static constexpr uint32_t funct3_csrrs			= 0b010;
	static constexpr uint32_t funct3_csrrc			= 0b011;
	static constexpr uint32_t funct3_csrrwi			= 0b101;
	static constexpr uint32_t funct3_csrrsi			= 0b110;
	static constexpr uint32_t funct3_csrrci			= 0b111;
	//end of decoder constants

        /**
	 * extract and return opcode for given instruction
	 *
	 * @param insn instruction used to decode
	 *
	 * @return opcode of given instruction in 32 bits
	 *********************************************************************/
	static uint32_t get_opcode(uint32_t insn);

	/**
	 * extract and return the rd field for given instruction
	 *
	 * @param insn instruction used to decode
	 *
	 * @return rd of given instruction as an int from 0-31
	 **********************************************************************/
	static uint32_t get_rd(uint32_t insn);


	/**
	 * extract and return the funct3 field for given instruction
	 *
	 * @param insn instruction used to decode
	 *
	 * @return funct3 of given instruction as an int from 0-7
	 **********************************************************************/
	static uint32_t get_funct3(uint32_t insn);

	/**
	 * extract and return the rs1 field for given instruction
	 *
	 * @param insn instruction used to decode
	 *
	 * @return rs1 of given instruction as an int from 0-31
	 **********************************************************************/
	static uint32_t get_rs1(uint32_t insn);


	/**
	 * extract and return the rs2 field for given instruction
	 *
	 * @param insn instruction used to decode
	 *
	 * @return rs2 of given instruction as an int from 0-31
	 **********************************************************************/
	static uint32_t get_rs2(uint32_t insn);


	/**
	 * extract and return the funct7 field for given instruction
	 *
	 * @param insn instruction used to decode
	 *
	 * @return funct7 of given instruction as an int from 0x00 to 0x7f
	 **********************************************************************/
	static uint32_t get_funct7(uint32_t insn);


	/**
	 * extract and return the imm_i field for given instruction
	 *
	 * @param insn instruction used to decode
	 *
	 * @return imm_i of given instruction in signed 32 bits int
	 **********************************************************************/
	static int32_t get_imm_i(uint32_t insn);


	/**
	 * extract and return the imm_u field for given instruction
	 *
	 * @param insn instruction used to decode
	 *
	 * @return imm_u of given instruction in signed 32 bits int
	 **********************************************************************/
	static int32_t get_imm_u(uint32_t insn);


	/**
	 * extract and return the imm_b field for given instruction
	 *
	 * @param insn instruction used to decode
	 *
	 * @return imm_b of given instruction in signed 32 bits int
	 **********************************************************************/
	static int32_t get_imm_b(uint32_t insn);


	/**
	 * extract and return the imm_s field for given instruction
	 *
	 * @param insn instruction used to decode
	 *
	 * @return imm_s of given instruction in signed 32 bits int
	 **********************************************************************/
	static int32_t get_imm_s(uint32_t insn);


	/**
	 * extract and return the imm_j field for given instruction
	 *
	 * @param insn instruction used to decode
	 *
	 * @return imm_j of given instruction in signed 32 bits int
	 **********************************************************************/
	static int32_t get_imm_j(uint32_t insn);

	static constexpr uint32_t XLEN = 32; //xlen constant



	/**
	 * determines if given instruction is illegal
	 *
	 * @param insn instruction used to test
	 *
	 * @return void if legal or an error string if illegal 
	 **********************************************************************/
	static std::string render_illegal_insn(uint32_t insn);

        /**
	 * render into a string a lui instruction decode format
	 *
	 * @param insn instruction used to decode
	 *
	 * @return a string of decoded fields for lui instruction 
	 **********************************************************************/
	static std::string render_lui(uint32_t insn);


        /**
	 * render into a string a auipc instruction decode format
	 *
	 * @param insn instruction used to decode
	 *
	 * @return a string of decoded fields for auipc instruction 
	 **********************************************************************/
	static std::string render_auipc(uint32_t insn);

	
        /**
	 * render into a string a jal instruction decode format
	 *
	 * @param addr memory address where insn is stored
	 * @param insn instruction used to decode
	 *
	 * @return a string of decoded fields for jal instruction 
	 **********************************************************************/
	static std::string render_jal(uint32_t addr, uint32_t insn);

        /**
	 * render into a string a jalr instruction decode format
	 *
	 * @param insn instruction used to decode
	 *
	 * @return a string of decoded fields for jalr instruction 
	 **********************************************************************/
	static std::string render_jalr(uint32_t insn);


        /**
	 * render into a string a b-type instruction decode format
	 *
	 * @param addr memory address where insn is stored
	 * @param insn instruction used to decode
	 * @param mnemonic the specific mnemonic for a b-type instruction
	 *
	 * @return a string of decoded fields for b-type instruction 
	 **********************************************************************/
	static std::string render_btype(uint32_t addr, uint32_t insn, const char *mnemonic);

        /**
	 * render into a string an i-type-load instruction decode format
	 *
	 * @param insn instruction used to decode
	 * @param mnemonic the specific mnemonic for a i-type-load instruction
	 *
	 * @return a string of decoded fields for i-type-load instruction 
	 **********************************************************************/
	static std::string render_itype_load(uint32_t insn, const char *mnemonic);

        /**
	 * render into a string an s-type instruction decode format
	 *
	 * @param insn instruction used to decode
	 * @param mnemonic the specific mnemonic for a s-type instruction
	 *
	 * @return a string of decoded fields for s-type instruction 
	 **********************************************************************/
	static std::string render_stype(uint32_t insn, const char *mnemonic);


        /**
	 * render into a string an i-type-alu instruction decode format
	 *
	 * @param insn instruction used to decode
	 * @param mnemonic the specific mnemonic for a i-type-alu instruction
	 * @param imm_i immidiate i value used to decode
	 *
	 * @return a string of decoded fields for i-type-alu instruction 
	 **********************************************************************/
	static std::string render_itype_alu(uint32_t insn, const char *mnemonic, int32_t imm_i);


        /**
	 * render into a string an r-type instruction decode format
	 *
	 * @param insn instruction used to decode
	 * @param mnemonic the specific mnemonic for a r-type instruction
	 *
	 * @return a string of decoded fields for r-type instruction 
	 **********************************************************************/
	static std::string render_rtype(uint32_t insn, const char *mnemonic);


        /**
	 * render into a string an ecall instruction decode format
	 *
	 * @param insn instruction used to decode
	 *
	 * @return a string of decoded fields for ecall-type instruction 
	 **********************************************************************/
	static std::string render_ecall(uint32_t insn);


        /**
	 * render into a string an ebreak instruction decode format
	 *
	 * @param insn instruction used to decode
	 *
	 * @return a string of decoded fields for ebreak-type instruction 
	 **********************************************************************/
	static std::string render_ebreak(uint32_t insn);


        /**
	 * render into a string a csrrx instruction decode format
	 *
	 * @param insn instruction used to decode
	 * @param mnemonic specific mnemonic for given csrrx type instruction
	 *
	 * @return a string of decoded fields for csrrx-type instruction 
	 **********************************************************************/
	static std::string render_csrrx(uint32_t insn, const char *mnemonic);


        /**
	 * render into a string a csrrxi instruction decode format
	 *
	 * @param insn instruction used to decode
	 * @param mnemonic specific mnemonic for given csrrxi type instruction
	 *
	 * @return a string of decoded fields for csrrxi-type instruction 
	 **********************************************************************/
	static std::string render_csrrxi(uint32_t insn, const char *mnemonic);


        /**
	 * render into a string operands of form imm(register) for instructions with such mode
	 *
	 * @param base the given register 
	 * @param disp the immidiate value
	 *
	 * @return a string of operands disp(base)
	 ****************************************************************************************/
	static std::string render_base_disp(uint32_t base, int32_t disp);

	/**
	 * render into a string the mnemonic for an instruction
	 *
	 * @param m the mnemonic passed in used to be formatted 
	 *
	 * @return the mnemonic for each instruction used to print in decoding
	 ***********************************************************************/
	static std::string render_mnemonic(const std::string &m);
};
#endif
