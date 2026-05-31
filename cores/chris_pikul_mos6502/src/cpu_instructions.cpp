/*
 *	MOS-6502 Emulator
 * ===================
 * Simple emulator recreating the processes of the famous MOS-6502 processor.
 *
 * Copyright (c) 2021 Chris Pikul.
 * MIT License. See file "LICENSE.txt" for full license.
 */
#include "cpu.h"

#include <iostream>

#include "utils.h"

namespace mos6502 {

	fast_byte CPU::ExecuteInstruction(const Instruction& instr, const address& addr) {
		switch (instr) {
			case Instruction::ILL: // Illegal Operand
				std::cerr << "attempting to execute an illegal instruction in mos6502::CPU::ExecuteInstruction" << std::endl;
				break;

			case Instruction::ADC: // Add with carry
				return Ins_ADC(addr);
			case Instruction::AND: // AND (Acc)
				return Ins_AND(addr);
			case Instruction::ASL: // Arithmetic Shift Left
				return Ins_ASL(addr);
			case Instruction::BCC: // Branch on Carry Clear
				return Ins_BCC(addr);
			case Instruction::BCS: // Branch on Carry Set
				return Ins_BCS(addr);
			case Instruction::BEQ: // Branch on Equal (zero set)
				return Ins_BEQ(addr);
			case Instruction::BIT: // Bit Test
				return Ins_BIT(addr);
			case Instruction::BMI: // Branch on Minus (negative set)
				return Ins_BMI(addr);
			case Instruction::BNE: // Branch on Not Equal (zero clear)
				return Ins_BNE(addr);
			case Instruction::BPL: // Branch on Plus (negative clear)
				return Ins_BPL(addr);
			case Instruction::BRK: // Break / Interrupt
				return Ins_BRK(addr);
			case Instruction::BVC: // Branch on Overflow Clear
				return Ins_BVC(addr);
			case Instruction::BVS: // Branch on Overflow Set
				return Ins_BVS(addr);
			case Instruction::CLC: // Clear Carry
				return Ins_CLC(addr);
			case Instruction::CLD: // Clear Decimal
				return Ins_CLD(addr);
			case Instruction::CLI: // Clear Interrupt Disable
				return Ins_CLI(addr);
			case Instruction::CLV: // Clear Overflow
				return Ins_CLV(addr);
			case Instruction::CMP: // Compare w/Acc
				return Ins_CMP(addr);
			case Instruction::CPX: // Compare w/X
				return Ins_CPX(addr);
			case Instruction::CPY: // Compare w/Y
				return Ins_CPY(addr);
			case Instruction::DEC: // Decrement
				return Ins_DEC(addr);
			case Instruction::DEX: // Decrement X
				return Ins_DEX(addr);
			case Instruction::DEY: // Decrement Y
				return Ins_DEY(addr);
			case Instruction::EOR: // Exclusive OR
				return Ins_EOR(addr);
			case Instruction::INC: // Increment
				return Ins_INC(addr);
			case Instruction::INX: // Increment X
				return Ins_INX(addr);
			case Instruction::INY: // Increment Y
				return Ins_INY(addr);
			case Instruction::JMP: // Jump
				return Ins_JMP(addr);
			case Instruction::JSR: // Jump Subroutine
				return Ins_JSR(addr);
			case Instruction::LDA: // Load Accumulator
				return Ins_LDA(addr);
			case Instruction::LDX: // Load X
				return Ins_LDX(addr);
			case Instruction::LDY: // Load Y
				return Ins_LDY(addr);
			case Instruction::LSR: // Logical Shift R
				return Ins_LSR(addr);
			case Instruction::NOP: // No-Operation
				return Ins_NOP(addr);
			case Instruction::ORA: // Or w/Acc
				return Ins_ORA(addr);
			case Instruction::PHA: // Push Acc
				return Ins_PHA(addr);
			case Instruction::PHP: // Push PC
				return Ins_PHP(addr);
			case Instruction::PLA: // Pull Acc
				return Ins_PLA(addr);
			case Instruction::PLP: // Pull PC
				return Ins_PLP(addr);
			case Instruction::ROL: // Rotate Left
				return Ins_ROL(addr);
			case Instruction::ROR: // Rotate Right
				return Ins_ROR(addr);
			case Instruction::RTI: // Return from Interrupt
				return Ins_RTI(addr);
			case Instruction::RTS: // Return from Subroutine
				return Ins_RTS(addr);
			case Instruction::SBC: // Subtract with Carry
				return Ins_SBC(addr);
			case Instruction::SEC: // Set Carry
				return Ins_SEC(addr);
			case Instruction::SED: // Set Decimal
				return Ins_SED(addr);
			case Instruction::SEI: // Set Interrupt Disable
				return Ins_SEI(addr);
			case Instruction::STA: // Store Accumulator
				return Ins_STA(addr);
			case Instruction::STX: // Store X
				return Ins_STX(addr);
			case Instruction::STY: // Store Y
				return Ins_STY(addr);
			case Instruction::TAX: // Transfer Acc to X
				return Ins_TAX(addr);
			case Instruction::TAY: // Transfer Acc to Y
				return Ins_TAY(addr);
			case Instruction::TSX: // Transfer SP to X
				return Ins_TSX(addr);
			case Instruction::TXA: // Transfer X to Acc
				return Ins_TXA(addr);
			case Instruction::TXS: // Transfer X to SP
				return Ins_TXS(addr);
			case Instruction::TYA: // Transfer Y to Acc
				return Ins_TYA(addr);

			default:
				std::cerr << "warning: unknown or unimplemented instruction in mos6502::CPU::ExecuteInstruction" << std::endl;
		}

		return 0;
	}

