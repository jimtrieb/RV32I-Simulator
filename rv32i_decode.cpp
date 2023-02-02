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

#include <sstream>
#include <string>
#include <iostream>
#include <cstdint>
#include <iomanip>
#include "rv32i_decode.h"
using namespace std;

std::string rv32i_decode::decode(uint32_t addr, uint32_t insn)
{
	switch (get_opcode(insn))
	{
		default: return render_illegal_insn(insn);

		case opcode_lui: return render_lui(insn);

		case opcode_auipc: return render_auipc(insn);

		case opcode_jal: return render_jal(addr, insn);

		case opcode_jalr: return render_jalr(insn);

		case opcode_btype:

			switch (get_funct3(insn)) //will switch on funct3 when b-types have same op code
			{
				default: return render_illegal_insn(insn);
				case funct3_beq: return render_btype(addr, insn, "beq");
				case funct3_bne: return render_btype(addr, insn, "bne");
				case funct3_blt: return render_btype(addr, insn, "blt");
				case funct3_bge: return render_btype(addr, insn, "bge");
				case funct3_bltu: return render_btype(addr, insn, "bltu");
				case funct3_bgeu: return render_btype(addr, insn, "bgeu");
			}
				
		case opcode_load_imm:

			switch (get_funct3(insn)) //will switch on funct3 when i-type-loads have same op code
			{
				default: return render_illegal_insn(insn);
				case funct3_lb: return render_itype_load(insn, "lb");
				case funct3_lh: return render_itype_load(insn, "lh");
				case funct3_lw: return render_itype_load(insn, "lw");
				case funct3_lbu: return render_itype_load(insn, "lbu");
				case funct3_lhu: return render_itype_load(insn, "lhu");
			}

		case opcode_stype:
			
			switch (get_funct3(insn)) //will switch on funct3 when s-stypes have same op code
			{
				default: return render_illegal_insn(insn);
				case funct3_sb: return render_stype(insn, "sb");
				case funct3_sh: return render_stype(insn, "sh");
				case funct3_sw: return render_stype(insn, "sw");
			}

		case opcode_alu_imm:

			switch (get_funct3(insn)) //switch on funct3 when i-type-alus have same op code
			{
				default: return render_illegal_insn(insn);
				case funct3_add: return render_itype_alu(insn, "addi", get_imm_i(insn));
				case funct3_sll: return render_itype_alu(insn, "slli", get_imm_i(insn));
				case funct3_slt: return render_itype_alu(insn, "slti", get_imm_i(insn));
				case funct3_sltu: return render_itype_alu(insn, "sltiu", get_imm_i(insn));
				case funct3_xor: return render_itype_alu(insn, "xori",get_imm_i(insn));
				case funct3_or: return render_itype_alu(insn, "ori", get_imm_i(insn));
				case funct3_and: return render_itype_alu(insn, "andi", get_imm_i(insn));
				case funct3_srx:
						  switch (get_funct7(insn)) //must switch on funct7 to differentiate between srli and srai instructions
						  {
							  default: return render_illegal_insn(insn);
							  case funct7_srl: return render_itype_alu(insn, "srli", get_imm_i(insn));
							  case funct7_sra: return render_itype_alu(insn, "srai", get_imm_i(insn) & 0x01f);
						  }
			}

		case opcode_rtype:

			switch (get_funct3(insn))
			{
				default: return render_illegal_insn(insn);
				case funct3_add: 
						 switch (get_funct7(insn)) //r-type add instructions swtich on funct7
						 {
							default: return render_illegal_insn(insn);
							case funct7_add: return render_rtype(insn, "add");
							case funct7_sub: return render_rtype(insn, "sub");
						 } 
				case funct3_sll: return render_rtype(insn, "sll");
				case funct3_slt: return render_rtype(insn, "slt"); //switch on funct3
				case funct3_sltu: return render_rtype(insn, "sltu");
				case funct3_xor: return render_rtype(insn, "xor");
				case funct3_srx:
						 switch (get_funct7(insn))
						 {
							default: return render_illegal_insn(insn);
							case funct7_srl: return render_rtype(insn, "srl"); //r-type srx instructions switch on funct7
							case funct7_sra: return render_rtype(insn, "sra");
						 } 
				case funct3_or: return render_rtype(insn, "or");  //switch on funct 3
				case funct3_and: return render_rtype(insn, "and");
			}

		case opcode_system:
				if (insn == insn_ecall)
				{
					return render_ecall(insn); //specific cases no need to pass mnemonic for ecall and ebreak
				}
				
				else if(insn == insn_ebreak)
				{
					return render_ebreak(insn);
				}
			
				else
				{
		        		switch (get_funct3(insn))
					{
						default: return render_illegal_insn(insn);
						case funct3_csrrw: return render_csrrx(insn, "csrrw");
						case funct3_csrrs: return render_csrrx(insn, "csrrs");
						case funct3_csrrc: return render_csrrx(insn, "csrrc");
						case funct3_csrrwi: return render_csrrxi(insn, "csrrwi"); //switch on funct3 for csrrx instructions
						case funct3_csrrsi: return render_csrrxi(insn, "csrrsi");
						case funct3_csrrci: return render_csrrxi(insn, "csrrci");
					}
				}
	}

}

