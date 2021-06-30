//***************************************************************************
//
//  rv32i.cpp
//  CSCI 463 Assignment 4
//
//  Created by Ian Sullivan z1830506
//
//***************************************************************************

#include <iostream>
#include <iomanip>
#include "rv32i.h"
#include "memory.h"
#include "hex.h"
#include <cassert>
#include "registerfile.h"


using std::string;
using std::cin;
using std::cout;
using std::endl;


/**
 * Constructor for rv32i class
 * 
 * sets mem = to the m arguement
 *
 * @param m a reference to a memory object
 *
 *
 ********************************************************************************/
rv32i::rv32i(memory *m) {

    mem = m;

}

void rv32i::set_show_insturctions(bool b) {

    show_instructions = b;
    //cout << "Show instructions is set" << show_instructions << endl;

}

void rv32i::set_show_registers(bool b) {

    show_registers = b;

}

/**
 * disasmbler function
 * 
 * it's primary purpose is to keep track of the pc counter and loop through the program
 *
 * 
 *
 *
 ********************************************************************************/
void rv32i::disasm(void) {

    pc = 0;

    for (pc = 0; pc < mem->get_size(); pc += 4) {

        cout << hex32(pc) << ": "; //print out the pc counter

        cout << hex32(mem->get32(pc)) << "  "; //print out the 32 bit hex value at pc
        cout << decode(mem->get32(pc)) << endl; //print the string given from decode


    }

    

}

/**
 * reset function
 * 
 * basically just resets all the instructions to the beginning and clears a halt
 * flag
 *
 ********************************************************************************/
void rv32i::reset() {

    pc = 0;
    insn_counter = 0;
    halt = false;

}

/**
 * dump function
 * 
 * this dumps the registers and the pc counter + value
 *
 *
 ********************************************************************************/
void rv32i::dump() const {

    regs.dump();
    cout << std::right << " pc" << " " << hex32(pc) << endl; // maybe edit this later

}

/**
 * tick function
 * 
 * this function holds a lot of the logic that decides what prints
 * most decides how to call dcex() which contains most of the logic
 * for actually running the program
 *
 *
 ********************************************************************************/
void rv32i::tick() {

    if (halt == true) {

        return;

    }

    //cout << "I made it to before show reg" << show_registers << endl;
    insn_counter++;

    if (show_registers == true) {

       //cout << "I tried to dump during show_registers" << endl;
        regs.dump();
        cout << " pc " << hex32(pc) << endl; 

    }

    //cout << "I made it to after show reg" << endl;
    uint32_t insn = mem->get32(pc);

    if (show_instructions == true) {

        cout << hex32(pc) << ": "; //print out the pc counter

        cout << hex32(mem->get32(pc)) << "  "; //print out the 32 bit hex value at pc
        
        dcex(insn, &std::cout);

    } else {

        dcex(insn, nullptr);

    }



}

/**
 * run function
 * 
 * This 'runs' the program it basically just checks to see if the program should
 * stop and if it doesn't need to stop it calls tick.
 * 
 * @param limit a uint64 which determins how many instructions maximum can run
 *
 ********************************************************************************/
void rv32i::run(uint64_t limit) {

    regs.set(2, mem->get_size());

    while (halt == false && insn_counter <= limit) {

        tick();

    }

    cout << insn_counter << " instructions executed" << endl;
    //Add a message if terminated by ebreak

}

/**
 * dcex
 * 
 * Contains a lot of the logic for running the program. It uses the same logic
 * as decode to decide which instruction needs to run. It then calls the relevant
 * exec function
 *
 * @param insn a uint32 which represents the current instruction
 * @param pos  a pointer to an ostream
 * 
 *
 ********************************************************************************/
void rv32i::dcex(uint32_t insn, std::ostream* pos) {

    
    uint32_t opcode = get_opcode(insn);
    uint32_t funct3 = get_funct3(insn);
    uint32_t funct7 = get_funct7(insn);
    //int32_t imm_i = get_imm_i(insn);
    uint32_t rs2 = get_rs2(insn);
    //cout << "I hit dcex" << endl;
    
    switch(opcode)
    {
        //Some of the opcode individual cases
        default:                    exec_illegal_insn(insn, pos); return;
        case opcode_lui:            exec_lui(insn, pos); return;// render_lui(insn);
        case opcode_auipc:          exec_auipc(insn, pos); return;// render_auipc(insn);
        case opcode_jal:            exec_jal(insn, pos); return;// render_jal(insn, pc);
        case opcode_jalr:           exec_jalr(insn, pos); return;// render_jalr(insn, imm_i);
        case opcode_fence:          exec_fence(insn, pos); return;// render_fence(insn);
        case opcode_rtype:          
        
        //rtype instructions
        switch(funct3)
        {
        default:                exec_illegal_insn(insn, pos); return;// render_illegal_insn();
        case funct3_add:
            switch(funct7)
            {
            default:            exec_illegal_insn(insn, pos); return;// render_illegal_insn();
            case funct7_add:    exec_add(insn, pos); return;// render_rtype(insn, "add");
            case funct7_sub:    exec_sub(insn, pos); return;// render_rtype(insn, "sub");
            }

            //assert(0 && "unhandled funct7");
        case funct3_sll:        exec_sll(insn, pos); return;// render_rtype(insn, "sll");
        case funct3_slt:        exec_slt(insn, pos); return;// render_rtype(insn, "slt");
        case funct3_sltu:       exec_sltu(insn, pos); return;// render_rtype(insn, "sltu");
        case funct3_xor:        exec_xor(insn, pos); return;// render_rtype(insn, "xor");
        case funct3_srx:
            switch(funct7) {

            default:            exec_illegal_insn(insn, pos); return;// render_illegal_insn();
            case funct7_srl:    exec_srl(insn, pos); return;// render_rtype(insn, "srl");
            case funct7_sra:    exec_sra(insn, pos); return;// render_rtype(insn, "sra");
            }

            //assert(0 && "unhandled funct7");
        case funct3_or:         exec_or(insn, pos); return;// render_rtype(insn, "or");
        case funct3_and:        exec_and(insn, pos); return;// render_rtype(insn, "and");

        }
        
        //assert(0 && "unhandled funct3");
        

        case opcode_alu_imm:
        //Some of the I instructions
        
        switch (funct3)
        {
        default:                exec_illegal_insn(insn, pos); return;// render_illegal_insn();
        case funct3_sll:        exec_slli(insn, pos); return;// render_itype_alu(insn, "slli", imm_i%XLEN);
        case funct3_srx:
            switch(funct7)
            {
            default:            exec_illegal_insn(insn, pos); return;// render_illegal_insn();
            case funct7_sra:    exec_srai(insn, pos); return;// render_itype_alu(insn, "srai", imm_i%XLEN);
            case funct7_srl:    exec_srli(insn, pos); return;// render_itype_alu(insn, "srli", imm_i%XLEN);
            }
            //assert(0 && "unhandled funct7");
        case funct3_add:        exec_addi(insn, pos); return;// render_itype_alu(insn, "addi", imm_i);
        case funct3_slti:       exec_slti(insn, pos); return;// render_itype_alu(insn, "slti", imm_i);
        case funct3_sltiu:      exec_sltiu(insn, pos); return;// render_itype_alu(insn, "sltiu", imm_i);
        case funct3_xori:       exec_xori(insn, pos); return;// render_itype_alu(insn, "xori", imm_i);
        case funct3_ori:        exec_ori(insn, pos); return;// render_itype_alu(insn, "ori", imm_i);
        case funct3_andi:       exec_andi(insn, pos); return;// render_itype_alu(insn, "andi", imm_i);
        }
        
        //assert(0 && "unhandled funct3");
        
        case opcode_load_imm:
        //The rest of the I instructions
        
        switch (funct3) {

        default:                exec_illegal_insn(insn, pos); return;// render_illegal_insn();
        case funct3_lb:         exec_lb(insn, pos); return;// render_itype_load(insn, "lb", imm_i);
        case funct3_lh:         exec_lh(insn, pos); return;// render_itype_load(insn, "lh", imm_i);
        case funct3_lw:         exec_lw(insn, pos); return;// render_itype_load(insn, "lw", imm_i);
        case funct3_lbu:        exec_lbu(insn, pos); return;// render_itype_load(insn, "lbu", imm_i);
        case funct3_lhu:        exec_lhu(insn, pos); return;// render_itype_load(insn, "lhu", imm_i);

        }
        
    
        case opcode_btype:
        //B type instructions
        
        switch (funct3) {

        default:                exec_illegal_insn(insn, pos); return;// render_illegal_insn();
        case funct3_beq:        exec_beq(insn, pos); return;// render_btype(insn, "beq", pc);
        case funct3_bne:        exec_bne(insn, pos); return;// render_btype(insn, "bne", pc);
        case funct3_blt:        exec_blt(insn, pos); return;// render_btype(insn, "blt", pc);
        case funct3_bge:        exec_bge(insn, pos); return;// render_btype(insn, "bge", pc);
        case funct3_bltu:       exec_bltu(insn, pos); return;// render_btype(insn, "bltu", pc);
        case funct3_bgeu:       exec_bgeu(insn, pos); return;// render_btype(insn, "bgeu", pc);

        }
        
    
        case opcode_stype:
        //S type instructions
        
        switch (funct3) {

        default:                exec_illegal_insn(insn, pos); return;// render_illegal_insn();
        case funct3_sb:         exec_sb(insn, pos); return;// render_stype(insn, "sb");
        case funct3_sh:         exec_sh(insn, pos); return;// render_stype(insn, "sh");
        case funct3_sw:         exec_sw(insn, pos); return;// render_stype(insn, "sw");

        }
        
    
        case opcode_e:
        //'e' instructions ebreak and ecall
            switch (rs2) {

            default:                exec_illegal_insn(insn, pos); return; // render_illegal_insn();
            case rs2_ebreak:        exec_ebreak(insn, pos); return;
            case rs2_ecall:         exec_ecall(insn, pos); return;// render_ecall();

            }

    }
    
    //assert(0 && "unhandled opcode");
    
    //cout << "I'm getting this far" << endl;
    exec_illegal_insn(insn, &std::cout);

}

