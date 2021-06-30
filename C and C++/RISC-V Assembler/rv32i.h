//***************************************************************************
//
//  rv32i.h
//  CSCI 463 Assignment 4
//
//  Created by Ian Sullivan z1830506
//
//***************************************************************************

#ifndef rv32i_H
#define rv32i_H

#include <iostream>
#include "memory.h"
#include <stdint.h>
#include "registerfile.h"

class rv32i {

    private:

        memory * mem; // Pointer to a memory object
        uint32_t pc = 0; // Program counter
        bool show_instructions = false; //show instructions ?
        bool show_registers = false; // show registers ?
        bool halt; // use to stop execution 
        registerfile regs;
        uint64_t insn_counter = 0; //instruction counter


    public:

        void set_show_insturctions(bool b);
        void set_show_registers(bool b);

        static constexpr uint32_t XLEN = 32;

        static constexpr int mnemonic_width = 8;
        static constexpr int instruction_width = 35;

        //consts for opcodes
        static constexpr uint32_t opcode_lui            = 0b0110111;
        static constexpr uint32_t opcode_auipc          = 0b0010111;
        static constexpr uint32_t opcode_rtype          = 0b0110011;
        static constexpr uint32_t opcode_jal            = 0b1101111;
        static constexpr uint32_t opcode_jalr           = 0b1100111;
        static constexpr uint32_t opcode_btype          = 0b1100011;
        static constexpr uint32_t opcode_load_imm       = 0b0000011;
        static constexpr uint32_t opcode_alu_imm        = 0b0010011;
        static constexpr uint32_t opcode_stype          = 0b0100011;
        static constexpr uint32_t opcode_fence          = 0b0001111;
        static constexpr uint32_t opcode_e              = 0b1110011;

        //consts for funct3 split by type
        static constexpr uint32_t funct3_add            = 0b0000000;
        static constexpr uint32_t funct3_sll            = 0b0000001;
        static constexpr uint32_t funct3_slt            = 0b0000010;
        static constexpr uint32_t funct3_sltu           = 0b0000011;
        static constexpr uint32_t funct3_xor            = 0b0000100;
        static constexpr uint32_t funct3_or             = 0b0000110;
        static constexpr uint32_t funct3_and            = 0b0000111;
        static constexpr uint32_t funct3_srx            = 0b0000101;

        static constexpr uint32_t funct3_slti           = 0b0000010;
        static constexpr uint32_t funct3_sltiu          = 0b0000011;
        static constexpr uint32_t funct3_xori           = 0b0000100;
        static constexpr uint32_t funct3_ori            = 0b0000110;
        static constexpr uint32_t funct3_andi           = 0b0000111;

        static constexpr uint32_t funct3_lb             = 0b0000000;
        static constexpr uint32_t funct3_lh             = 0b0000001;
        static constexpr uint32_t funct3_lw             = 0b0000010;
        static constexpr uint32_t funct3_lbu            = 0b0000100;
        static constexpr uint32_t funct3_lhu            = 0b0000101;

        static constexpr uint32_t funct3_beq            = 0b0000000;
        static constexpr uint32_t funct3_bne            = 0b0000001;
        static constexpr uint32_t funct3_blt            = 0b0000100;
        static constexpr uint32_t funct3_bge            = 0b0000101;
        static constexpr uint32_t funct3_bltu           = 0b0000110;
        static constexpr uint32_t funct3_bgeu           = 0b0000111;

        static constexpr uint32_t funct3_sb             = 0b0000000;
        static constexpr uint32_t funct3_sh             = 0b0000001;
        static constexpr uint32_t funct3_sw             = 0b0000010;

        //const for funct7
        static constexpr uint32_t funct7_add            = 0b0000000;
        static constexpr uint32_t funct7_sub            = 0b0100000;
        static constexpr uint32_t funct7_srl            = 0b0000000;
        static constexpr uint32_t funct7_sra            = 0b0100000;

        //const for estatements
        static constexpr uint32_t rs2_ebreak            = 0b0000001;
        static constexpr uint32_t rs2_ecall             = 0b0000000;

        //consts for the fence instruction
        static constexpr uint32_t fence_w               = 0b0000001;
        static constexpr uint32_t fence_r               = 0b0000010;
        static constexpr uint32_t fence_rw              = 0b0000011;
        static constexpr uint32_t fence_o               = 0b0000100;
        static constexpr uint32_t fence_ow              = 0b0000101;
        static constexpr uint32_t fence_or              = 0b0000110;
        static constexpr uint32_t fence_orw             = 0b0000111;
        static constexpr uint32_t fence_i               = 0b0001000;
        static constexpr uint32_t fence_iw              = 0b0001001;
        static constexpr uint32_t fence_ir              = 0b0001010;
        static constexpr uint32_t fence_irw             = 0b0001011;
        static constexpr uint32_t fence_io              = 0b0001100;
        static constexpr uint32_t fence_iow             = 0b0001101;
        static constexpr uint32_t fence_ior             = 0b0001110;
        static constexpr uint32_t fence_iorw            = 0b0001111;