	fast_byte CPU::Branch(const address& addr) {
		// Get the next program counter address
		address nextPC = m_PC + addr.value;

		// The amount of cycles increases if there was a page change
		fast_byte cycles = nextPC.page != (m_PC & 0xFF00) ? 3 : 2;

		// Assign the new PC
		m_PC = nextPC.value;

		return cycles;
	}

	fast_byte CPU::Ins_ADC(const address& addr) {
		const byte value = FetchData(addr);
		const byte carry = GetStatusFlag(StatusFlag::CARRY);
		const word binary = static_cast<word>(m_Acc) + static_cast<word>(value) + carry;

		if (HasStatusFlag(StatusFlag::DECIMAL)) {
			uint8_t low = static_cast<uint8_t>((m_Acc & 0x0F) + (value & 0x0F) + carry);
			uint8_t high = static_cast<uint8_t>((m_Acc >> 4) + (value >> 4));
			if (low > 9) {
				low = static_cast<uint8_t>(low - 10);
				high++;
			}

			byte result = static_cast<byte>((high << 4) | (low & 0x0F));
			SetStatusFlag(StatusFlag::ZERO, GET_LOW_BYTE(binary) == 0);
			SetStatusFlag(StatusFlag::NEGATIVE, IS_NEGATIVE(result));
			SetStatusFlag(StatusFlag::INT_OVERFLOW, (~(m_Acc ^ value) & (m_Acc ^ result) & 0x80) != 0);

			if (high > 9) {
				result = static_cast<byte>(result - 0xA0);
				SetStatusFlag(StatusFlag::CARRY, true);
			} else {
				SetStatusFlag(StatusFlag::CARRY, false);
			}

			m_Acc = result;
			return 1;
		}

		const byte result = GET_LOW_BYTE(binary);
		SetStatusFlag(StatusFlag::CARRY, binary > 0xFF);
		SetStatusFlag(StatusFlag::ZERO, result == 0);
		SetStatusFlag(StatusFlag::NEGATIVE, IS_NEGATIVE(result));
		SetStatusFlag(StatusFlag::INT_OVERFLOW, (~(m_Acc ^ value) & (m_Acc ^ result) & 0x80) != 0);
		m_Acc = result;
		return 1;
	}

	fast_byte CPU::Ins_AND(const address& addr) {
		m_Acc &= FetchData(addr);

		SetStatusFlag(StatusFlag::ZERO, m_Acc == 0);
		SetStatusFlag(StatusFlag::NEGATIVE, IS_NEGATIVE(m_Acc));

		return 1;
	}

	fast_byte CPU::Ins_ASL(const address& addr) {
		word result = static_cast<word>(FetchData(addr)) << 1;
		const byte resultByte = GET_LOW_BYTE(result);

		{ // Set the processor status flags
			SetStatusFlag(StatusFlag::CARRY, result > 0xFF);
			SetStatusFlag(StatusFlag::ZERO, resultByte == 0);
			SetStatusFlag(StatusFlag::NEGATIVE, IS_NEGATIVE(resultByte));
		}

		if (m_WasSupplied) {
			m_Acc = resultByte;
		} else {
			WriteByte(addr, resultByte);
		}

		return m_WasSupplied ? 1 : 3;
	}