/**
 * execute illegal instruction
 * 
 * handles executing illegal instructions. If a ostream input is given render the
 * instruction
 *
 ********************************************************************************/
void rv32i::exec_illegal_insn(uint32_t insn, std::ostream* pos) {

    halt = true; //halt the program
    if (insn == 0) {

    }

    if (pos != nullptr) {

        render_illegal_insn();

    }

}

/**
 * execute ecall
 * 
 * renders the 'ecall instruction'
 * 
 * @param insn an instruction
 * @param pos  a potential ostream operator
 *
 ********************************************************************************/
void rv32i::exec_ecall(uint32_t insn, std::ostream* pos) {

    uint32_t temp = insn;
    temp++;

    if (pos) {

        string s = render_ecall();
        s.resize(instruction_width, ' ');
        *pos << s << "// ecall" << endl;

    }

    pc += 4;

}

/**
 * execute ebreak
 * 
 * renders the 'ebreak instruction'. Halts the program.
 * 
 * @param insn an instruction
 * @param pos  a potential ostream operator
 *
 ********************************************************************************/
void rv32i::exec_ebreak(uint32_t insn, std::ostream* pos) {

    uint32_t temp = insn;
    temp++;

    if (pos) {

        string s = render_ebreak();
        s.resize(instruction_width, ' ');
        *pos << s << "// HALT" << endl;

    }
    halt = true;
    cout << "Execution terminated by EBREAK instruction" << endl;

}

/**
 * execute lui
 * 
 * renders the 'lui instruction'. Sets the register rd to immu.
 * 
 * @param insn an instruction
 * @param pos  a potential ostream operator
 *
 ********************************************************************************/
void rv32i::exec_lui(uint32_t insn, std::ostream* pos) {

    uint32_t rd = get_rd(insn);
    uint32_t immu = (get_imm_u(insn) << 12); //immu needs to be shifted back over 

    regs.set(rd, immu); //set the register rd to immu

    if (pos) {

        string s = render_lui(insn);
        s.resize(instruction_width, ' ');
        *pos << s << "// " << "x" << rd << " = " << hex0x32(regs.get(rd)) << endl;  

    }

    pc += 4;

}

/**
 * execute auipc
 * 
 * renders the 'auipc instruction'. Add the adress of the instruction and the auipc
 * together then store that.
 * 
 * @param insn an instruction
 * @param pos  a potential ostream operator
 *
 ********************************************************************************/
void rv32i::exec_auipc(uint32_t insn, std::ostream* pos) {

    uint32_t rd = get_rd(insn);
    uint32_t immu = (get_imm_u(insn) << 12); //Immu needs to be shifted back over

    int32_t tempreg = pc;

    tempreg += immu; //add immu to temp reg
    

    if (pos) {

        string s = render_auipc(insn);
        s.resize(instruction_width, ' ');
        *pos << s << "// " << "x" << rd << " = " << hex0x32(pc) << " + " << hex0x32(immu);
        regs.set(rd, tempreg); //set rd to the result
        *pos << " = " << hex0x32(regs.get(rd)) << endl;

    } else {

        regs.set(rd, tempreg); //set rd to the result

    }

    pc += 4;

}

/**
 * execute ecall
 * 
 * renders the ' instruction'
 * 
 * @param insn an instruction
 * @param pos  a potential ostream operator
 *
 ********************************************************************************/
void rv32i::exec_jal(uint32_t insn, std::ostream* pos) {

    uint32_t rd = get_rd(insn);
    uint32_t pc_rel21 = (get_imm_j(insn) + pc);

    regs.set(rd, pc+4); //set register rd + 4

    if (pos) {

        string s = render_jal(insn, pc);
        s.resize(instruction_width, ' ');
        *pos << s << "// x" << rd << " = " << hex0x32(regs.get(rd)) << ",  pc = " << hex0x32(pc) << " + " << hex0x32(get_imm_j(insn)) << " = " << hex0x32(pc_rel21) << endl;

    }

    pc = pc_rel21;

}

/**
 * execute jalr
 * 
 * renders the 'jalr instruction'
 * 
 * Sets the pc reg to the instruction + 4. Then jumps to immi.
 * 
 * @param insn an instruction
 * @param pos  a potential ostream operator
 *
 ********************************************************************************/
void rv32i::exec_jalr(uint32_t insn, std::ostream* pos) {

    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t immi = get_imm_i(insn);
    uint32_t temp32 = regs.get(rs1);
    temp32 += immi;
    temp32 = (temp32 & 0xfffffffe);

    regs.set(rd, pc+4); //set register rd + 4
    pc = temp32;

    if (pos) {

        string s = render_jalr(insn, immi);
        s.resize(instruction_width, ' ');
   
        *pos << s << "// x" << rd << " = " << hex0x32(regs.get(rd)) << ",  pc = (" << hex0x32(immi) << " + " << hex0x32(regs.get(rs1)) << ") & 0xfffffffe = " << hex0x32(pc) << endl;

    }

}

/**
 * execute bne
 * 
 * renders the 'bne instruction'
 * 
 * branch if rs1 and rs2 are not equal
 * 
 * @param insn an instruction
 * @param pos  a potential ostream operator
 *
 ********************************************************************************/
void rv32i::exec_bne(uint32_t insn, std::ostream* pos) {

    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    uint32_t immb = get_imm_b(insn);
    string s = render_btype(insn, "bne", pc);

    if (regs.get(rs1) != regs.get(rs2)) {

        pc += immb;

    } else {

        pc += 4;

    }

    if (pos) {

        
        s.resize(instruction_width, ' ');

        *pos << s << "// pc += (" << hex0x32(regs.get(rs1)) << " != " << hex0x32(regs.get(rs2)) << " ? " << hex0x32(immb) << " : 4) = " << hex0x32(pc) << endl;

    }

}

/**
 * execute blt
 * 
 * renders the 'blt instruction'
 * 
 * branches if rs1 is < rs2
 * 
 * @param insn an instruction
 * @param pos  a potential ostream operator
 *
 ********************************************************************************/