//each get_ function masks of bits needed according to rvalp
uint32_t rv32i_decode::get_opcode(uint32_t insn)
{
	return (insn & 0x0000007f);

}
	
uint32_t rv32i_decode::get_rd(uint32_t insn)
{
	return ((insn & 0x00000f80) >> 7);

}
	
uint32_t rv32i_decode::get_funct3(uint32_t insn)
{
	return ((insn & 0x0007000) >> 12);

}
	
uint32_t rv32i_decode::get_rs1(uint32_t insn)
{
	return ((insn & 0x000f8000) >> 15);

}
	
uint32_t rv32i_decode::get_rs2(uint32_t insn)
{
	return ((insn & 0x01f00000) >> 20);

}
	
uint32_t rv32i_decode::get_funct7(uint32_t insn)
{
	return ((insn & 0xfe000000) >> 25);

}
	
int32_t rv32i_decode::get_imm_i(uint32_t insn)
{
	int32_t imm_i = (insn & 0xfff00000) >> 20;

	if (insn & 0x80000000) //check high order bit
	{
		imm_i |= 0xfffff000; //sign extend
	}

	return imm_i;
}
	
int32_t rv32i_decode::get_imm_u(uint32_t insn)
{
	return ((insn & 0xfffff000) >> 12) & 0xfffff;
}
	
int32_t rv32i_decode::get_imm_b(uint32_t insn)
{
	int32_t imm_b = 
			((insn & 0x80) << (11-7))                 // bit 11
	              | ((insn & 0xF00) >> (8-1))                 // bits 1-4
                      | ((insn & 0x7E000000) >> (30-10))          // bits 5-10
	              | ((insn & 0x80000000) >> (31-12))          // bit 12
		      ;

	if ((insn & 0x80000000))
	{
		imm_b |= 0xfffff000; //sign extend
	}

	return imm_b;
}
	
int32_t rv32i_decode::get_imm_s(uint32_t insn)
{
	int32_t imm_s = ((insn & 0x00000f80) >> 7) | ((insn &0x7e000000) >> (25-5)); //bits 0-10
	
	if ((insn & 0x80000000))
	{
		imm_s |= 0xfffff800; //sign extend
	}

	return imm_s;

}
	
int32_t rv32i_decode::get_imm_j(uint32_t insn)
{
	int32_t imm_j = (insn & 0x000ff000) //bits 12-19
		     |  (insn & 0x7fe00000) >> (30-10) //bits 1-10 
		     |  (insn & 0x00100000) >> (20-11) //bit 11
		     ;
	
	if ((insn & 0x80000000))
	{
		imm_j |= 0xfff00000; //sign extend
	}

	return imm_j;
}

std::string rv32i_decode::render_illegal_insn(uint32_t insn)
{
	(void)insn; //void if legal
	return "ERROR: UNIMPLEMENTED INSTRUCTION";
}


//each render function extracts the given fields using helper functions and renders them in a string
std::string rv32i_decode::render_lui(uint32_t insn)
{
	uint32_t rd = get_rd(insn);
	int32_t imm_u = get_imm_u(insn);
	
	std::ostringstream os;
	os << render_mnemonic("lui") << render_reg(rd) << ","
		<< hex::to_hex0x20(imm_u);
	return os.str();
}
std::string rv32i_decode::render_auipc(uint32_t insn)
{
	uint32_t rd = get_rd(insn);
	int32_t imm_u = get_imm_u(insn);

	
	std::ostringstream os;
	os << render_mnemonic("auipc") << render_reg(rd) << ","
		<< hex::to_hex0x20(imm_u);
	return os.str();
}

std::string rv32i_decode::render_jal(uint32_t addr, uint32_t insn)
{
	uint32_t rd = get_rd(insn);
	int32_t imm_j = get_imm_j(insn);

	std::ostringstream os;
	os << render_mnemonic("jal") << render_reg(rd) << ","
		<< hex::to_hex0x32(imm_j + addr);
	return os.str();

}

