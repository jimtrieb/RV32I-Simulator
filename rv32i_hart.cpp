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
#include <iomanip>
#include "rv32i_hart.h"
	
void rv32i_hart::tick(const std::string &hdr)
{
	if (halt == true)
	{
		return;
	}

	insn_counter++;

	if (show_registers) //show_registers = true
	{
		dump(hdr); //dump contents
	}
	
	
	uint32_t insn = mem.get32(pc);

	if (show_instructions)
	{
		cout << hdr << to_hex32(pc) << ": " << to_hex32(insn) << "  "; 
		exec(insn, &std::cout);
	}

	else
	{
		exec(insn, nullptr);
	}

}


void rv32i_hart::dump(const std::string &hdr) const
{
	regs.dump(hdr);
	cout << hdr << " pc " << hex::to_hex32(pc) << endl;
}

void rv32i_hart::reset()
{
	pc = 0;
	regs.reset();
	insn_counter = 0;          //set values on reset
	halt = false;
	halt_reason = "none";
}

void rv32i_hart::exec(uint32_t insn, std::ostream* pos)
{
	uint32_t opcode = get_opcode(insn);
	switch(opcode)							//switch on opcode to simulate execution
	{
		default: exec_illegal_insn(insn, pos); return;

		case opcode_lui: exec_lui(insn, pos); return;

		case opcode_auipc: exec_auipc(insn, pos); return;

		case opcode_jal: exec_jal(insn, pos); return;

		case opcode_jalr: exec_jalr(insn, pos); return;

		case opcode_btype:

			switch (get_funct3(insn)) //will switch on funct3 when b-types have same op code
			{
				default: exec_illegal_insn(insn, pos); return;
				case funct3_beq: exec_beq(insn, pos); return;
				case funct3_bne: exec_bne(insn, pos); return;
				case funct3_blt: exec_blt(insn, pos); return;
				case funct3_bge: exec_bge(insn, pos); return;
				case funct3_bltu: exec_bltu(insn, pos); return;
				case funct3_bgeu: exec_bgeu(insn, pos); return;
			}
				
		case opcode_load_imm:

			switch (get_funct3(insn)) //will switch on funct3 when i-type-loads have same op code
			{
				default: exec_illegal_insn(insn, pos); return;
				case funct3_lb: exec_lb(insn, pos); return;
				case funct3_lh: exec_lh(insn, pos); return;
				case funct3_lw: exec_lw(insn, pos); return;
				case funct3_lbu: exec_lbu(insn, pos); return;
				case funct3_lhu: exec_lhu(insn, pos); return;
			}

		case opcode_stype:
			
			switch (get_funct3(insn)) //will switch on funct3 when s-stypes have same op code
			{
				default: exec_illegal_insn(insn, pos); return;
				case funct3_sb: exec_sb(insn, pos); return;
				case funct3_sh: exec_sh(insn, pos); return;
				case funct3_sw: exec_sw(insn, pos); return;
			}

		case opcode_alu_imm:

			switch (get_funct3(insn)) //switch on funct3 when i-type-alus have same op code
			{
				default: exec_illegal_insn(insn, pos); return;
				case funct3_add: exec_addi(insn, pos); return;
				case funct3_sll: exec_slli(insn, pos); return;
				case funct3_slt: exec_slti(insn, pos); return;
				case funct3_sltu: exec_sltiu(insn, pos); return;
				case funct3_xor: exec_xori(insn, pos); return;
				case funct3_or: exec_ori(insn, pos); return;
				case funct3_and: exec_andi(insn, pos); return;
				case funct3_srx:
						  switch (get_funct7(insn)) //must switch on funct7 to differentiate between srli and srai instructions
						  {
							  default: exec_illegal_insn(insn, pos); return;
							  case funct7_srl: exec_srli(insn, pos); return;
							  case funct7_sra: exec_srai(insn, pos); return;
						  }
			}

		case opcode_rtype:

			switch (get_funct3(insn))
			{
				default: exec_illegal_insn(insn, pos); return;
				case funct3_add: 
						 switch (get_funct7(insn)) //r-type add instructions swtich on funct7
						 {
							default: exec_illegal_insn(insn, pos); return;
							case funct7_add: exec_add(insn, pos); return;
							case funct7_sub: exec_sub(insn, pos); return;
						 } 
				case funct3_sll: exec_sll(insn, pos); return;
				case funct3_slt: exec_slt(insn, pos); return; //switch on funct3
				case funct3_sltu: exec_sltu(insn, pos); return;
				case funct3_xor: exec_xor(insn, pos); return;
				case funct3_srx:
						 switch (get_funct7(insn))
						 {
							default: exec_illegal_insn(insn, pos); return;
							case funct7_srl: exec_srl(insn, pos); return; //r-type srx instructions switch on funct7
							case funct7_sra: exec_sra(insn, pos); return;
						 } 
				case funct3_or: exec_or(insn, pos); return;  //switch on funct 3
				case funct3_and: exec_and(insn, pos); return;
			}

		case opcode_system:
				if (insn == insn_ecall)
				{
					return exec_ecall(insn, pos); //specific cases no need to pass mnemonic for ecall and ebreak
				}
				
				else if(insn == insn_ebreak)
				{
					return exec_ebreak(insn, pos);
				}
			
				else
				{
		        		switch (get_funct3(insn))
					{
						default: exec_illegal_insn(insn, pos); return;
						case funct3_csrrs: exec_csrrs(insn, pos); return;
					}
				}
	}
}

void rv32i_hart::exec_illegal_insn(uint32_t insn , std :: ostream* pos)     //handles illegal instructions
{
	(void) insn;

	if (pos)
	{
		*pos << render_illegal_insn(insn) << endl;          
	}

	halt = true;
	halt_reason = "Illegal instruction";
}