void rv32i::exec_blt(uint32_t insn, std::ostream* pos) {

    int32_t rs1 = regs.get(get_rs1(insn));
    int32_t rs2 = regs.get(get_rs2(insn));
    uint32_t immb = get_imm_b(insn);
    string s = render_btype(insn, "blt", pc);

    if (rs1 < rs2) {

        pc += immb;

    } else {

        pc += 4;

    }

    if (pos) {

        s.resize(instruction_width, ' ');

        *pos << s << "// pc += (" << hex0x32(rs1) << " < " << hex0x32(rs2) << " ? " << hex0x32(immb) << " : 4) = " << hex0x32(pc) << endl;

    }

}

/**
 * execute bge
 * 
 * renders the 'bge instruction'
 * 
 * branches if rs1 is >= rs2
 * 
 * @param insn an instruction
 * @param pos  a potential ostream operator
 *
 ********************************************************************************/
void rv32i::exec_bge(uint32_t insn, std::ostream* pos) {

    int32_t rs1 = regs.get(get_rs1(insn));
    int32_t rs2 = regs.get(get_rs2(insn));
    uint32_t immb = get_imm_b(insn);
    string s = render_btype(insn, "bge", pc);

    if (rs1 >= rs2) {

        pc += immb;

    } else {

        pc += 4;

    }

    if (pos) {

        s.resize(instruction_width, ' ');

        *pos << s << "// pc += (" << hex0x32(rs1) << " >= " << hex0x32(rs2) << " ? " << hex0x32(immb) << " : 4) = " << hex0x32(pc) << endl;

    }

}

/**
 * execute bltu
 * 
 * renders the 'bltu instruction'. Branches if unsigned rs1 < rs2.
 * 
 * @param insn an instruction
 * @param pos  a potential ostream operator
 *
 ********************************************************************************/
void rv32i::exec_bltu(uint32_t insn, std::ostream* pos) {

    uint32_t rs1 = regs.get(get_rs1(insn));
    uint32_t rs2 = regs.get(get_rs2(insn));
    uint32_t immb = get_imm_b(insn);
    string s = render_btype(insn, "bltu", pc);

    if (rs1 < rs2) {

        pc += immb;

    } else {

        pc += 4;

    }

    if (pos) {

        s.resize(instruction_width, ' ');

        *pos << s << "// pc += (" << hex0x32(rs1) << " <U " << hex0x32(rs2) << " ? " << hex0x32(immb) << " : 4) = " << hex0x32(pc) << endl;

    }

}

/**
 * execute bgeu
 * 
 * renders the 'bgeu instruction'. Branches if unsigned rs1 >= rs2.
 * 
 * @param insn an instruction
 * @param pos  a potential ostream operator
 *
 ********************************************************************************/
void rv32i::exec_bgeu(uint32_t insn, std::ostream* pos) {

    uint32_t rs1 = regs.get(get_rs1(insn));
    uint32_t rs2 = regs.get(get_rs2(insn));
    uint32_t immb = get_imm_b(insn);
    string s = render_btype(insn, "bgeu", pc);

    if (rs1 >= rs2) {

        pc += immb;

    } else {

        pc += 4;

    }

    if (pos) {

        s.resize(instruction_width, ' ');

        *pos << s << "// pc += (" << hex0x32(rs1) << " >=U " << hex0x32(rs2) << " ? " << hex0x32(immb) << " : 4) = " << hex0x32(pc) << endl;

    }

}

/**
 * execute beq
 * 
 * renders the 'beq instruction'. branches if rs1 is == to rs2
 * 
 * @param insn an instruction
 * @param pos  a potential ostream operator
 *
 ********************************************************************************/
void rv32i::exec_beq(uint32_t insn, std::ostream* pos) {

    int32_t rs1 = regs.get(get_rs1(insn));
    int32_t rs2 = regs.get(get_rs2(insn));
    uint32_t immb = get_imm_b(insn);
    string s = render_btype(insn, "beq", pc);

    if (rs1 == rs2) {

        pc += immb;

    } else {

        pc += 4;

    }

    if (pos) {

        s.resize(instruction_width, ' ');

        *pos << s << "// pc += (" << hex0x32(rs1) << " == " << hex0x32(rs2) << " ? " << hex0x32(immb) << " : 4) = " << hex0x32(pc) << endl;

    }

}

/**
 * execute addi
 * 
 * renders the 'addi instruction'. Set register rd to rs1 + immi.
 * 
 * @param insn an instruction
 * @param pos  a potential ostream operator
 *
 ********************************************************************************/
void rv32i::exec_addi(uint32_t insn, std::ostream* pos) {

    uint32_t rd = get_rd(insn);
    uint32_t rs1 = regs.get(get_rs1(insn));
    uint32_t immi = get_imm_i(insn);

    regs.set(rd, (rs1 + immi));

    if (pos) {

        string s = render_itype_alu(insn, "addi", immi);
        s.resize(instruction_width, ' ');
        *pos << s << "// x" << rd << " = " << hex0x32(rs1) << " + " << hex0x32(immi) << " = " << hex0x32(regs.get(rd)) << endl;

    }

    pc += 4;

}

/**
 * execute lbu
 * 
 * renders the 'lbu instruction'. Set rd = to zero extended val at rs1 + immi.
 * 
 * @param insn an instruction
 * @param pos  a potential ostream operator
 *
 ********************************************************************************/
void rv32i::exec_lbu(uint32_t insn, std::ostream* pos) {

    uint32_t rd = get_rd(insn);
    uint32_t rs1 = regs.get(get_rs1(insn));
    uint32_t immi = get_imm_i(insn);

    uint32_t temp32 = rs1 + immi;
    uint32_t temp_byte = mem->get8(temp32);

    //temp_byte = (temp_byte >> 20);
    //temp_byte = (temp_byte & 0x000000ff);
    //zero extended

    regs.set(rd, temp_byte);

    if (pos) {

        string s = render_itype_load(insn, "lbu", immi);
        s.resize(instruction_width, ' ');
        *pos << s << "// x" << rd << " = zx(m8(" << hex0x32(rs1) << " + " << hex0x32(immi) << ")) = " << hex0x32(regs.get(rd)) << endl;

    }

    pc += 4;

}

/**
 * execute lhu
 * 
 * renders the 'lhu instruction'. like lbu but loads a halfword.
 * 
 * @param insn an instruction
 * @param pos  a potential ostream operator
 *
 ********************************************************************************/
void rv32i::exec_lhu(uint32_t insn, std::ostream* pos) {

    uint32_t rd = get_rd(insn);
    uint32_t rs1 = regs.get(get_rs1(insn));
    uint32_t immi = get_imm_i(insn);

    uint32_t temp32 = rs1+ immi;
    uint32_t temp_byte = mem->get16(temp32);

    regs.set(rd, temp_byte);

    if (pos) {

        string s = render_itype_load(insn, "lhu", immi);
        s.resize(instruction_width, ' ');
        *pos << s << "// x" << rd << " = zx(m16(" << hex0x32(rs1) << " + " << hex0x32(immi) << ")) = " << hex0x32(regs.get(rd)) << endl;

    }

    pc += 4;

}

/**
 * execute lb
 * 
 * renders the 'lb instruction'. Like lbu but uses a sign extended byte.
 * 
 * @param insn an instruction
 * @param pos  a potential ostream operator
 *
 ********************************************************************************/
void rv32i::exec_lb(uint32_t insn, std::ostream* pos) {

    uint32_t rd = get_rd(insn);
    uint32_t rs1 = regs.get(get_rs1(insn));
    uint32_t immi = get_imm_i(insn);

    uint32_t temp32 = rs1 + immi;
    uint32_t temp_byte = mem->get8(temp32);

    if ((temp_byte & 0x00000080) == 0x00000080) {

        temp_byte = (temp_byte | 0xffffff00); // sign extend

    } else {

        temp_byte = (temp_byte & 0x000000ff); // sign extend

    }

    regs.set(rd, temp_byte);

    if (pos) {

        string s = render_itype_load(insn, "lb", immi);
        s.resize(instruction_width, ' ');
        *pos << s << "// x" << rd << " = sx(m8(" << hex0x32(rs1) << " + " << hex0x32(immi) << ")) = " << hex0x32(regs.get(rd)) << endl;

    }

    pc += 4;

}