std::string rv32i_decode::render_jalr(uint32_t insn)
{

	uint32_t rd = get_rd(insn);
	int32_t imm_i = get_imm_i(insn);
	uint32_t rs1 = get_rs1(insn);

	std::ostringstream os;
	os << render_mnemonic("jalr") << render_reg(rd) << ","
	   << render_base_disp(rs1, imm_i);
	return os.str();

}

std::string rv32i_decode::render_btype(uint32_t addr, uint32_t insn, const char *mnemonic)
{


	uint32_t rs1 = get_rs1(insn);
	uint32_t rs2 = get_rs2(insn);
	int32_t imm_b = get_imm_b(insn);

	std::ostringstream os;
	os << render_mnemonic(mnemonic) << render_reg(rs1) << ","
		<< render_reg(rs2) << "," << hex::to_hex0x32(imm_b + addr);
	return os.str();

}

std::string rv32i_decode::render_itype_load(uint32_t insn, const char *mnemonic)
{

	uint32_t rs1 = get_rs1(insn);
	uint32_t rd = get_rd(insn);
	int32_t imm_i = get_imm_i(insn);


	std::ostringstream os;
	os << render_mnemonic(mnemonic) 
		<< render_reg(rd) << ","
		<< render_base_disp(rs1, imm_i);
	return os.str();

}

std::string rv32i_decode::render_stype(uint32_t insn, const char *mnemonic)
{

	uint32_t rs1 = get_rs1(insn);
	uint32_t rs2 = get_rs2(insn);
	int32_t imm_s = get_imm_s(insn);


	std::ostringstream os;
	os << render_mnemonic(mnemonic) 
	   << render_reg(rs2) << ","
	   << render_base_disp(rs1, imm_s);
	return os.str();

}

std::string rv32i_decode::render_itype_alu(uint32_t insn, const char *mnemonic, int32_t imm_i)
{

	uint32_t rs1 = get_rs1(insn);
	int32_t rd = get_rd(insn);


	std::ostringstream os;
	os << render_mnemonic(mnemonic) << render_reg(rd) << ","
		<< render_reg(rs1) << "," << imm_i;
	return os.str();


}

std::string rv32i_decode::render_rtype(uint32_t insn, const char *mnemonic)
{

	uint32_t rs1 = get_rs1(insn);
	uint32_t rs2 = get_rs2(insn);
	int32_t rd = get_rd(insn);

	std::ostringstream os;
	os << render_mnemonic(mnemonic) << render_reg(rd) << ","
		<< render_reg(rs1) << "," << render_reg(rs2);
	return os.str();

}

//special cases no mnemonic needed
std::string rv32i_decode::render_ecall(uint32_t insn)
{
	std::ostringstream os;
	os << "ecall";
	return os.str();

}

std::string rv32i_decode::render_ebreak(uint32_t insn)
{

	std::ostringstream os;
	os << "ebreak";
	return os.str();
}

std::string rv32i_decode::render_csrrx(uint32_t insn, const char *mnemonic)
{

	uint32_t rs1 = get_rs1(insn);
	int32_t imm_i = get_imm_i(insn);
	int32_t rd = get_rd(insn);


	std::ostringstream os;
	os << render_mnemonic(mnemonic) << render_reg(rd) << ","
		<< hex::to_hex0x12(imm_i) << "," << render_reg(rs1);
	return os.str();

}

std::string rv32i_decode::render_csrrxi(uint32_t insn, const char *mnemonic)
{
	uint32_t rs1 = get_rs1(insn);
	int32_t rd = get_rd(insn);
	int32_t imm_i = get_imm_i(insn);

	std::ostringstream os;
	os << render_mnemonic(mnemonic) << render_reg(rd) << ","
		<< hex::to_hex0x12(imm_i) << "," << rs1;
	return os.str();


}

std::string rv32i_decode::render_reg(int r)  //gets register with x in front of reg number
{
	std::ostringstream os;
	os << "x" << r;
	return os.str();
	
	

}

std::string rv32i_decode::render_base_disp(uint32_t reg, int32_t imm) //for imm(register) type instructions
{
	std::ostringstream os;
	os << imm << "(" << render_reg(reg) << ")";
	return os.str();



}

std::string rv32i_decode::render_mnemonic(const std::string &m) //get instruction mnemonic
{
	std::ostringstream os;
	os << left << setw(mnemonic_width) << m;
	return os.str();
}