	fast_byte CPU::Ins_BCC(const address& addr) { 
		if (HasStatusFlag(StatusFlag::CARRY) == false)
			return Branch(addr);

		return 1;
	}

	fast_byte CPU::Ins_BCS(const address& addr) {
		if (HasStatusFlag(StatusFlag::CARRY) == true)
			return Branch(addr);

		return 1;
	}

	fast_byte CPU::Ins_BEQ(const address& addr) { 
		if (HasStatusFlag(StatusFlag::ZERO) == true)
			return Branch(addr);

		return 1;
	}

	fast_byte CPU::Ins_BIT(const address& addr) {
		byte value = FetchData(addr);

		byte result = m_Acc & value;

		{ // Set the processor status flags
			SetStatusFlag(StatusFlag::ZERO, result == 0);
			SetStatusFlag(StatusFlag::INT_OVERFLOW, value & 0x40);
			SetStatusFlag(StatusFlag::NEGATIVE, IS_NEGATIVE(value));
		}

		return 1;
	}

	fast_byte CPU::Ins_BMI(const address& addr) { 
		if (HasStatusFlag(StatusFlag::NEGATIVE) == true)
			return Branch(addr);
		return 1;
	}

	fast_byte CPU::Ins_BNE(const address& addr) {
		if (HasStatusFlag(StatusFlag::ZERO) == false)
			return Branch(addr);
		return 1;
	}

	fast_byte CPU::Ins_BPL(const address& addr) {
		if (HasStatusFlag(StatusFlag::NEGATIVE) == false)
			return Branch(addr);
		return 1;
	}

	fast_byte CPU::Ins_BRK(const address& addr) {
		//Increment PC past the brk
		m_PC++;

		//Push the PC onto the stack
		PushToStack(GET_HIGH_BYTE(m_PC)); // High bit
		PushToStack(GET_LOW_BYTE(m_PC)); // Low bit

		// Push the Processor Status as it appears on the bus for BRK.
		PushToStack(m_ProcStatus.value | static_cast<byte>(StatusFlag::BREAK) | static_cast<byte>(StatusFlag::UNUSED));

		//Load the interrupt vector
		const byte low = ReadByte(0xFFFE);
		const byte high = ReadByte(0xFFFF);

		//Set the PC to the vector
		m_PC = MAKE_WORD(low, high);

		//Mask further IRQs after the status value has been pushed.
		SetStatusFlag(StatusFlag::INTERRUPT, true);

		return 6; //5 bytes of IO and the function
	}

	fast_byte CPU::Ins_BVC(const address& addr) { 
		if (HasStatusFlag(StatusFlag::INT_OVERFLOW) == false)
			return Branch(addr);
		return 1;
	}

	fast_byte CPU::Ins_BVS(const address& addr) {
		if (HasStatusFlag(StatusFlag::INT_OVERFLOW) == true)
			return Branch(addr);
		return 1;
	}

	fast_byte CPU::Ins_CLC(const address& addr) {
		SetStatusFlag(StatusFlag::CARRY, false);
		return 1;
	}

	fast_byte CPU::Ins_CLD(const address& addr) {
		SetStatusFlag(StatusFlag::DECIMAL, false);
		return 1;
	}

	fast_byte CPU::Ins_CLI(const address& addr) {
		SetStatusFlag(StatusFlag::INTERRUPT, false);
		return 1;
	}

	fast_byte CPU::Ins_CLV(const address& addr) {
		SetStatusFlag(StatusFlag::INT_OVERFLOW, false);
		return 1;
	}

	fast_byte CPU::Ins_CMP(const address& addr) {
		const byte value = FetchData(addr);
		const byte result = m_Acc - value;

		SetStatusFlag(StatusFlag::CARRY, m_Acc >= value);
		SetStatusFlag(StatusFlag::ZERO, result == 0);
		SetStatusFlag(StatusFlag::NEGATIVE, IS_NEGATIVE(result));

		return 1;
	}