/**
 * execute lh
 * 
 * renders the 'lh instruction'. Like lhu but loads a sign extended halfword.
 * 
 * @param insn an instruction
 * @param pos  a potential ostream operator
 *
 ********************************************************************************/
void rv32i::exec_lh(uint32_t insn, std::ostream* pos) {

    uint32_t rd = get_rd(insn);
    uint32_t rs1 = regs.get(get_rs1(insn));
    uint32_t immi = get_imm_i(insn);

    uint32_t temp32 = rs1+ immi;
    uint32_t temp_byte = mem->get16(temp32);

    if ((temp_byte & 0x00008000) == 0x00008000) {

        temp_byte = (temp_byte | 0xffff0000); // sign extend

    } else {

        temp_byte = (temp_byte & 0x0000ffff); // sign extend

    }

    regs.set(rd, temp_byte);

    if (pos) {

        string s = render_itype_load(insn, "lh", immi);
        s.resize(instruction_width, ' ');
        *pos << s << "// x" << rd << " = sx(m16(" << hex0x32(rs1) << " + " << hex0x32(immi) << ")) = " << hex0x32(regs.get(rd)) << endl;

    }

    pc += 4;

}

/**
 * execute lw
 * 
 * renders the 'lw instruction'. Loads a fullword into the rs1 + immi addr.
 * 
 * @param insn an instruction
 * @param pos  a potential ostream operator
 *
 ********************************************************************************/
void rv32i::exec_lw(uint32_t insn, std::ostream* pos) {

    uint32_t rd = get_rd(insn);
    uint32_t rs1 = regs.get(get_rs1(insn));
    uint32_t immi = get_imm_i(insn);

    uint32_t temp32 = rs1 + immi;
    uint32_t temp_byte = mem->get32(temp32);

    regs.set(rd, temp_byte);

    if (pos) {

        string s = render_itype_load(insn, "lw", immi);
        s.resize(instruction_width, ' ');
        *pos << s << "// x" << rd << " = sx(m32(" << hex0x32(rs1) << " + " << hex0x32(immi) << ")) = " << hex0x32(regs.get(rd)) << endl;

    }

    pc += 4;

}

/**
 * execute sb
 * 
 * renders the 'sb instruction'. Sets a byte at rs1 + imms.
 * 
 * @param insn an instruction
 * @param pos  a potential ostream operator
 *
 ********************************************************************************/
void rv32i::exec_sb(uint32_t insn, std::ostream* pos) {

    uint32_t rs1 = regs.get(get_rs1(insn));
    uint32_t rs2 = regs.get(get_rs2(insn));
    uint32_t imms = get_imm_s(insn);

    uint32_t temp32 = rs1 + imms;
    uint32_t temp_byte = (rs2 & 0x000000ff);

    mem->set8(temp32, temp_byte); // set the byte at the sum address

    if (pos) {

        string s = render_stype(insn, "sb");
        s.resize(instruction_width, ' ');
        *pos << s << "// m8(" << hex0x32(rs1) << " + " << hex0x32(imms) << ") = " << hex0x32(rs2 & 0x000000ff) << endl;
    
    }

    pc += 4;

}

/**
 * execute sh
 * 
 * renders the 'sh instruction'. Sets a halfword at rs1 + imms.
 * 
 * @param insn an instruction
 * @param pos  a potential ostream operator
 *
 ********************************************************************************/
void rv32i::exec_sh(uint32_t insn, std::ostream* pos) {

    uint32_t rs1 = regs.get(get_rs1(insn));
    uint32_t rs2 = regs.get(get_rs2(insn));
    uint32_t imms = get_imm_s(insn);

    uint32_t temp32 = rs1 + imms;
    uint32_t temp_byte = (rs2 & 0x0000ffff);

    mem->set16(temp32, temp_byte); // set the byte at the sum address

    if (pos) {

        string s = render_stype(insn, "sh");
        s.resize(instruction_width, ' ');
        *pos << s << "// m16(" << hex0x32(rs1) << " + " << hex0x32(imms) << ") = " << hex0x32(rs2 & 0x0000ffff) << endl;
    
    }

    pc += 4;

}

/**
 * execute sw
 * 
 * renders the 'sw instruction'. Sets the fullword at rs1 + imms.
 * 
 * @param insn an instruction
 * @param pos  a potential ostream operator
 *
 ********************************************************************************/
void rv32i::exec_sw(uint32_t insn, std::ostream* pos) {

    uint32_t rs1 = regs.get(get_rs1(insn));
    uint32_t rs2 = regs.get(get_rs2(insn));
    uint32_t imms = get_imm_s(insn);

    uint32_t temp32 = rs1 + imms;
    uint32_t temp_byte = (rs2);

    mem->set32(temp32, temp_byte); // set the byte at the sum address

    if (pos) {

        string s = render_stype(insn, "sw");
        s.resize(instruction_width, ' ');
        *pos << s << "// m32(" << hex0x32(rs1) << " + " << hex0x32(imms) << ") = " << hex0x32(rs2) << endl;
    
    }

    pc += 4;

}

/**
 * execute slti
 * 
 * renders the 'slti instruction'. If rs1 < immi sets rd to 1 if not to 0.
 * 
 * @param insn an instruction
 * @param pos  a potential ostream operator
 *
 ********************************************************************************/
void rv32i::exec_slti(uint32_t insn, std::ostream* pos) {

    uint32_t rd = get_rd(insn);
    int32_t rs1 = regs.get(get_rs1(insn));
    int32_t immi = get_imm_i(insn);

    if (rs1 < immi) {

        regs.set(rd, 1);

    } else {

        regs.set(rd, 0);

    }

    if (pos) {

        string s = render_itype_alu(insn, "slti", immi);
        s.resize(instruction_width, ' ');
        *pos << s << "// x" << rd << " = (" << hex0x32(rs1) << " < " << immi << ") ? 1 : 0 = " << hex0x32(regs.get(rd)) << endl; 

    }

    pc += 4;

}

/**
 * execute sltiu
 * 
 * renders the 'sltiu instruction'. Same as slti but uses uints.
 * 
 * @param insn an instruction
 * @param pos  a potential ostream operator
 *
 ********************************************************************************/
void rv32i::exec_sltiu(uint32_t insn, std::ostream* pos) {

    uint32_t rd = get_rd(insn);
    uint32_t rs1 = regs.get(get_rs1(insn));
    uint32_t immi = get_imm_i(insn);

    if (rs1 < immi) {

        regs.set(rd, 1);

    } else {

        regs.set(rd, 0);

    }

    if (pos) {

        string s = render_itype_alu(insn, "sltiu", immi);
        s.resize(instruction_width, ' ');
        *pos << s << "// x" << rd << " = (" << hex0x32(rs1) << " <U " << immi << ") ? 1 : 0 = " << hex0x32(regs.get(rd)) << endl; 

    }

    pc += 4;

}

/**
 * execute xori
 * 
 * renders the 'xori instruction'. Set rd to ^ of rs1 and immi.
 * 
 * @param insn an instruction
 * @param pos  a potential ostream operator
 *
 ********************************************************************************/
void rv32i::exec_xori(uint32_t insn, std::ostream* pos) {

    uint32_t rd = get_rd(insn);
    uint32_t rs1 = regs.get(get_rs1(insn));
    uint32_t immi = get_imm_i(insn);

    regs.set(rd, (rs1 ^ immi));
    //double check these on multiple answers

    if (pos) {

        string s = render_itype_alu(insn, "xori", immi);
        s.resize(instruction_width, ' ');
        *pos << s << "// x" << rd << " = " << hex0x32(rs1) << " ^ " << hex0x32(immi) << " = " << hex0x32(regs.get(rd)) << endl;

    }

    pc += 4;

}

/**
 * execute ori
 * 
 * renders the 'ori instruction'. Sets rd to rs1 | immi.
 * 
 * @param insn an instruction
 * @param pos  a potential ostream operator
 *
 ********************************************************************************/