        rv32i(memory *m); //constructor
        void disasm(void);
        void reset();
        void dump() const;
        void dcex(uint32_t insn, std::ostream* pos);
        void exec_illegal_insn(uint32_t insn, std::ostream* pos);
        void tick();
        void run(uint64_t limit);
        std::string decode (uint32_t insn) const;

        //exec functions
        void exec_ebreak(uint32_t insn, std::ostream* pos);
        void exec_lui(uint32_t insn, std::ostream* pos);
        void exec_auipc(uint32_t insn, std::ostream* pos);
        void exec_jal(uint32_t insn, std::ostream* pos);
        void exec_jalr(uint32_t insn, std::ostream* pos);
        void exec_bne(uint32_t insn, std::ostream* pos);
        void exec_blt(uint32_t insn, std::ostream* pos);
        void exec_bge(uint32_t insn, std::ostream* pos);
        void exec_bltu(uint32_t insn, std::ostream* pos);
        void exec_bgeu(uint32_t insn, std::ostream* pos);
        void exec_beq(uint32_t insn, std::ostream* pos);
        void exec_addi(uint32_t insn, std::ostream* pos);
        void exec_lbu(uint32_t insn, std::ostream* pos);
        void exec_lhu(uint32_t insn, std::ostream* pos);
        void exec_lb(uint32_t insn, std::ostream* pos);
        void exec_lh(uint32_t insn, std::ostream* pos);
        void exec_lw(uint32_t insn, std::ostream* pos);
        void exec_sb(uint32_t insn, std::ostream* pos);
        void exec_sh(uint32_t insn, std::ostream* pos);
        void exec_sw(uint32_t insn, std::ostream* pos);
        void exec_slti(uint32_t insn, std::ostream* pos);
        void exec_sltiu(uint32_t insn, std::ostream* pos);
        void exec_xori(uint32_t insn, std::ostream* pos);
        void exec_ori(uint32_t insn, std::ostream* pos);
        void exec_andi(uint32_t insn, std::ostream* pos);
        void exec_slli(uint32_t insn, std::ostream* pos);
        void exec_srli(uint32_t insn, std::ostream* pos);
        void exec_srai(uint32_t insn, std::ostream* pos);
        void exec_add(uint32_t insn, std::ostream* pos);
        void exec_sub(uint32_t insn, std::ostream* pos);
        void exec_sll(uint32_t insn, std::ostream* pos);
        void exec_slt(uint32_t insn, std::ostream* pos);
        void exec_sltu(uint32_t insn, std::ostream* pos);
        void exec_xor(uint32_t insn, std::ostream* pos);
        void exec_srl(uint32_t insn, std::ostream* pos);
        void exec_sra(uint32_t insn, std::ostream* pos);
        void exec_or(uint32_t insn, std::ostream* pos);
        void exec_and(uint32_t insn, std::ostream* pos);
        void exec_fence(uint32_t insn, std::ostream* pos);
        void exec_ecall(uint32_t insn, std::ostream* pos);


        //Get functions
        static uint32_t get_opcode(uint32_t insn);
        static uint32_t get_rd(uint32_t insn);
        static uint32_t get_rs1(uint32_t insn);
        static uint32_t get_rs2(uint32_t insn);
        static uint32_t get_funct3(uint32_t insn);
        static uint32_t get_funct7(uint32_t insn);
        static int32_t get_imm_i(uint32_t insn);
        static uint32_t get_imm_u(uint32_t insn);
        static uint32_t get_imm_b(uint32_t insn);
        static uint32_t get_imm_s(uint32_t insn);
        static uint32_t get_imm_j(uint32_t insn);
        static uint32_t get_pred(uint32_t insn);
        static uint32_t get_succ(uint32_t insn);
        static std::string get_fence_string(uint32_t num);

        //Render instructions
        std::string render_illegal_insn() const;
        std::string render_lui(uint32_t insn) const;
        std::string render_auipc(uint32_t insn) const;
        std::string render_jal(uint32_t insn, uint32_t pc) const;
        std::string render_jalr(uint32_t insn, int32_t imm_i) const;
        std::string render_btype(uint32_t insn, const char *mnemonic, int32_t pc) const;
        std::string render_itype_load(uint32_t insn, const char *mnemonic, int32_t imm_i) const;
        std::string render_stype(uint32_t insn, const char *mnemonic) const;
        std::string render_itype_alu(uint32_t insn, const char *mnemonic, int32_t imm_i) const;
        std::string render_rtype(uint32_t insn, const char *mnemonic) const;
        std::string render_fence(uint32_t insn) const;
        std::string render_ecall() const;
        std::string render_ebreak() const;

};


#endif