	fast_byte CPU::Ins_CPX(const address& addr) {
		const byte value = FetchData(addr);
		const byte result = m_X - value;

		SetStatusFlag(StatusFlag::CARRY, m_X >= value);
		SetStatusFlag(StatusFlag::ZERO, result == 0);
		SetStatusFlag(StatusFlag::NEGATIVE, IS_NEGATIVE(result));

		return 1;
	}

	fast_byte CPU::Ins_CPY(const address& addr) {
		const byte value = FetchData(addr);
		const byte result = m_Y - value;

		SetStatusFlag(StatusFlag::CARRY, m_Y >= value);
		SetStatusFlag(StatusFlag::ZERO, result == 0);
		SetStatusFlag(StatusFlag::NEGATIVE, IS_NEGATIVE(result));

		return 1;
	}

	fast_byte CPU::Ins_DEC(const address& addr) {
		byte value = FetchData(addr);
		value--;

		//Write to the memory location
		WriteByte(addr, value);

		SetStatusFlag(StatusFlag::ZERO, value == 0);
		SetStatusFlag(StatusFlag::NEGATIVE, IS_NEGATIVE(value));

		return 3; //Read, Execute, Write
	}

	fast_byte CPU::Ins_DEX(const address& addr) {
		m_X--;

		SetStatusFlag(StatusFlag::ZERO, m_X == 0);
		SetStatusFlag(StatusFlag::NEGATIVE, IS_NEGATIVE(m_X));

		return 1;
	}

	fast_byte CPU::Ins_DEY(const address& addr) {
		m_Y--;

		SetStatusFlag(StatusFlag::ZERO, m_Y == 0);
		SetStatusFlag(StatusFlag::NEGATIVE, IS_NEGATIVE(m_Y));

		return 1;
	}

	fast_byte CPU::Ins_EOR(const address& addr) {
		const byte value = FetchData(addr);

		// Exclusive OR the accumulator
		m_Acc ^= value;

		SetStatusFlag(StatusFlag::ZERO, m_Acc == 0);
		SetStatusFlag(StatusFlag::NEGATIVE, IS_NEGATIVE(m_Acc));

		return 1;
	}

	fast_byte CPU::Ins_INC(const address& addr) {
		byte value = FetchData(addr);
		value++;

		//Write to the memory location
		WriteByte(addr, value);

		SetStatusFlag(StatusFlag::ZERO, value == 0);
		SetStatusFlag(StatusFlag::NEGATIVE, IS_NEGATIVE(value));

		return 3; //Read, Execute, Write
	}

	fast_byte CPU::Ins_INX(const address& addr) {
		m_X++;

		SetStatusFlag(StatusFlag::ZERO, m_X == 0);
		SetStatusFlag(StatusFlag::NEGATIVE, IS_NEGATIVE(m_X));

		return 1;
	}

	fast_byte CPU::Ins_INY(const address& addr) {
		m_Y++;

		SetStatusFlag(StatusFlag::ZERO, m_Y == 0);
		SetStatusFlag(StatusFlag::NEGATIVE, IS_NEGATIVE(m_Y));

		return 1;
	}

	fast_byte CPU::Ins_JMP(const address& addr) {
		// NOTE: Original 6502 chips did not do this directly if the indirect addressing vector
		// was on a page boundary (0x**FF).
		// This was corrected in the 65SC02 chips and later.
		m_PC = addr.value;

		return 1;
	}

	fast_byte CPU::Ins_JSR(const address& addr) {
		//Correct the PC position
		m_PC--;

		//Push the PC to the stack
		PushToStack(GET_HIGH_BYTE(m_PC));
		PushToStack(GET_LOW_BYTE(m_PC));
		
		//Assign new PC
		m_PC = addr.value;

		return 3;
	}

	fast_byte CPU::Ins_LDA(const address& addr) {
		m_Acc = FetchData(addr);

		SetStatusFlag(StatusFlag::ZERO, m_Acc == 0);
		SetStatusFlag(StatusFlag::NEGATIVE, IS_NEGATIVE(m_Acc));

		return 1;
	}

	fast_byte CPU::Ins_LDX(const address& addr) {
		m_X = FetchData(addr);

		SetStatusFlag(StatusFlag::ZERO, m_X == 0);
		SetStatusFlag(StatusFlag::NEGATIVE, IS_NEGATIVE(m_X));

		return 1;
	}