void rv32i::exec_ori(uint32_t insn, std::ostream* pos) {

    uint32_t rd = get_rd(insn);
    uint32_t rs1 = regs.get(get_rs1(insn));
    uint32_t immi = get_imm_i(insn);

    regs.set(rd, (rs1 | immi));
    //double check these on multiple answers

    if (pos) {

        string s = render_itype_alu(insn, "ori", immi);
        s.resize(instruction_width, ' ');
        *pos << s << "// x" << rd << " = " << hex0x32(rs1) << " | " << hex0x32(immi) << " = " << hex0x32(regs.get(rd)) << endl;

    }

    pc += 4;
    
}

/**
 * execute andi
 * 
 * renders the 'andi instruction'. Sets rd to rs1 & immi.
 * 
 * @param insn an instruction
 * @param pos  a potential ostream operator
 *
 ********************************************************************************/
void rv32i::exec_andi(uint32_t insn, std::ostream* pos) {

    uint32_t rd = get_rd(insn);
    uint32_t rs1 = regs.get(get_rs1(insn));
    uint32_t immi = get_imm_i(insn);

    regs.set(rd, (rs1 & immi));
    //double check these on multiple answers

    if (pos) {

        string s = render_itype_alu(insn, "andi", immi);
        s.resize(instruction_width, ' ');
        *pos << s << "// x" << rd << " = " << hex0x32(rs1) << " & " << hex0x32(immi) << " = " << hex0x32(regs.get(rd)) << endl;

    }

    pc += 4;

}

/**
 * execute slli
 * 
 * renders the 'slli instruction'. Shift rs1 << by immi and store at rd.
 * 
 * @param insn an instruction
 * @param pos  a potential ostream operator
 *
 ********************************************************************************/
void rv32i::exec_slli(uint32_t insn, std::ostream* pos) {

    uint32_t rd = get_rd(insn);
    uint32_t rs1 = regs.get(get_rs1(insn));
    uint32_t immi = get_imm_i(insn);

    regs.set(rd, (rs1 << immi));

    if (pos) {

        string s = render_itype_alu(insn, "slli", immi);
        s.resize(instruction_width, ' ');
        *pos << s << "// x" << rd << " = " << hex0x32(rs1) << " << " << immi << " = " << hex0x32(regs.get(rd)) << endl;

    }

    pc += 4;

}

/**
 * execute srli
 * 
 * renders the 'srli instruction'. Shift rs1 >> logically by immi store at rd.
 * 
 * @param insn an instruction
 * @param pos  a potential ostream operator
 *
 ********************************************************************************/
void rv32i::exec_srli(uint32_t insn, std::ostream* pos) {

    uint32_t rd = get_rd(insn);
    uint32_t rs1 = regs.get(get_rs1(insn));
    uint32_t immi = get_imm_i(insn);

    regs.set(rd, (rs1 >> immi));

    if (pos) {

        string s = render_itype_alu(insn, "srli", immi);
        s.resize(instruction_width, ' ');
        *pos << s << "// x" << rd << " = " << hex0x32(rs1) << " >> " << immi << " = " << hex0x32(regs.get(rd)) << endl;

    }

    pc += 4;

}

/**
 * execute srai
 * 
 * renders the 'srai instruction'. Shift rs1 >> arithmatically by immi store at rd.
 * 
 * @param insn an instruction
 * @param pos  a potential ostream operator
 *
 ********************************************************************************/
void rv32i::exec_srai(uint32_t insn, std::ostream* pos) {

    uint32_t rd = get_rd(insn);
    uint32_t rs1 = regs.get(get_rs1(insn));
    uint32_t immi = get_imm_i(insn);

    if ((rs1 & 0x80000000) == 0x80000000) {

        regs.set(rd, ((int32_t)rs1 >> immi)); //cast it to signed for arithmatic shift

    } else {

        regs.set(rd, (rs1 >> immi));

    }

    if (pos) {

        string s = render_itype_alu(insn, "srai", immi%XLEN);
        s.resize(instruction_width, ' ');
        *pos << s << "// x" << rd << " = " << hex0x32(rs1) << " >> " << immi%XLEN << " = " << hex0x32(regs.get(rd)) << endl;

    }

    pc += 4;
    //Check this function for errors later

}

/**
 * execute add
 * 
 * renders the 'add instruction'. add rs1 to rs2 store at rd.
 * 
 * @param insn an instruction
 * @param pos  a potential ostream operator
 *
 ********************************************************************************/
void rv32i::exec_add(uint32_t insn, std::ostream* pos) {

    uint32_t rd = get_rd(insn);
    uint32_t rs1 = regs.get(get_rs1(insn));
    uint32_t rs2 = regs.get(get_rs2(insn));

    regs.set(rd, rs1 + rs2);

    if (pos) {

        string s = render_rtype(insn, "add");
        s.resize(instruction_width, ' ');
        *pos << s << "// x" << rd << " = " << hex0x32(rs1) << " + " << hex0x32(rs2) << " = " << hex0x32(regs.get(rd)) << endl;

    }

    pc += 4;

}

/**
 * execute sub
 * 
 * renders the 'sub instruction'. Subtract rs1 from rs2 store at rd.
 * 
 * @param insn an instruction
 * @param pos  a potential ostream operator
 *
 ********************************************************************************/
void rv32i::exec_sub(uint32_t insn, std::ostream* pos) {

    uint32_t rd = get_rd(insn);
    uint32_t rs1 = regs.get(get_rs1(insn));
    uint32_t rs2 = regs.get(get_rs2(insn));

    regs.set(rd, rs1 - rs2);

    if (pos) {

        string s = render_rtype(insn, "sub");
        s.resize(instruction_width, ' ');
        *pos << s << "// x" << rd << " = " << hex0x32(rs1) << " - " << hex0x32(rs2) << " = " << hex0x32(regs.get(rd)) << endl;

    }

    pc += 4;

}

/**
 * execute sll
 * 
 * renders the 'sll instruction'. Shift rs1 << logically by the last 5 bits of rs2.
 * Store at rd.
 * 
 * @param insn an instruction
 * @param pos  a potential ostream operator
 *
 ********************************************************************************/
void rv32i::exec_sll(uint32_t insn, std::ostream* pos) {

    uint32_t rd = get_rd(insn);
    uint32_t rs1 = regs.get(get_rs1(insn));
    uint32_t rs2 = regs.get(get_rs2(insn));

    uint32_t temp32 = (rs2 & 0x0000001f);

    regs.set(rd, (rs1 << temp32));

    if (pos) {

        string s = render_rtype(insn, "sll");
        s.resize(instruction_width, ' ');
        *pos << s << "// x" << rd << " = " << hex0x32(rs1) << " << " << temp32 << " = " << hex0x32(regs.get(rd)) << endl;

    }

    pc += 4;

}

/**
 * execute slt
 * 
 * renders the 'slt instruction'. If signed rs1 < rs2 put 1 into rd else put 0.
 * 
 * @param insn an instruction
 * @param pos  a potential ostream operator
 *
 ********************************************************************************/
void rv32i::exec_slt(uint32_t insn, std::ostream* pos) {

    uint32_t rd = get_rd(insn);
    int32_t rs1 = regs.get(get_rs1(insn));
    int32_t rs2 = regs.get(get_rs2(insn));

    if (rs1 < rs2) {

        regs.set(rd, 1);

    } else {

        regs.set(rd, 0);

    }

    if (pos) {

        string s = render_rtype(insn, "slt");
        s.resize(instruction_width, ' ');
        *pos << s << "// x" << rd << " = (" << hex0x32(rs1) << " < " << hex0x32(rs2) << ") ? 1 : 0 = " << hex0x32(regs.get(rd)) << endl;

    }

    pc += 4;
    
}

/**
 * execute sltu
 * 
 * renders the 'sltu instruction'. Same as slt but unsigned.
 * 
 * @param insn an instruction
 * @param pos  a potential ostream operator
 *
 ********************************************************************************/
void rv32i::exec_sltu(uint32_t insn, std::ostream* pos) {

    uint32_t rd = get_rd(insn);
    uint32_t rs1 = regs.get(get_rs1(insn));
    uint32_t rs2 = regs.get(get_rs2(insn));

    if (rs1 < rs2) {

        regs.set(rd, 1);

    } else {

        regs.set(rd, 0);

    }

    if (pos) {

        string s = render_rtype(insn, "sltu");
        s.resize(instruction_width, ' ');
        *pos << s << "// x" << rd << " = (" << hex0x32(rs1) << " <U " << hex0x32(rs2) << ") ? 1 : 0 = " << hex0x32(regs.get(rd)) << endl;

    }

    pc += 4;
    
}