//all exec functions will have a pos stream to ouput what the simulator is doing. They also all will update program by 4 or some value for branch instructions
//some but not all will set the destination register as well


void rv32i_hart::exec_lui(uint32_t insn, std::ostream* pos)
{
	//get required fields
	uint32_t rd = get_rd(insn);		
	int32_t imm_u = get_imm_u(insn) << 12; //shift over imm_u
	
 	if (pos)
 	{
 		std::string s = render_lui(insn);
 		*pos << std::setw(instruction_width) 
		<< std::setfill(' ') 
		<< std::left 
		<< s;
 		*pos << "// " 
		<< render_reg(rd) 
		<< " = " 
		<< hex::to_hex0x32(imm_u);
		*pos << endl;

 	}
 	regs.set(rd, imm_u); 
 	pc += 4; 

}
void rv32i_hart::exec_auipc(uint32_t insn, std::ostream* pos)
{
	
	//get required fields
	uint32_t rd = get_rd(insn);
	int32_t imm_u = get_imm_u(insn) << 12; //shift over imm_u
	int32_t val = pc + imm_u;


 	if (pos)
 	{
 		std::string s = render_auipc(insn);
 		*pos << std::setw(instruction_width) 
		<< std::setfill(' ') 
		<< std::left 
		<< s;
 		*pos << "// " 
		<< render_reg(rd) 
		<< " = " 						//print simulation
		<< hex::to_hex0x32(pc) 
		<< " + "
		<< hex::to_hex0x32(imm_u) 
		<< " = " 
		<< hex::to_hex0x32(val);
		*pos << endl;
 	}
 	regs.set(rd, val);
 	pc += 4;

}
void rv32i_hart::exec_jal(uint32_t insn, std::ostream* pos)
{
	//get required fields
	uint32_t rd = get_rd(insn);
	int32_t imm_j = get_imm_j(insn);
	int32_t val = pc + 4;

 	if (pos)
 	{
 		std::string s = render_jal(pc, insn);
 		*pos << std::setw(instruction_width) 
		<< std::setfill(' ') 
		<< std::left 
		<< s;
 		*pos << "// " 
		<< render_reg(rd) 
		<< " = " 					//print simulation
		<< hex::to_hex0x32(val) 
		<< ",  pc = "
		<< hex::to_hex0x32(pc) 
		<< " + " 
		<< hex::to_hex0x32(imm_j) 
		<< " = " 
		<< hex::to_hex0x32(pc + imm_j);
		*pos << endl;
 	}

	regs.set(rd, val);
	pc += imm_j;
}
void rv32i_hart::exec_jalr(uint32_t insn, std::ostream* pos)
{
	//get required fields
	uint32_t rd = get_rd(insn);
	int32_t imm_i = get_imm_i(insn);
 	uint32_t rs1 = get_rs1(insn);
	int32_t address = regs.get(rs1) + imm_i; //address to jump tp
	int32_t val = pc + 4;
	uint32_t mask = 0xfffffffe;
	address = address & mask;
 	if (pos)
 	{
 		std::string s = render_jalr(insn);
 		*pos << std::setw(instruction_width) 
		<< std::setfill(' ') 
		<< std::left 
		<< s
 		<< "// "
		<< render_reg(rd) 
		<< " = " 
		<< hex::to_hex0x32(val) 
		<< ",  pc = "					//print simulation
		<< "("
		<< hex::to_hex0x32(imm_i) 
		<< " + " 
		<< hex::to_hex0x32(regs.get(rs1)) 
		<< ")"
		<< " & "
		<< hex::to_hex0x32(mask)
		<<" = "
		<< hex::to_hex0x32(address);
		*pos << endl;
 	}

	regs.set(rd, val);
	pc = address;
}
void rv32i_hart::exec_beq(uint32_t insn, std::ostream* pos)
{

	//get required fields
 	uint32_t rs1 = get_rs1(insn);
 	uint32_t rs2 = get_rs2(insn);
	int32_t imm_b = get_imm_b(insn);

	if (pos)
	{
 		std::string s = render_btype(pc, insn, "beq");
 		*pos << std::setw(instruction_width) 
			<< std::setfill(' ') 
			<< std::left 
			<< s
 			<< "// "
			<< "pc += "
			<< "("
			<< hex::to_hex0x32(regs.get(rs1)) 
			<< " == " 						//print simulation
			<< hex::to_hex0x32(regs.get(rs2)) 
			<< " ? "
			<< hex::to_hex0x32(imm_b)
			<<" : "
			<< "4"
			<<")"
			<< " = ";
	}

	if (regs.get(rs1) == regs.get(rs2))
	{
		pc = pc + imm_b;			//if rs1 = rs2 add imm to pc
	}

	else
	{
		pc += 4;
	}

	if (pos)
	{
		*pos << hex::to_hex0x32(pc);
		*pos << endl;
	}
}
void rv32i_hart::exec_bne(uint32_t insn, std::ostream* pos)
{

	//get required fields
 	uint32_t rs1 = get_rs1(insn);
 	uint32_t rs2 = get_rs2(insn);
	int32_t imm_b = get_imm_b(insn);

	if (pos)
	{
 		std::string s = render_btype(pc, insn, "bne");
 		*pos << std::setw(instruction_width) 
			<< std::setfill(' ') 
			<< std::left 
			<< s
 			<< "// "
			<< "pc += "
			<< "("
			<< hex::to_hex0x32(regs.get(rs1)) 
			<< " != " 					//print simulation
			<< hex::to_hex0x32(regs.get(rs2)) 
			<< " ? "
			<< hex::to_hex0x32(imm_b)
			<<" : "
			<< "4" 
			<<")"
			<< " = ";
	}



	if (regs.get(rs1) != regs.get(rs2))
	{
		pc = pc + imm_b;			//if rs2 dne rs2 add imm to pc
	}

	else 
	{
		pc += 4;
	}
	if (pos)
	{
		*pos << hex::to_hex0x32(pc);
		*pos << endl;
	}
}
void rv32i_hart::exec_blt(uint32_t insn, std::ostream* pos)
{
	//get required fields
 	int32_t rs1 = get_rs1(insn);
 	int32_t rs2 = get_rs2(insn);
	int32_t imm_b = get_imm_b(insn);
	if (pos)
	{
 		std::string s = render_btype(pc, insn, "blt");
 		*pos << std::setw(instruction_width) 
			<< std::setfill(' ') 
			<< std::left 
			<< s
 			<< "// "
			<< "pc += "
			<< "("
			<< hex::to_hex0x32(regs.get(rs1)) 		//print simulation
			<< " < " 
			<< hex::to_hex0x32(regs.get(rs2)) 
			<< " ? "
			<< hex::to_hex0x32(imm_b)
			<<" : "
			<< "4"
			<<")"
			<< " = ";
	}



	if (regs.get(rs1) < regs.get(rs2))
	{
		pc = pc + imm_b;				//if rs1 < rs2 add imm to pc
	}
	else 
	{
		pc += 4;
	}

	if (pos)
	{
		*pos << hex::to_hex0x32(pc);
		*pos << endl;
	}

}
void rv32i_hart::exec_bge(uint32_t insn, std::ostream* pos)
{
	//get required fields
 	int32_t rs1 = get_rs1(insn);
 	int32_t rs2 = get_rs2(insn);
	int32_t imm_b = get_imm_b(insn);
	if (pos)
	{
 		std::string s = render_btype(pc, insn, "bge");
 		*pos << std::setw(instruction_width) 
			<< std::setfill(' ') 
			<< std::left 
			<< s
 			<< "// "
			<< "pc += "
			<< "("							//print simulation
			<< hex::to_hex0x32(regs.get(rs1)) 
			<< " >= " 
			<< hex::to_hex0x32(rs2) 
			<< " ? "
			<< hex::to_hex0x32(imm_b)
			<<" : "
			<< "4"
			<<")"
			<< " = ";
	}



	if (regs.get(rs1) >= regs.get(rs2))
	{
		pc = pc + imm_b;			//if rs1 >= rs2 add imm to pc
	}
	else 
	{
		pc += 4;
	}

	if (pos)
	{
		*pos << hex::to_hex0x32(pc);
		*pos << endl;
	}

}
void rv32i_hart::exec_bltu(uint32_t insn, std::ostream* pos)
{

	//get required fields
 	uint32_t rs1 = get_rs1(insn);
 	uint32_t rs2 = get_rs2(insn);
	int32_t imm_b = get_imm_b(insn);
	
	uint32_t rs1Val = regs.get(rs1);
	uint32_t rs2Val = regs.get(rs2);

	if (pos)
	{
 		std::string s = render_btype(pc, insn, "bltu");
 		*pos << std::setw(instruction_width) 
		<< std::setfill(' ') 
		<< std::left 
		<< s
 		<< "// "
		<< "pc += "
		<< "("
		<< hex::to_hex0x32(rs1Val) 
		<< " <U " 					//print simulation
		<< hex::to_hex0x32(rs2Val) 
		<< " ? "
		<< hex::to_hex0x32(imm_b)
		<<" : "
		<< "4"
		<<")"
		<< " = ";
	}

	if (rs1Val < rs2Val)
	{
		pc = pc + imm_b;   //if rs1 < rs1 add imm to pc
	}
	else 
	{
		pc += 4;
	}

	if (pos)
	{
		*pos << hex::to_hex0x32(pc);
		*pos << endl;
	}

}
void rv32i_hart::exec_bgeu(uint32_t insn, std::ostream* pos)
{
	//get required fields
 	uint32_t rs1 = get_rs1(insn);
 	uint32_t rs2 = get_rs2(insn);
	int32_t imm_b = get_imm_b(insn);
	
	uint32_t rs1Val = regs.get(rs1);
	uint32_t rs2Val = regs.get(rs2);
	
	if (pos)
	{
 		std::string s = render_btype(pc, insn, "bgeu");
 		*pos << std::setw(instruction_width) 
		<< std::setfill(' ') 
		<< std::left 
		<< s
 		<< "// "						//print simulation
		<< "pc += "
		<< "("
		<< hex::to_hex0x32(rs1Val) 
		<< " >=U " 
		<< hex::to_hex0x32(rs2Val) 
		<< " ? "
		<< hex::to_hex0x32(imm_b)
		<<" : "
		<< "4"
		<<")"
		<< " = ";
	}

	if (rs1Val >= rs2Val)
	{
		pc = pc + imm_b;   // if rs1>= to rs2 add imm to pc
	}
	else 
	{
		pc += 4;
	}

	if (pos)
	{
		*pos << hex::to_hex0x32(pc);
		*pos << endl;
	}

}
void rv32i_hart::exec_lb(uint32_t insn, std::ostream* pos)
{

	//get required fields
	uint32_t rd = get_rd(insn);
 	uint32_t rs1 = get_rs1(insn);
 	int32_t imm_i = get_imm_i(insn);
	uint32_t mask = 0xffffff00;

	int32_t address = regs.get(rs1) + imm_i;

	int32_t memVal;
	memVal  = mem.get8(address);

	if (memVal & 0x80)
	{
		memVal = memVal | mask;     //sign extend
	}


	if (pos)
	{
 		std::string s = render_itype_load(insn, "lb");
 		*pos << std::setw(instruction_width) 
		<< std::setfill(' ') 
		<< std::left 
		<< s
		<< "// "
		<< render_reg(rd)
		<< " = "					//print simulation
		<< "sx(m8("
		<< hex::to_hex0x32(regs.get(rs1))
		<< " + "
		<< hex::to_hex0x32(imm_i)
		<<")) = "
		<< hex::to_hex0x32(memVal);
		*pos << endl;
	}

	regs.set(rd, memVal);
	pc += 4;

}
void rv32i_hart::exec_lh(uint32_t insn, std::ostream* pos)
{

	//get required fields
	uint32_t rd = get_rd(insn);
 	uint32_t rs1 = get_rs1(insn);
 	int32_t imm_i = get_imm_i(insn);
	uint32_t mask = 0xffff0000;

	int32_t address = regs.get(rs1) + imm_i;

	int32_t memVal;
	memVal  = mem.get16(address);

	if (memVal & 0x8000)
	{
		memVal = memVal | mask;    //sign extend
	}


	if (pos)
	{
 		std::string s = render_itype_load(insn, "lh");
 		*pos << std::setw(instruction_width) 
		<< std::setfill(' ') 
		<< std::left 
		<< s
		<< "// "
		<< render_reg(rd)
		<< " = "
		<< "sx(m16("					//print simulation
		<< hex::to_hex0x32(regs.get(rs1))
		<< " + "
		<< hex::to_hex0x32(imm_i)
		<<")) = "
		<< hex::to_hex0x32(memVal);
		*pos << endl;
	}

	regs.set(rd, memVal);
	pc += 4;
}
void rv32i_hart::exec_lw(uint32_t insn, std::ostream* pos)
{

	//get required fields
	uint32_t rd = get_rd(insn);
 	uint32_t rs1 = get_rs1(insn);
 	int32_t imm_i = get_imm_i(insn);

	int32_t address = regs.get(rs1) + imm_i;

	uint32_t memVal;
	memVal  = mem.get32(address);

	int32_t signedMemVal;

	if (memVal & 0x80000000)
	{
		signedMemVal = memVal - 0x100000000;  //sign extend
	}

	else 
	{
		signedMemVal = memVal;
	}


	if (pos)
	{
 		std::string s = render_itype_load(insn, "lw");
 		*pos << std::setw(instruction_width) 
		<< std::setfill(' ') 
		<< std::left 
		<< s
		<< "// "
		<< render_reg(rd)
		<< " = "
		<< "sx(m32("						//print simulation
		<< hex::to_hex0x32(regs.get(rs1))
		<< " + "
		<< hex::to_hex0x32(imm_i)
		<<")) = "
		<< hex::to_hex0x32(signedMemVal);
		*pos << endl;
	}

	regs.set(rd, signedMemVal);
	pc += 4;
}
void rv32i_hart::exec_lbu(uint32_t insn, std::ostream* pos)
{
	//get required fields
	uint32_t rd = get_rd(insn);
 	uint32_t rs1 = get_rs1(insn);
 	int32_t imm_i = get_imm_i(insn);
	uint32_t mask = 0xff;            //mask for zero extend

	int32_t address = regs.get(rs1) + imm_i;

	int32_t memVal  = mem.get8(address) & mask;


	if (pos)
	{
 		std::string s = render_itype_load(insn, "lbu");
 		*pos << std::setw(instruction_width) 
		<< std::setfill(' ') 
		<< std::left 
		<< s
		<< "// "
		<< render_reg(rd)
		<< " = "
		<< "zx(m8("						//print simulation
		<< hex::to_hex0x32(regs.get(rs1))
		<< " + "
		<< hex::to_hex0x32(imm_i)
		<<")) = "
		<< hex::to_hex0x32(memVal);
		*pos << endl;
	}

	regs.set(rd, memVal);

	pc += 4;
}
void rv32i_hart::exec_lhu(uint32_t insn, std::ostream* pos)
{
	//get required fields
	uint32_t rd = get_rd(insn);
 	uint32_t rs1 = get_rs1(insn);
 	int32_t imm_i = get_imm_i(insn);
	uint32_t mask = 0xffff;       //mask for zero extend

	int32_t address = regs.get(rs1) + imm_i;

	int32_t memVal  = mem.get16(address) & mask;

	if (pos)
	{
 		std::string s = render_itype_load(insn, "lhu");
 		*pos << std::setw(instruction_width) 
		<< std::setfill(' ') 
		<< std::left 
		<< s								//print simulation
		<< "// "
		<< render_reg(rd)
		<< " = "
		<< "zx(m16("
		<< hex::to_hex0x32(regs.get(rs1))
		<< " + "
		<< hex::to_hex0x32(imm_i)
		<<")) = "
		<< hex::to_hex0x32(memVal);
		*pos << endl;
	}

	regs.set(rd, memVal);
	pc += 4;
}
void rv32i_hart::exec_sb(uint32_t insn, std::ostream* pos)
{
	//get required fields
 	uint32_t rs1 = get_rs1(insn);
 	uint32_t rs2 = get_rs2(insn);
 	int32_t imm_s = get_imm_s(insn);

 	uint32_t maskVal = regs.get(rs2) & 0xff;   //get 8 lsb

	uint32_t address = regs.get(rs1) + imm_s;
	
	mem.set8(address, maskVal);
	
	if (pos)
	{
 		std::string s = render_stype(insn, "sb");
 		*pos << std::setw(instruction_width) 
		<< std::setfill(' ') 
		<< std::left 
		<< s							//print simulation
		<< "// m8("
		<< hex::to_hex0x32(regs.get(rs1))
		<< " + "
		<< hex::to_hex0x32(imm_s)
		<< ") = "
		<< hex::to_hex0x32(maskVal);
		*pos << endl;
	}

	pc += 4;

}
void rv32i_hart::exec_sh(uint32_t insn, std::ostream* pos)
{

	
	//get required fields
 	uint32_t rs1 = get_rs1(insn);
 	uint32_t rs2 = get_rs2(insn);
 	int32_t imm_s = get_imm_s(insn);

 	uint32_t maskVal = regs.get(rs2) & 0xffff;  //16 lsb

	uint32_t address = regs.get(rs1) + imm_s;
	
	mem.set16(address, maskVal);
	
	if (pos)
	{
 		std::string s = render_stype(insn, "sh");
 		*pos << std::setw(instruction_width) 
		<< std::setfill(' ') 
		<< std::left 					//print simulation
		<< s
		<< "// m16("
		<< hex::to_hex0x32(regs.get(rs1))
		<< " + "
		<< hex::to_hex0x32(imm_s)
		<< ") = "
		<< hex::to_hex0x32(maskVal);
		*pos << endl;
	}

	pc += 4;
}
void rv32i_hart::exec_sw(uint32_t insn, std::ostream* pos)
{

	//get required fields
 	uint32_t rs1 = get_rs1(insn);
 	uint32_t rs2 = get_rs2(insn);
 	int32_t imm_s = get_imm_s(insn);

	uint32_t address = regs.get(rs1) + imm_s;
	
	mem.set32(address, regs.get(rs2));
	
	if (pos)
	{
 		std::string s = render_stype(insn, "sw");
 		*pos << std::setw(instruction_width) 
		<< std::setfill(' ') 
		<< std::left 
		<< s
		<< "// m32("					//print simulation
		<< hex::to_hex0x32(regs.get(rs1))
		<< " + "
		<< hex::to_hex0x32(imm_s)
		<< ") = "
		<< hex::to_hex0x32(regs.get(rs2));
		*pos << endl;
	}

	pc += 4;
}
void rv32i_hart::exec_addi(uint32_t insn, std::ostream* pos)
{
	//get required fields
	uint32_t imm_i = get_imm_i(insn);
	uint32_t rs1 = get_rs1(insn);
	uint32_t rd = get_rd(insn);
	

	uint32_t updatedRd = regs.get(rs1) + imm_i; //rs1+imm

	if (pos)
	{
 		std::string s = render_itype_alu(insn, "addi", imm_i);
 		*pos << std::setw(instruction_width) 
		<< std::setfill(' ') 
		<< std::left 
		<< s						//print simulation
		<< "// "
		<< render_reg(rd)
		<< " = "
		<< hex::to_hex0x32(regs.get(rs1))
		<< " + "
		<< hex::to_hex0x32(imm_i)
		<< " = "
		<< hex::to_hex0x32(updatedRd);
		*pos << endl;
	}



	regs.set(rd, updatedRd);
 	pc += 4;
}
void rv32i_hart::exec_slti(uint32_t insn, std::ostream* pos)
{
	//get required fields
	int32_t rs1 = get_rs1(insn);
	int32_t rd = get_rd(insn);
	int32_t imm_i = get_imm_i(insn);

	if (pos)
	{
 		std::string s = render_itype_alu(insn, "slti", imm_i);
 		*pos << std::setw(instruction_width) 
		<< std::setfill(' ') 
		<< std::left 
		<< s				//print simulation
		<< "// "
		<< render_reg(rd)
		<< " = ("
		<< hex::to_hex0x32(regs.get(rs1))
		<< " < "
		<< imm_i
		<< ") ? 1 : 0 = ";
	}


	if (regs.get(rs1) < imm_i)
	{
		regs.set(rd, 1);    //if rs1 < imm set rd to 1 
	}

	else
	{
		regs.set(rd, 0);
	}
	if (pos)
	{
		*pos << hex::to_hex0x32(regs.get(rd));
		*pos << endl;
	}
 	pc += 4;
}
void rv32i_hart::exec_sltiu(uint32_t insn, std::ostream* pos)
{
	//get required fields
	
	uint32_t rs1 = get_rs1(insn);
	uint32_t rd = get_rd(insn);
	uint32_t imm_i = get_imm_i(insn);

	int32_t signedReg;
	uint32_t reg;

	signedReg = regs.get(rs1);

	if (signedReg & 0x80000000)  //check msb
	{
		reg = signedReg + 0x100000000;
	}

	else 
	{
		reg = signedReg;
	}

	if (pos)
	{
 		std::string s = render_itype_alu(insn, "sltiu", imm_i);
 		*pos << std::setw(instruction_width) 
		<< std::setfill(' ') 
		<< std::left 
		<< s
		<< "// "
		<< render_reg(rd)					//print simulation
		<< " = ("
		<< hex::to_hex0x32(regs.get(rs1))
		<< " <U "
		<< imm_i
		<< ") ? 1 : 0 = ";
	}


	if (reg < imm_i)
	{
		regs.set(rd, 1); //set rd to 1 if reg < imm
	}

	else
	{
		regs.set(rd, 0);
	}
	if (pos)
	{
		*pos << hex::to_hex0x32(regs.get(rd));
		*pos << endl;
	}
 	pc += 4;
}
void rv32i_hart::exec_xori(uint32_t insn, std::ostream* pos)
{
	//get required fields
	uint32_t rs1 = get_rs1(insn);
	uint32_t rd = get_rd(insn);
	uint32_t imm_i = get_imm_i(insn);

	if (pos)
	{
 		std::string s = render_itype_alu(insn, "xori", imm_i);
 		*pos << std::setw(instruction_width) 
		<< std::setfill(' ') 
		<< std::left 
		<< s
		<< "// "
	        << render_reg(rd)			//print simulation
       		<< " = "
 		<< hex::to_hex0x32(regs.get(rs1))
		<< " ^ "
		<< hex::to_hex0x32(imm_i)
		<< " = ";
	}

	regs.set(rd, (regs.get(rs1) ^ imm_i));  //bitwise xor
	if (pos)
	{
		*pos << hex::to_hex0x32(regs.get(rd));
		*pos << endl;
	}
 	pc += 4;
}
void rv32i_hart::exec_ori(uint32_t insn, std::ostream* pos)
{
 
	//get required fields
	uint32_t rs1 = get_rs1(insn);
	uint32_t rd = get_rd(insn);
	uint32_t imm_i = get_imm_i(insn);

	if (pos)
	{
 		std::string s = render_itype_alu(insn, "ori", imm_i);
 		*pos << std::setw(instruction_width) 
		<< std::setfill(' ') 
		<< std::left 
		<< s
		<< "// "
	        << render_reg(rd)
       		<< " = "					//print simulation
 		<< hex::to_hex0x32(regs.get(rs1))
		<< " | "
		<< hex::to_hex0x32(imm_i)
		<< " = ";
	}

	regs.set(rd, (regs.get(rs1) | imm_i));  //bitwise or
	if (pos)
	{
		*pos << hex::to_hex0x32(regs.get(rd));
		*pos << endl;
	}
 	pc += 4;
}
void rv32i_hart::exec_andi(uint32_t insn, std::ostream* pos)
{
	//get required fields

	uint32_t rs1 = get_rs1(insn);
	uint32_t rd = get_rd(insn);
	uint32_t imm_i = get_imm_i(insn);

	if (pos)
	{
 		std::string s = render_itype_alu(insn, "andi", imm_i);
 		*pos << std::setw(instruction_width) 
		<< std::setfill(' ') 
		<< std::left 
		<< s					//print simulation
		<< "// "
	        << render_reg(rd)
       		<< " = "
 		<< hex::to_hex0x32(regs.get(rs1))
		<< " & "
		<< hex::to_hex0x32(imm_i)
		<< " = ";
	}

	regs.set(rd, (regs.get(rs1) & imm_i));  //bitwise and
	if (pos)
	{
		*pos << hex::to_hex0x32(regs.get(rd));
		*pos << endl;
	}
 	pc += 4;
}
void rv32i_hart::exec_slli(uint32_t insn, std::ostream* pos)
{
	//get required fields
	
	uint32_t rs1 = get_rs1(insn);
	uint32_t shamt_i = get_rs2(insn);
	uint32_t rd = get_rd(insn);
	
	uint32_t shiftVal = regs.get(rs1) << shamt_i; //shift rs1

	if (pos)
	{
 		std::string s = render_itype_alu(insn, "slli", shamt_i);
 		*pos << std::setw(instruction_width) 
		<< std::setfill(' ') 
		<< std::left 
		<< s			//print simulation
		<< "// "
		<< render_reg(rd)
		<< " = "
		<<hex::to_hex0x32(regs.get(rs1))
		<< " << "
		<< shamt_i
		<< " = ";
	}


	regs.set(rd, shiftVal);
	if (pos)
	{
		*pos << hex::to_hex0x32(regs.get(rd));
		*pos << endl;
	}
 	pc += 4;

}
void rv32i_hart::exec_srli(uint32_t insn, std::ostream* pos)
{
	//get required fields
	uint32_t rs1 = get_rs1(insn);
	uint32_t shamt_i = get_rs2(insn);
	uint32_t rd = get_rd(insn);

	int32_t shiftVal = regs.get(rs1) >> shamt_i;  //logit shift rs1 right

	if (regs.get(rs1) & 0x80000000) //check high order bit
	{
		uint32_t maskVal = 0xffffffff >> (shamt_i); 
		shiftVal = shiftVal & maskVal;

	}
	
	if (pos)
	{
 		std::string s = render_itype_alu(insn, "srli", shamt_i);
 		*pos << std::setw(instruction_width) 
		<< std::setfill(' ') 
		<< std::left 
		<< s
		<< "// "
		<< render_reg(rd)
		<< " = "				//print simulation
		<< hex::to_hex0x32(regs.get(rs1))
		<< " >> "
		<< shamt_i
		<< " = ";
	}

	regs.set(rd, shiftVal);

	if (pos)
	{
		*pos << hex::to_hex0x32(regs.get(rd));
		*pos << endl;
	}
 	pc += 4;


}
void rv32i_hart::exec_srai(uint32_t insn, std::ostream* pos)
{
	//get required fields
	
	uint32_t rs1 = get_rs1(insn);
	uint32_t shamt_i = get_rs2(insn);
	uint32_t rd = get_rd(insn);

	int32_t shiftVal = regs.get(rs1) >> shamt_i; //arithmetic shift rs1 right

	if (pos)
	{
 		std::string s = render_itype_alu(insn, "srai", shamt_i);
 		*pos << std::setw(instruction_width) 
		<< std::setfill(' ') 
		<< std::left 
		<< s				//print simulation
		<< "// "
		<< render_reg(rd)
		<< " = "
		<< hex::to_hex0x32(regs.get(rs1))
		<< " >> "
		<< shamt_i
		<< " = ";
	}

	regs.set(rd, shiftVal);
	if (pos)
	{
		*pos << hex::to_hex0x32(regs.get(rd));
		*pos << endl;
	}
 	pc += 4;
}
void rv32i_hart::exec_add(uint32_t insn, std::ostream* pos)
{
	//get required fields
	uint32_t rd = get_rd(insn);
 	uint32_t rs1 = get_rs1(insn);
 	uint32_t rs2 = get_rs2(insn);

 	int32_t val = regs.get(rs1) + regs.get(rs2); //rs1 + rs2

 	if (pos)
 	{
 		std::string s = render_rtype(insn, "add");
 		*pos << std::setw(instruction_width) 
		<< std::setfill(' ') 
		<< std::left 
		<< s;
 		*pos << "// " 
		<< render_reg(rd) 		//print simulation
		<< " = " 
		<< hex::to_hex0x32(regs.get(rs1)) 
		<< " + " 
		<< hex::to_hex0x32(regs.get(rs2)) 
		<< " = " 
		<< hex::to_hex0x32(val);
		*pos << endl;
 	}
 	regs.set(rd, val);
 	pc += 4;
}
void rv32i_hart::exec_sub(uint32_t insn, std::ostream* pos)
{
	//get required fields

	uint32_t rd = get_rd(insn);
 	uint32_t rs1 = get_rs1(insn);
 	uint32_t rs2 = get_rs2(insn);

 	int32_t val = regs.get(rs1) - regs.get(rs2); //rs1-rs2

 	if (pos)
 	{
 		std::string s = render_rtype(insn, "sub");
 		*pos << std::setw(instruction_width) 
		<< std::setfill(' ') 
		<< std::left 
		<< s;
 		*pos << "// " 
		<< render_reg(rd) 
		<< " = " 		//print simulation
		<< hex::to_hex0x32(regs.get(rs1)) 
		<< " - " 
		<< hex::to_hex0x32(regs.get(rs2)) 
		<< " = " 
		<< hex::to_hex0x32(val);
		*pos << endl;
 	}
 	regs.set(rd, val);
 	pc += 4;
}
void rv32i_hart::exec_sll(uint32_t insn, std::ostream* pos)
{
	//get required fields

	uint32_t rd = get_rd(insn);
 	uint32_t rs1 = get_rs1(insn);
 	uint32_t rs2 = get_rs2(insn);

	int32_t shiftVal = (0x1f & regs.get(rs2)); //lsb 5
 	int32_t val = regs.get(rs1) << shiftVal; //shift rs1 left

 	if (pos)
 	{
 		std::string s = render_rtype(insn, "sll");
 		*pos << std::setw(instruction_width) 
		<< std::setfill(' ') 
		<< std::left << s;
 		*pos << "// " 
		<< render_reg(rd) 		//print simulation
		<< " = " 
		<< hex::to_hex0x32(regs.get(rs1)) 
		<< " << " 
		<< shiftVal 
		<< " = " 
		<< hex::to_hex0x32(val);
		*pos << endl;
 	}
 	regs.set(rd, val);
 	pc += 4;
}
void rv32i_hart::exec_slt(uint32_t insn, std::ostream* pos)
{
	//get required fields
	uint32_t rd = get_rd(insn);
 	uint32_t rs1 = get_rs1(insn);
 	uint32_t rs2 = get_rs2(insn);

 	int32_t val = (regs.get(rs1) < regs.get(rs2)) ? 1 : 0; //if rs1 < rs1 set rd to 1 else 0

 	if (pos)
 	{
 		std::string s = render_rtype(insn, "slt");
 		*pos << std::setw(instruction_width) 
		<< std::setfill(' ') 
		<< std::left 
		<< s;
 		*pos << "// " 
		<< render_reg(rd) 
		<< " = (" 			//print simulation
		<< hex::to_hex0x32(regs.get(rs1)) 
		<< " < " 
		<< hex::to_hex0x32(regs.get(rs2)) 
		<< ") ? 1 : 0 = " 
		<< hex::to_hex0x32(val);
		*pos << endl;
 	}
 	regs.set(rd, val);
 	pc += 4;
}
void rv32i_hart::exec_sltu(uint32_t insn, std::ostream* pos)
{

	uint32_t rd = get_rd(insn);
 	uint32_t rs1 = get_rs1(insn);
 	uint32_t rs2 = get_rs2(insn);

 	int32_t val;
	
	if(regs.get(rs1) < regs.get(rs2))
	{
		val = 1; //will set rd to 1 else 0
	}

	else 
	{
		val = 0;
	}

 	if (pos)
 	{
 		std::string s = render_rtype(insn, "sltu");
 		*pos << std::setw(instruction_width) 
		<< std::setfill(' ') 
		<< std::left 
		<< s;
 		*pos << "// " 
		<< render_reg(rd) 		//print simulation
		<< " = (" 
		<< hex::to_hex0x32(regs.get(rs1)) 
		<< " <U " 
		<< hex::to_hex0x32(regs.get(rs2)) 
		<< ") ? 1 : 0 = " 
		<< hex::to_hex0x32(val);
		*pos << endl;
 	}
 	regs.set(rd, val);
 	pc += 4;
}
void rv32i_hart::exec_xor(uint32_t insn, std::ostream* pos)
{
	//get required fields

	uint32_t rd = get_rd(insn);
 	uint32_t rs1 = get_rs1(insn);
 	uint32_t rs2 = get_rs2(insn);

 	int32_t val = regs.get(rs1) ^ regs.get(rs2); //bitwise xor

 	if (pos)
 	{
 		std::string s = render_rtype(insn, "xor");
 		*pos << std::setw(instruction_width) 
		<< std::setfill(' ') 
		<< std::left 
		<< s;
 		*pos << "// " 
		<< render_reg(rd) 
		<< " = " 				//print simulation
		<< hex::to_hex0x32(regs.get(rs1)) 
		<< " ^ " 
		<< hex::to_hex0x32(regs.get(rs2)) 
		<< " = " 
		<< hex::to_hex0x32(val);
		*pos << endl;
 	}
 	regs.set(rd, val);
 	pc += 4;
}
void rv32i_hart::exec_srl(uint32_t insn, std::ostream* pos)
{
	//get required fields

	uint32_t rd = get_rd(insn);
	uint32_t rs2 = get_rs2(insn);
 	uint32_t rs1 = get_rs1(insn);

	int32_t numberOfBitsToShift = (0x1f & regs.get(rs2)); //5 lsb of rs2
 	int32_t val = regs.get(rs1) >> numberOfBitsToShift; //logic shift rs1 right


	if (regs.get(rs1) & 0x80000000) //check high order bit
	{
		uint32_t maskVal = 0xffffffff >> (numberOfBitsToShift); 
		val = maskVal & val;

	}
 	if (pos)
 	{
 		std::string s = render_rtype(insn, "srl");
 		*pos << std::setw(instruction_width) 
		<< std::setfill(' ') 
		<< std::left 
		<< s;
 		*pos << "// " 
		<< render_reg(rd) 
		<< " = " 			//print simulation
		<< hex::to_hex0x32(regs.get(rs1)) 
		<< " >> " 
		<< numberOfBitsToShift 
		<< " = " 
		<< hex::to_hex0x32(val);
		*pos << endl;
 	}
 	regs.set(rd, val);
 	pc += 4;
}
void rv32i_hart::exec_sra(uint32_t insn, std::ostream* pos)
{
	//get required fields

	uint32_t rd = get_rd(insn);
 	uint32_t rs1 = get_rs1(insn);
 	uint32_t rs2 = get_rs2(insn);

	int32_t shiftVal = (0x1f & regs.get(rs2)); //lsb 5 bits of rs2

 	int32_t val = regs.get(rs1) >> shiftVal; //arithmetic shift rs1 right

 	if (pos)
 	{
 		std::string s = render_rtype(insn, "sra");
 		*pos << std::setw(instruction_width) 
		<< std::setfill(' ') 
		<< std::left 
		<< s;
 		*pos << "// " 
		<< render_reg(rd) 
		<< " = " 			//print simulation
		<< hex::to_hex0x32(regs.get(rs1)) 
		<< " >> " 
		<< shiftVal 
		<< " = " 
		<< hex::to_hex0x32(val);
		*pos << endl;
 	}
 	regs.set(rd, val);
 	pc += 4;
}
void rv32i_hart::exec_or(uint32_t insn, std::ostream* pos)
{
	//get required fields

	uint32_t rd = get_rd(insn);
 	uint32_t rs1 = get_rs1(insn);
 	uint32_t rs2 = get_rs2(insn);

 	int32_t val = regs.get(rs1) | regs.get(rs2); //bitwise or

 	if (pos)
 	{
 		std::string s = render_rtype(insn, "or");
 		*pos << std::setw(instruction_width) 
		<< std::setfill(' ') 
		<< std::left 
		<< s;
 		*pos << "// " 
		<< render_reg(rd) 
		<< " = " 			//print simulation
		<< hex::to_hex0x32(regs.get(rs1)) 
		<< " | " 
		<< hex::to_hex0x32(regs.get(rs2)) 
		<< " = " 
		<< hex::to_hex0x32(val);
		*pos << endl;
 	}
 	regs.set(rd, val);
 	pc += 4;
}
void rv32i_hart::exec_and(uint32_t insn, std::ostream* pos)
{
	//get required fields

	uint32_t rd = get_rd(insn);
 	uint32_t rs1 = get_rs1(insn);
 	uint32_t rs2 = get_rs2(insn);

 	int32_t val = regs.get(rs1) & regs.get(rs2);  //bitwise and

 	if (pos)
 	{
 		std::string s = render_rtype(insn, "and");
 		*pos << std::setw(instruction_width) 
		<< std::setfill(' ') 
		<< std::left 
		<< s;
 		*pos << "// " 				//print simulation
		<< render_reg(rd) 
		<< " = " 
		<< hex::to_hex0x32(regs.get(rs1)) 
		<< " & " 
		<< hex::to_hex0x32(regs.get(rs2)) 
		<< " = " 
		<< hex::to_hex0x32(val);
		*pos << endl;
 	}
 	regs.set(rd, val);
 	pc += 4;
}
void rv32i_hart::exec_csrrs(uint32_t insn, std::ostream* pos)
{
	//get required fields
	uint32_t rd = get_rd(insn);
	int32_t imm_i = get_imm_i(insn) & 0xfff;
	if (pos)
	{
 		std::string s = render_csrrx(insn, "csrrs");
 		*pos << std::setw(instruction_width) 
		<< std::setfill(' ') 
		<< std::left 
		<< s;
 		*pos << "// " 
		<< render_reg(rd) 			//print simulation
		<< " = " 
		<< mhartid;
		*pos << endl;
	}
	
	if (imm_i == 0xf14)
	{
		regs.set(rd, mhartid); //set rd to hart id if imm = 0xf14
		pc += 4;
	}

	else
	{
		halt_reason = "Illegal CSR in CRRSS instruction"; //set reason
		halt = true;
	}

}

void rv32i_hart::exec_ecall(uint32_t insn, std::ostream* pos)
{
	if (pos)
	{
 		std::string s = render_ecall(insn);
 		*pos << std::setw(instruction_width) 
		<< std::setfill(' ') 
		<< std::left 
		<< s;				//print simualtion
 		*pos << "// HALT";
		*pos << endl;
	}
	halt_reason = "ECALL instruction"; //set reason
	halt = true;

}

void rv32i_hart::exec_ebreak(uint32_t insn, std::ostream* pos)
{
	if (pos)
	{
 		std::string s = render_ebreak(insn);
 		*pos << std::setw(instruction_width) 
		<< std::setfill(' ') 
		<< std::left 
		<< s;				//print simulation
 		*pos << "// HALT";
		*pos << endl;
	}
	halt_reason = "EBREAK instruction"; //set reason
	halt = true;

}