	fast_byte CPU::Ins_LDY(const address& addr) {
		m_Y = FetchData(addr);

		SetStatusFlag(StatusFlag::ZERO, m_Y == 0);
		SetStatusFlag(StatusFlag::NEGATIVE, IS_NEGATIVE(m_Y));

		return 1;
	}

	fast_byte CPU::Ins_LSR(const address& addr) {
		const byte value = FetchData(addr);
		const byte result = value >> 1;

		{ //Set status flag
			SetStatusFlag(StatusFlag::CARRY, value & 0x01); // Last bit
			SetStatusFlag(StatusFlag::ZERO, result == 0);
			SetStatusFlag(StatusFlag::NEGATIVE, false);
		}

		if (m_WasSupplied) {
			m_Acc = result;
		} else {
			WriteByte(addr, result);
		}

		return m_WasSupplied ? 1 : 3;
	}

	fast_byte CPU::Ins_NOP(const address& addr) {
		//Do nothing.

		//NOTE: Illegal opcodes may actually take 2 cycles.

		return 1;
	}


	fast_byte CPU::Ins_ORA(const address& addr) {
		m_Acc |= FetchData(addr);

		SetStatusFlag(StatusFlag::ZERO, m_Acc == 0);
		SetStatusFlag(StatusFlag::NEGATIVE, IS_NEGATIVE(m_Acc));

		return 1;
	}

	fast_byte CPU::Ins_PHA(const address& addr) {
		PushToStack(m_Acc);

		return 2;
	}

	fast_byte CPU::Ins_PHP(const address& addr) { 
		PushToStack(m_ProcStatus.value | static_cast<byte>(StatusFlag::BREAK) | static_cast<byte>(StatusFlag::UNUSED));

		return 2; 
	}

	fast_byte CPU::Ins_PLA(const address& addr) {
		m_Acc = PullFromStack();

		SetStatusFlag(StatusFlag::ZERO, m_Acc == 0);
		SetStatusFlag(StatusFlag::NEGATIVE, IS_NEGATIVE(m_Acc));

		return 3;
	}

	fast_byte CPU::Ins_PLP(const address& addr) {
		m_ProcStatus = PullFromStack();

		SetStatusFlag(StatusFlag::UNUSED, true);

		return 3;
	}

	fast_byte CPU::Ins_ROL(const address& addr) {
		const byte value = FetchData(addr);
		const byte result = (value << 1) | GetStatusFlag(StatusFlag::CARRY);

		{ // Set status flags
			SetStatusFlag(StatusFlag::CARRY, value & 0x80);
			SetStatusFlag(StatusFlag::ZERO, result == 0);
			SetStatusFlag(StatusFlag::NEGATIVE, IS_NEGATIVE(result));
		}

		fast_byte cost = 1;
		if (m_WasSupplied) {
			m_Acc = result;
		} else {
			cost++;
			WriteByte(addr, result);
		}

		return cost;
	}

	fast_byte CPU::Ins_ROR(const address& addr) {
		const byte value = FetchData(addr);
		const byte result = (value >> 1) | (GetStatusFlag(StatusFlag::CARRY) << 7);

		{ // Set status flags
			SetStatusFlag(StatusFlag::CARRY, value & 0x01);
			SetStatusFlag(StatusFlag::ZERO, result == 0);
			SetStatusFlag(StatusFlag::NEGATIVE, IS_NEGATIVE(result));
		}

		fast_byte cost = 1;
		if (m_WasSupplied) {
			m_Acc = result;
		} else {
			cost++;
			WriteByte(addr, result);
		}

		return cost;
	}

	fast_byte CPU::Ins_RTI(const address& addr) {
		m_ProcStatus = PullFromStack();
		SetStatusFlag(StatusFlag::UNUSED, 1);
		
		const byte low = PullFromStack();
		const byte high = PullFromStack();
		m_PC = MAKE_WORD(low, high);

		return 5;
	}

	fast_byte CPU::Ins_RTS(const address& addr) {
		const byte low = PullFromStack();
		const byte high = PullFromStack();
		m_PC = MAKE_WORD(low, high) + 1;
		//NOTE: Doc says minus one, impl says +

		return 5;
	}