/**
 * execute xor
 * 
 * renders the 'xor instruction'. Xor rs1 and rs2 store at rd.
 * 
 * @param insn an instruction
 * @param pos  a potential ostream operator
 *
 ********************************************************************************/
void rv32i::exec_xor(uint32_t insn, std::ostream* pos) {

    uint32_t rd = get_rd(insn);
    uint32_t rs1 = regs.get(get_rs1(insn));
    uint32_t rs2 = regs.get(get_rs2(insn));

    regs.set(rd, (rs1 ^ rs2)); // set to bitwise xor

    if (pos) {

        string s = render_rtype(insn, "xor");
        s.resize(instruction_width, ' ');
        *pos << s << "// x" << rd << " = " << hex0x32(rs1) << " ^ " << hex0x32(rs2) << " = " << hex0x32(regs.get(rd)) << endl;

    }

    pc += 4;

}

/**
 * execute or
 * 
 * renders the 'or instruction'. Or rs1 and rs2 store at rd.
 * 
 * @param insn an instruction
 * @param pos  a potential ostream operator
 *
 ********************************************************************************/
void rv32i::exec_or(uint32_t insn, std::ostream* pos) {

    uint32_t rd = get_rd(insn);
    uint32_t rs1 = regs.get(get_rs1(insn));
    uint32_t rs2 = regs.get(get_rs2(insn));

    regs.set(rd, (rs1 | rs2)); // set to bitwise xor

    if (pos) {

        string s = render_rtype(insn, "or");
        s.resize(instruction_width, ' ');
        *pos << s << "// x" << rd << " = " << hex0x32(rs1) << " | " << hex0x32(rs2) << " = " << hex0x32(regs.get(rd)) << endl;

    }

    pc += 4;

}

/**
 * execute and
 * 
 * renders the 'and instruction'. And rs1 and rs2 store at rd.
 * 
 * @param insn an instruction
 * @param pos  a potential ostream operator
 *
 ********************************************************************************/
void rv32i::exec_and(uint32_t insn, std::ostream* pos) {

    uint32_t rd = get_rd(insn);
    uint32_t rs1 = regs.get(get_rs1(insn));
    uint32_t rs2 = regs.get(get_rs2(insn));

    regs.set(rd, (rs1 & rs2)); // set to bitwise xor

    if (pos) {

        string s = render_rtype(insn, "and");
        s.resize(instruction_width, ' ');
        *pos << s << "// x" << rd << " = " << hex0x32(rs1) << " & " << hex0x32(rs2) << " = " << hex0x32(regs.get(rd)) << endl;

    }

    pc += 4;

}

/**
 * execute srl
 * 
 * renders the 'srl instruction'. Shift rs1 >> logically by rs2 store at rd.
 * 
 * @param insn an instruction
 * @param pos  a potential ostream operator
 *
 ********************************************************************************/
void rv32i::exec_srl(uint32_t insn, std::ostream* pos) {

    uint32_t rd = get_rd(insn);
    uint32_t rs1 = regs.get(get_rs1(insn));
    uint32_t rs2 = regs.get(get_rs2(insn));

    regs.set(rd, (rs1 >> rs2));

    if (pos) {

        string s = render_rtype(insn, "srl");
        s.resize(instruction_width, ' ');
        *pos << s << "// x" << rd << " = " << hex0x32(rs1) << " >> " << rs2%XLEN << " = " << hex0x32(regs.get(rd)) << endl;

    }

    pc += 4;

}

/**
 * execute sra
 * 
 * renders the 'sra instruction'. Same as srl but shifts arithmatically.
 * 
 * @param insn an instruction
 * @param pos  a potential ostream operator
 *
 ********************************************************************************/
void rv32i::exec_sra(uint32_t insn, std::ostream* pos) {

    uint32_t rd = get_rd(insn);
    uint32_t rs1 = regs.get(get_rs1(insn));
    uint32_t rs2 = regs.get(get_rs2(insn));

    if ((rs1 & 0x80000000) == 0x80000000) {

        regs.set(rd, ((int32_t)rs1 >> rs2)); //if it's cast to signed it will arithmatic shift

    } else {

        regs.set(rd, (rs1 >> rs2));

    }
    

    if (pos) {

        string s = render_rtype(insn, "sra");
        s.resize(instruction_width, ' ');
        *pos << s << "// x" << rd << " = " << hex0x32(rs1) << " >> " << rs2%XLEN << " = " << hex0x32(regs.get(rd)) << endl;

    }

    pc += 4;

}

/**
 * execute fence
 * 
 * renders the 'fence instruction'.
 * 
 * @param insn an instruction
 * @param pos  a potential ostream operator
 *
 ********************************************************************************/
void rv32i::exec_fence(uint32_t insn, std::ostream* pos) {

    if (pos) {

        string s = render_fence(insn);
        s.resize(instruction_width, ' ');
        *pos << s << "// fence" << endl;

    }

    pc += 4;

}


/**
 * Decode function
 * 
 * this does the heavy lifting for the program so far. It goes through a logic tree
 * of switch statements. It first checks for an opcode and then for whatever
 * distinguishes the individual function from others of the same type.
 * It then calls the proper render function and feeds it the hex instruction
 *
 * @param insn an unsigned int that represents a rv32i instruction / an attempt at one
 *
 * @return It returns a string formatted with the decoded instructions information to be printed.
 ********************************************************************************/
string rv32i::decode(uint32_t insn) const {

    uint32_t opcode = get_opcode(insn);
    uint32_t funct3 = get_funct3(insn);
    uint32_t funct7 = get_funct7(insn);
    int32_t imm_i = get_imm_i(insn);
    uint32_t rs2 = get_rs2(insn);

    switch(opcode)
    {
    //Some of the opcode individual cases
    default:                    return render_illegal_insn();
    case opcode_lui:            return render_lui(insn);
    case opcode_auipc:          return render_auipc(insn);
    case opcode_jal:            return render_jal(insn, pc);
    case opcode_jalr:           return render_jalr(insn, imm_i);
    case opcode_fence:          return render_fence(insn);
    case opcode_rtype:
        //rtype instructions
        switch(funct3)
        {
        default:                return render_illegal_insn();
        case funct3_add:
            switch(funct7)
            {
            default:            return render_illegal_insn();
            case funct7_add:    return render_rtype(insn, "add");
            case funct7_sub:    return render_rtype(insn, "sub");
            }

            assert(0 && "unhandled funct7");
        case funct3_sll:        return render_rtype(insn, "sll");
        case funct3_slt:        return render_rtype(insn, "slt");
        case funct3_sltu:       return render_rtype(insn, "sltu");
        case funct3_xor:        return render_rtype(insn, "xor");
        case funct3_srx:
            switch(funct7) {

            default:            return render_illegal_insn();
            case funct7_srl:    return render_rtype(insn, "srl");
            case funct7_sra:    return render_rtype(insn, "sra");
            }

            assert(0 && "unhandled funct7");
        case funct3_or:         return render_rtype(insn, "or");
        case funct3_and:        return render_rtype(insn, "and");

        }
        
        assert(0 && "unhandled funct3");

    case opcode_alu_imm:
        //Some of the I instructions
        switch (funct3)
        {
        default:                return render_illegal_insn();
        case funct3_sll:        return render_itype_alu(insn, "slli", imm_i%XLEN);
        case funct3_srx:
            switch(funct7)
            {
            default:            return render_illegal_insn();
            case funct7_sra:    return render_itype_alu(insn, "srai", imm_i%XLEN);
            case funct7_srl:    return render_itype_alu(insn, "srli", imm_i%XLEN);
            }
            //assert(0 && "unhandled funct7");
        case funct3_add:        return render_itype_alu(insn, "addi", imm_i);
        case funct3_slti:       return render_itype_alu(insn, "slti", imm_i);
        case funct3_sltiu:      return render_itype_alu(insn, "sltiu", imm_i);
        case funct3_xori:       return render_itype_alu(insn, "xori", imm_i);
        case funct3_ori:        return render_itype_alu(insn, "ori", imm_i);
        case funct3_andi:       return render_itype_alu(insn, "andi", imm_i);
        }
        
        assert(0 && "unhandled funct3");
    case opcode_load_imm:
        //The rest of the I instructions
        switch (funct3) {

        default:                return render_illegal_insn();
        case funct3_lb:         return render_itype_load(insn, "lb", imm_i);
        case funct3_lh:         return render_itype_load(insn, "lh", imm_i);
        case funct3_lw:         return render_itype_load(insn, "lw", imm_i);
        case funct3_lbu:        return render_itype_load(insn, "lbu", imm_i);
        case funct3_lhu:        return render_itype_load(insn, "lhu", imm_i);

        }
    
    case opcode_btype:
        //B type instructions
        switch (funct3) {

        default:                return render_illegal_insn();
        case funct3_beq:        return render_btype(insn, "beq", pc);
        case funct3_bne:        return render_btype(insn, "bne", pc);
        case funct3_blt:        return render_btype(insn, "blt", pc);
        case funct3_bge:        return render_btype(insn, "bge", pc);
        case funct3_bltu:       return render_btype(insn, "bltu", pc);
        case funct3_bgeu:       return render_btype(insn, "bgeu", pc);


        }
    
    case opcode_stype:
        //S type instructions
        switch (funct3) {

        default:                return render_illegal_insn();
        case funct3_sb:         return render_stype(insn, "sb");
        case funct3_sh:         return render_stype(insn, "sh");
        case funct3_sw:         return render_stype(insn, "sw");

        }

    case opcode_e:
        //'e' instructions ebreak and ecall
        switch (rs2) {

        default:                return render_illegal_insn();
        case rs2_ebreak:        return render_ebreak();
        case rs2_ecall:         return render_ecall();

        }

    }
    //assert(0 && "unhandled opcode");
    

}

/**
 * &'s the opcode out of the given instruction
 *
 * @param insn a unsigned int 32 which contains an instruction
 *
 * @return returns an unsigned 32 with the opcode in it
 ********************************************************************************/
uint32_t rv32i::get_opcode(uint32_t insn) {

    return (insn & 0x0000007f);

}

/**
 * &'s the rd value out of the given instruction
 *
 * @param insn a unsigned int 32 which contains an instruction
 *
 * @return returns an unsigned 32 with the rd value in it
 ********************************************************************************/
uint32_t rv32i::get_rd(uint32_t insn) {

    return ((insn & 0x00000f80) >> 7); //rd is in bits 11-7

}

/**
 * &'s the immu_u out of the given instruction
 *
 * @param insn a unsigned int 32 which contains an instruction
 *
 * @return returns an unsigned 32 with the imm_u in it
 ********************************************************************************/
uint32_t rv32i::get_imm_u(uint32_t insn) {

    return ((insn & 0xfffff000) >> 12); //U type imm is in 31-12

}

/**
 * &'s the funct3 out of the given instruction
 *
 * @param insn a unsigned int 32 which contains an instruction
 *
 * @return returns an unsigned 32 with the funct3 value in it
 ********************************************************************************/
uint32_t rv32i::get_funct3(uint32_t insn) {

    return ((insn & 0x00007000) >> 12); //Funct 3 is in 14-12

}

/**
 * &'s the funct7 out of the given instruction
 *
 * @param insn a unsigned int 32 which contains an instruction
 *
 * @return returns an unsigned 32 with the funct7 value in it
 ********************************************************************************/
uint32_t rv32i::get_funct7(uint32_t insn) {

    return ((insn & 0xfe000000) >> 25); //Funct 7 is in 25-31

}

/**
 * &'s the rs1 out of the given instruction
 *
 * @param insn a unsigned int 32 which contains an instruction
 *
 * @return returns an unsigned 32 with the rs1 value in it
 ********************************************************************************/
uint32_t rv32i::get_rs1(uint32_t insn) {

    return ((insn & 0x000f8000) >> 15); //Rs1 is in 15-19

}

/**
 * &'s the rs2 out of the given instruction
 *
 * @param insn a unsigned int 32 which contains an instruction
 *
 * @return returns an unsigned 32 with the rs2 value in it
 ********************************************************************************/
uint32_t rv32i::get_rs2(uint32_t insn) {

    return ((insn & 0x01f00000) >> 20); //Rs2 is in 20-24

}

/**
 * &'s the imm_i value out of the instruction. If the first bit is a 1 it sign
 * extends the bit
 *
 * @param insn a unsigned int 32 which contains an instruction
 *
 * @return returns an signed 32 with the imm_i in it
 ********************************************************************************/
int32_t rv32i::get_imm_i(uint32_t insn) {

    //this isn't technically arithmatic shifting if there's an error come back here.
    //return ((insn & 0xfff00000) >> 20)

    int32_t immi = ((insn & 0xfff00000) >> 20);

    if ((insn & 0x80000000) == 0x80000000) {

        immi = (immi | 0xfffff000); //sign extend the 1
    }

    return immi;

}

/**
 * &'s the imm_b out of an instruction. It pulls peices out individually and then
 * puts them back together at the end with ors. It sign extends the sign bit.
 *
 * @param insn a unsigned int 32 which contains an instruction
 *
 * @return returns an unsigned 32 with the imm_b in it
 ********************************************************************************/
uint32_t rv32i::get_imm_b(uint32_t insn) {

    uint32_t extract11 = ((insn & 0x00000080) << 4); //this puts only bit 11 into the var
    uint32_t extract1to4 = ((insn & 0x00000f00) >> 7); //this extracts the first 5 bits

    uint32_t extract5to10 = ((insn & 0x7e000000) >> 20); //this should place these 6 bits into 5-10
    uint32_t extract12 = ((insn & 0x80000000) >> 19); // extracts the first bit this bit needs to be in 12 and 13

    uint32_t immb = (extract1to4 | extract5to10);
    immb = (immb | extract11);
    immb = (immb | extract12);

    if ((insn & 0x80000000) == 0x80000000) {

        immb = (immb | 0xfffff000); //if this bit is on then sign extend it.
    }

    return immb;

}

/**
 * &'s the imm_s out of an instruction. sign extends if necessary
 *
 * @param insn a unsigned int 32 which contains an instruction
 *
 * @return returns an unsigned 32 with the imm_s in it
 ********************************************************************************/
uint32_t rv32i::get_imm_s(uint32_t insn) {

    uint32_t imms = ((insn & 0x00000f80) >> 7); //extract 7-11
    uint32_t extract5to11 = ((insn & 0xfe000000) >> 20); // extract 31-25

    imms = (imms | extract5to11);

    if ((insn & 0x80000000) == 0x80000000) {

        imms = (imms | 0xfffff000); //sign extend if necessary

    }

    return imms;

}

/**
 * &'s the imm_j out of an instruction. takes peices apart and puts them back
 * together in the correct order. Sign extends the sign bit
 *
 * @param insn a unsigned int 32 which contains an instruction
 *
 * @return returns an unsigned 32 with the imm_j in it
 ********************************************************************************/
uint32_t rv32i::get_imm_j(uint32_t insn) {

    uint32_t extract1to10 = ((insn & 0x7fe00000) >>  20); //this should shift it to the 1 spot
    uint32_t extract11 = ((insn & 0x00100000) >> 9); //this should shift it to the 11th spot
    uint32_t extract12to19 = (insn & 0x000ff000); //no shift required
    uint32_t extract20 = ((insn & 0x80000000) >> 11); // shift from 31 to 20

    uint32_t immj = (extract1to10 | extract11);
    immj = (immj | extract12to19);
    immj = (immj | extract20);

    if ((insn & 0x80000000) == 0x80000000) {

        immj = (immj | 0xffe00000); //sign extend if the first bit is on.
    } else {

        immj = (immj & 0x001fffff);
    }

    return immj;

}

/**
 * &'s the pred out of the instruction
 *
 * @param insn a unsigned int 32 which contains an instruction
 *
 * @return returns an unsigned 32 with the pred shifted
 ********************************************************************************/