	fast_byte CPU::Ins_SBC(const address& addr) {
		const byte value = FetchData(addr);
		const byte carry = GetStatusFlag(StatusFlag::CARRY);
		const word binary = static_cast<word>(m_Acc) + static_cast<word>(value ^ 0xFFu) + carry;

		if (HasStatusFlag(StatusFlag::DECIMAL)) {
			const int borrow = carry ? 0 : 1;
			int low = static_cast<int>(m_Acc & 0x0F) - static_cast<int>(value & 0x0F) - borrow;
			int high = static_cast<int>(m_Acc >> 4) - static_cast<int>(value >> 4);
			if (low < 0) {
				low += 10;
				high--;
			}

			byte result = static_cast<byte>((static_cast<byte>(high) << 4) | (low & 0x0F));
			SetStatusFlag(StatusFlag::ZERO, GET_LOW_BYTE(binary) == 0);
			SetStatusFlag(StatusFlag::NEGATIVE, IS_NEGATIVE(result));
			SetStatusFlag(StatusFlag::INT_OVERFLOW, ((m_Acc ^ value) & (m_Acc ^ result) & 0x80) != 0);

			if (high < 0) {
				result = static_cast<byte>(result + 0xA0);
				SetStatusFlag(StatusFlag::CARRY, false);
			} else {
				SetStatusFlag(StatusFlag::CARRY, true);
			}

			m_Acc = result;
			return 1;
		}

		const byte result = GET_LOW_BYTE(binary);
		SetStatusFlag(StatusFlag::CARRY, binary > 0xFF);
		SetStatusFlag(StatusFlag::ZERO, result == 0);
		SetStatusFlag(StatusFlag::NEGATIVE, IS_NEGATIVE(result));
		SetStatusFlag(StatusFlag::INT_OVERFLOW, ((m_Acc ^ value) & (m_Acc ^ result) & 0x80) != 0);
		m_Acc = result;
		return 1;
	}

	fast_byte CPU::Ins_SEC(const address& addr) {
		SetStatusFlag(StatusFlag::CARRY, true);

		return 1;
	}

	fast_byte CPU::Ins_SED(const address& addr) {
		SetStatusFlag(StatusFlag::DECIMAL, true);

		return 1;
	}

	fast_byte CPU::Ins_SEI(const address& addr) {
		SetStatusFlag(StatusFlag::INTERRUPT, true);

		return 1;
	}


	fast_byte CPU::Ins_STA(const address& addr) {
		WriteByte(addr, m_Acc);

		return 1;
	}

	fast_byte CPU::Ins_STX(const address& addr) {
		WriteByte(addr, m_X);

		return 1;
	}

	fast_byte CPU::Ins_STY(const address& addr) {
		WriteByte(addr, m_Y);

		return 1;
	}

	fast_byte CPU::Ins_TAX(const address& addr) {
		m_X = m_Acc;

		SetStatusFlag(StatusFlag::ZERO, m_X == 0);
		SetStatusFlag(StatusFlag::NEGATIVE, IS_NEGATIVE(m_X));

		return 1;
	}

	fast_byte CPU::Ins_TAY(const address& addr) {
		m_Y = m_Acc;

		SetStatusFlag(StatusFlag::ZERO, m_Y == 0);
		SetStatusFlag(StatusFlag::NEGATIVE, IS_NEGATIVE(m_Y));

		return 1;
	}

	fast_byte CPU::Ins_TSX(const address& addr) {
		m_X = m_SP;

		SetStatusFlag(StatusFlag::ZERO, m_X == 0);
		SetStatusFlag(StatusFlag::NEGATIVE, IS_NEGATIVE(m_X));

		return 1;
	}

	fast_byte CPU::Ins_TXA(const address& addr) {
		m_Acc = m_X;

		SetStatusFlag(StatusFlag::ZERO, m_Acc == 0);
		SetStatusFlag(StatusFlag::NEGATIVE, IS_NEGATIVE(m_Acc));

		return 1;
	}

	fast_byte CPU::Ins_TXS(const address& addr) { 
		m_SP = m_X;

		return 1;
	}

	fast_byte CPU::Ins_TYA(const address& addr) {
		m_Acc = m_Y;

		SetStatusFlag(StatusFlag::ZERO, m_Acc == 0);
		SetStatusFlag(StatusFlag::NEGATIVE, IS_NEGATIVE(m_Acc));

		return 1;
	}
};