uint32_t rv32i::get_pred(uint32_t insn) {

    return ((insn & 0x0f000000) >> 24); //return the pred shifted 24

}

/**
 * &'s the pred out of the instruction
 *
 * @param insn a unsigned int 32 which contains an instruction
 *
 * @return returns an unsigned 32 with the succ shifted
 ********************************************************************************/
uint32_t rv32i::get_succ(uint32_t insn) {

    return ((insn & 0x00f00000) >> 20); //return the succ shifted 20

}

/**
 * essentially a switch statement placed neatly off to the side
 *
 * @param num a unsigned int 32 which contains either a pred or succ
 *
 * @return returns a string with the correct flags
 ********************************************************************************/
string rv32i::get_fence_string(uint32_t num) {

    switch(num) {

    default:                    return "Error no string found for fence";
    case fence_w:               return "w";
    case fence_r:               return "r";
    case fence_rw:              return "rw";
    case fence_o:               return "o";
    case fence_ow:              return "ow";
    case fence_or:              return "or";
    case fence_orw:             return "orw";
    case fence_i:               return "i";
    case fence_iw:              return "iw";
    case fence_ir:              return "ir";
    case fence_irw:             return "irw";
    case fence_io:              return "io";
    case fence_iow:             return "iow";
    case fence_ior:             return "ior";
    case fence_iorw:            return "iorw";



    }

}


// RENDER FUNCTIONS

/**
 *
 * @return returns a string stating that an error has occured
 ********************************************************************************/
string rv32i::render_illegal_insn() const {

    return "ERROR: UNIMPLEMENTED INSTRUCTION";

}

/**
 * returns the formatting for the decoded lui instruction
 *
 * @param insn a unsigned int 32 which contains an instruction
 *
 * @return returns a string with the decoded lui instruction
 ********************************************************************************/
std::string rv32i::render_lui(uint32_t insn) const {

    //lui rd, imm, U-type
    uint32_t rd = get_rd(insn);
    int32_t imm_u = get_imm_u(insn);

    std::ostringstream os;
    os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << "lui" 
		<< "x" << std::dec << rd << ",0x" << std::hex << imm_u;
    return os.str();

}

/**
 * returns the formatting for the decoded auipc instruction
 *
 * @param insn a unsigned int 32 which contains an instruction
 *
 * @return returns a string with the decoded auipc instruction
 ********************************************************************************/
string rv32i::render_auipc(uint32_t insn) const {

    //auipc  rd, imm, U-type
    uint32_t rd = get_rd(insn);
    int32_t imm_u = get_imm_u(insn);

    std::ostringstream os;
    os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << "auipc" 
		<< "x" << std::dec << rd << ",0x"<< std::hex << imm_u;
    return os.str();

}

/**
 * returns the formatting for the decoded r type instructions
 *
 * @param insn a unsigned int 32 which contains an instruction
 * @param mnemonic basically a string to print containing the name of the instruction
 *
 * @return returns a string with the decoded r type instruction
 ********************************************************************************/
string rv32i::render_rtype(uint32_t insn, const char *mnemonic) const {

    //add / sub rd, rs1, rs2  R-type
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);

    std::ostringstream os;
    os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << mnemonic
        << "x" << std::dec << rd << ",x" << rs1 << ",x" << rs2 << std::hex;
    return os.str();


}

/**
 * returns the formatting for the decoded alu type instructions
 *
 * @param insn a unsigned int 32 which contains an instruction
 * @param mnemonic basically a string to print containing the name of the instruction
 * @param imm_i the immediate I value of the instruction
 *
 * @return returns a string with the decoded alu type instruction
 ********************************************************************************/
string rv32i::render_itype_alu(uint32_t insn, const char *mnemonic, int32_t imm_i) const {

    // rd, rs1, imm
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    

    std::ostringstream os;
    os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << mnemonic
        << "x" << std::dec << rd << ",x" << rs1 << "," << imm_i;
    return os.str(); 

}

/**
 * returns the formatting for the decoded load type instructions
 *
 * @param insn a unsigned int 32 which contains an instruction
 * @param mnemonic basically a string to print containing the name of the instruction
 * @param imm_i the immediate I value for the function
 *
 * @return returns a string with the decoded load type instruction
 ********************************************************************************/
string rv32i::render_itype_load(uint32_t insn, const char *mnemonic, int32_t imm_i) const {

    //rd, imm, rs1
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);

    std::ostringstream os;
    os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << mnemonic
        << "x" << std::dec << rd << "," << imm_i << "(x"  << rs1 << ")";
    return os.str();

}

/**
 * returns the formatting for the decoded b type instructions
 *
 * @param insn a unsigned int 32 which contains an instruction
 * @param mnemonic basically a string to print containing the name of the instruction
 * @param pc the loctions counter of the pc
 *
 * @return returns a string with the decoded b type instruction
 ********************************************************************************/
string rv32i::render_btype(uint32_t insn, const char *mnemonic, int32_t pc) const {

    //rs1, rs2, pcrel_13

    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);

    int32_t pcrel_13 = get_imm_b(insn) + pc; //doesn't chekc if it should be added assumes.

    std::ostringstream os;
    os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << mnemonic
        << "x" << std::dec << rs1 << ",x" << rs2 << ",0x" << std::hex << pcrel_13;
    return os.str();
    

}

/**
 * returns the formatting for the decoded s type instructions
 *
 * @param insn a unsigned int 32 which contains an instruction
 * @param mnemonic basically a string to print containing the name of the instruction
 *
 * @return returns a string with the decoded r type instruction
 ********************************************************************************/
string rv32i::render_stype(uint32_t insn, const char *mnemonic) const {

    //rs2, imm(rs1)

    uint32_t rs2 = get_rs2(insn);
    uint32_t rs1 = get_rs1(insn);
    int32_t imms = get_imm_s(insn);

    std::ostringstream os;
    os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << mnemonic
        << "x" << std::dec << rs2 << "," << imms << "(x" << rs1 << ")";
    return os.str();

}

/**
 * returns the formatting for the decoded jal instruction
 *
 * @param insn a unsigned int 32 which contains an instruction
 * @param pc the location counter of the pc
 *
 * @return returns a string with the decoded jal instruction
 ********************************************************************************/
string rv32i::render_jal(uint32_t insn, uint32_t pc) const {

    //rd, pcrel_21
    uint32_t rd = get_rd(insn);

    uint32_t pc_rel21 = (get_imm_j(insn) + pc);

    std::ostringstream os;
    os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << "jal"
        << "x" << std::dec << rd << ",0x" << std::hex << pc_rel21;
    return os.str();

}

/**
 * returns the formatting for the decoded jalr type instructions
 *
 * @param insn a unsigned int 32 which contains an instruction
 * @param imm_i the immediate I value of the instruction
 *
 * @return returns a string with the decoded jalr type instruction
 ********************************************************************************/
string rv32i::render_jalr(uint32_t insn, int32_t imm_i) const {

    // rd,imm(rs1)
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);

    std::ostringstream os;
    os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << "jalr"
        << "x" << std::dec << rd << "," << imm_i << "(x" << rs1 << ")";
    return os.str();

}

/**
 * returns the formatting for the decoded ebreak instruction
 *
 *
 * @return returns a string with the decoded ebreak instruction
 ********************************************************************************/
string rv32i::render_ebreak() const {

    std::ostringstream os;
    os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << "ebreak";
    return os.str();

}

/**
 * returns the formatting for the decoded ecall instruction
 *
 *
 * @return returns a string with the decoded ecall instruction
 ********************************************************************************/
string rv32i::render_ecall() const {

    std::ostringstream os;
    os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << "ecall";
    return os.str();

}

/**
 * returns the formatting for the decoded fence instruction
 *
 * @param insn an unsigned 32 that contains the instruction in hex
 *
 * @return returns a string with the decoded fence instruction
 ********************************************************************************/
string rv32i::render_fence(uint32_t insn) const {

    string pred = get_fence_string(get_pred(insn));
    string succ = get_fence_string(get_succ(insn));

    std::ostringstream os;
    os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << "fence"
        << pred << "," << succ;
    return os.str();

}
