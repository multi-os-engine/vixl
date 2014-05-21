// Copyright 2013, ARM Limited
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//   * Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//   * Neither the name of ARM Limited nor the names of its contributors may be
//     used to endorse or promote products derived from this software without
//     specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


#include <cmath>
#include "a64/assembler-a64.h"

namespace vixl {

// CPURegList utilities.
CPURegister CPURegList::PopLowestIndex() {
  if (IsEmpty()) {
    return NoCPUReg;
  }
  int index = CountTrailingZeros(list_, kRegListSizeInBits);
  VIXL_ASSERT((1 << index) & list_);
  Remove(index);
  return CPURegister(index, size_, type_);
}


CPURegister CPURegList::PopHighestIndex() {
  VIXL_ASSERT(IsValid());
  if (IsEmpty()) {
    return NoCPUReg;
  }
  int index = CountLeadingZeros(list_, kRegListSizeInBits);
  index = kRegListSizeInBits - 1 - index;
  VIXL_ASSERT((1 << index) & list_);
  Remove(index);
  return CPURegister(index, size_, type_);
}


bool CPURegList::IsValid() const {
  if ((type_ == CPURegister::kRegister) ||
      (type_ == CPURegister::kFPRegister)) {
    bool is_valid = true;
    // Try to create a CPURegister for each element in the list.
    for (int i = 0; i < kRegListSizeInBits; i++) {
      if (((list_ >> i) & 1) != 0) {
        is_valid &= CPURegister(i, size_, type_).IsValid();
      }
    }
    return is_valid;
  } else if (type_ == CPURegister::kNoRegister) {
    // We can't use IsEmpty here because that asserts IsValid().
    return list_ == 0;
  } else {
    return false;
  }
}


void CPURegList::RemoveCalleeSaved() {
  if (type() == CPURegister::kRegister) {
    Remove(GetCalleeSaved(RegisterSizeInBits()));
  } else if (type() == CPURegister::kFPRegister) {
    Remove(GetCalleeSavedFP(RegisterSizeInBits()));
  } else {
    VIXL_ASSERT(type() == CPURegister::kNoRegister);
    VIXL_ASSERT(IsEmpty());
    // The list must already be empty, so do nothing.
  }
}


CPURegList CPURegList::GetCalleeSaved(unsigned size) {
  return CPURegList(CPURegister::kRegister, size, 19, 29);
}


CPURegList CPURegList::GetCalleeSavedFP(unsigned size) {
  return CPURegList(CPURegister::kFPRegister, size, 8, 15);
}


CPURegList CPURegList::GetCallerSaved(unsigned size) {
  // Registers x0-x18 and lr (x30) are caller-saved.
  CPURegList list = CPURegList(CPURegister::kRegister, size, 0, 18);
  list.Combine(lr);
  return list;
}


CPURegList CPURegList::GetCallerSavedFP(unsigned size) {
  // Registers d0-d7 and d16-d31 are caller-saved.
  CPURegList list = CPURegList(CPURegister::kFPRegister, size, 0, 7);
  list.Combine(CPURegList(CPURegister::kFPRegister, size, 16, 31));
  return list;
}


const CPURegList kCalleeSaved = CPURegList::GetCalleeSaved();
const CPURegList kCalleeSavedFP = CPURegList::GetCalleeSavedFP();
const CPURegList kCallerSaved = CPURegList::GetCallerSaved();
const CPURegList kCallerSavedFP = CPURegList::GetCallerSavedFP();


// Registers.
#define WREG(n) w##n,
const Register Register::wregisters[] = {
REGISTER_CODE_LIST(WREG)
};
#undef WREG

#define XREG(n) x##n,
const Register Register::xregisters[] = {
REGISTER_CODE_LIST(XREG)
};
#undef XREG

#define SREG(n) s##n,
const FPRegister FPRegister::sregisters[] = {
REGISTER_CODE_LIST(SREG)
};
#undef SREG

#define DREG(n) d##n,
const FPRegister FPRegister::dregisters[] = {
REGISTER_CODE_LIST(DREG)
};
#undef DREG


const Register& Register::WRegFromCode(unsigned code) {
  if (code == kSPRegInternalCode) {
    return wsp;
  } else {
    VIXL_ASSERT(code < kNumberOfRegisters);
    return wregisters[code];
  }
}


const Register& Register::XRegFromCode(unsigned code) {
  if (code == kSPRegInternalCode) {
    return sp;
  } else {
    VIXL_ASSERT(code < kNumberOfRegisters);
    return xregisters[code];
  }
}


const FPRegister& FPRegister::SRegFromCode(unsigned code) {
  VIXL_ASSERT(code < kNumberOfFPRegisters);
  return sregisters[code];
}


const FPRegister& FPRegister::DRegFromCode(unsigned code) {
  VIXL_ASSERT(code < kNumberOfFPRegisters);
  return dregisters[code];
}


const Register& CPURegister::W() const {
  VIXL_ASSERT(IsValidRegister());
  return Register::WRegFromCode(code_);
}


const Register& CPURegister::X() const {
  VIXL_ASSERT(IsValidRegister());
  return Register::XRegFromCode(code_);
}


const FPRegister& CPURegister::S() const {
  VIXL_ASSERT(IsValidFPRegister());
  return FPRegister::SRegFromCode(code_);
}


const FPRegister& CPURegister::D() const {
  VIXL_ASSERT(IsValidFPRegister());
  return FPRegister::DRegFromCode(code_);
}


// Operand.
Operand::Operand(int64_t immediate)
    : immediate_(immediate),
      reg_(NoReg),
      shift_(NO_SHIFT),
      extend_(NO_EXTEND),
      shift_amount_(0) {}


Operand::Operand(Register reg, Shift shift, unsigned shift_amount)
    : reg_(reg),
      shift_(shift),
      extend_(NO_EXTEND),
      shift_amount_(shift_amount) {
  VIXL_ASSERT(reg.Is64Bits() || (shift_amount < kWRegSize));
  VIXL_ASSERT(reg.Is32Bits() || (shift_amount < kXRegSize));
  VIXL_ASSERT(!reg.IsSP());
}


Operand::Operand(Register reg, Extend extend, unsigned shift_amount)
    : reg_(reg),
      shift_(NO_SHIFT),
      extend_(extend),
      shift_amount_(shift_amount) {
  VIXL_ASSERT(reg.IsValid());
  VIXL_ASSERT(shift_amount <= 4);
  VIXL_ASSERT(!reg.IsSP());

  // Extend modes SXTX and UXTX require a 64-bit register.
  VIXL_ASSERT(reg.Is64Bits() || ((extend != SXTX) && (extend != UXTX)));
}


bool Operand::IsImmediate() const {
  return reg_.Is(NoReg);
}


bool Operand::IsShiftedRegister() const {
  return reg_.IsValid() && (shift_ != NO_SHIFT);
}


bool Operand::IsExtendedRegister() const {
  return reg_.IsValid() && (extend_ != NO_EXTEND);
}


bool Operand::IsZero() const {
  if (IsImmediate()) {
    return immediate() == 0;
  } else {
    return reg().IsZero();
  }
}


Operand Operand::ToExtendedRegister() const {
  VIXL_ASSERT(IsShiftedRegister());
  VIXL_ASSERT((shift_ == LSL) && (shift_amount_ <= 4));
  return Operand(reg_, reg_.Is64Bits() ? UXTX : UXTW, shift_amount_);
}


// MemOperand
MemOperand::MemOperand(Register base, ptrdiff_t offset, AddrMode addrmode)
  : base_(base), regoffset_(NoReg), offset_(offset), addrmode_(addrmode) {
  VIXL_ASSERT(base.Is64Bits() && !base.IsZero());
}


MemOperand::MemOperand(Register base,
                       Register regoffset,
                       Extend extend,
                       unsigned shift_amount)
  : base_(base), regoffset_(regoffset), offset_(0), addrmode_(Offset),
    shift_(NO_SHIFT), extend_(extend), shift_amount_(shift_amount) {
  VIXL_ASSERT(base.Is64Bits() && !base.IsZero());
  VIXL_ASSERT(!regoffset.IsSP());
  VIXL_ASSERT((extend == UXTW) || (extend == SXTW) || (extend == SXTX));

  // SXTX extend mode requires a 64-bit offset register.
  VIXL_ASSERT(regoffset.Is64Bits() || (extend != SXTX));
}


MemOperand::MemOperand(Register base,
                       Register regoffset,
                       Shift shift,
                       unsigned shift_amount)
  : base_(base), regoffset_(regoffset), offset_(0), addrmode_(Offset),
    shift_(shift), extend_(NO_EXTEND), shift_amount_(shift_amount) {
  VIXL_ASSERT(base.Is64Bits() && !base.IsZero());
  VIXL_ASSERT(regoffset.Is64Bits() && !regoffset.IsSP());
  VIXL_ASSERT(shift == LSL);
}


MemOperand::MemOperand(Register base, const Operand& offset, AddrMode addrmode)
  : base_(base), regoffset_(NoReg), addrmode_(addrmode) {
  VIXL_ASSERT(base.Is64Bits() && !base.IsZero());

  if (offset.IsImmediate()) {
    offset_ = offset.immediate();
  } else if (offset.IsShiftedRegister()) {
    VIXL_ASSERT(addrmode == Offset);

    regoffset_ = offset.reg();
    shift_= offset.shift();
    shift_amount_ = offset.shift_amount();

    extend_ = NO_EXTEND;
    offset_ = 0;

    // These assertions match those in the shifted-register constructor.
    VIXL_ASSERT(regoffset_.Is64Bits() && !regoffset_.IsSP());
    VIXL_ASSERT(shift_ == LSL);
  } else {
    VIXL_ASSERT(offset.IsExtendedRegister());
    VIXL_ASSERT(addrmode == Offset);

    regoffset_ = offset.reg();
    extend_ = offset.extend();
    shift_amount_ = offset.shift_amount();

    shift_= NO_SHIFT;
    offset_ = 0;

    // These assertions match those in the extended-register constructor.
    VIXL_ASSERT(!regoffset_.IsSP());
    VIXL_ASSERT((extend_ == UXTW) || (extend_ == SXTW) || (extend_ == SXTX));
    VIXL_ASSERT((regoffset_.Is64Bits() || (extend_ != SXTX)));
  }
}


bool MemOperand::IsImmediateOffset() const {
  return (addrmode_ == Offset) && regoffset_.Is(NoReg);
}


bool MemOperand::IsRegisterOffset() const {
  return (addrmode_ == Offset) && !regoffset_.Is(NoReg);
}


bool MemOperand::IsPreIndex() const {
  return addrmode_ == PreIndex;
}


bool MemOperand::IsPostIndex() const {
  return addrmode_ == PostIndex;
}


// Assembler
Assembler::Assembler(byte* buffer, unsigned buffer_size)
    : buffer_size_(buffer_size), literal_pool_monitor_(0) {

  buffer_ = reinterpret_cast<Instruction*>(buffer);
  pc_ = buffer_;
  Reset();
}


Assembler::~Assembler() {
  VIXL_ASSERT(finalized_ || (pc_ == buffer_));
  VIXL_ASSERT(literals_.empty());
}


void Assembler::Reset() {
#ifdef DEBUG
  VIXL_ASSERT((pc_ >= buffer_) && (pc_ < buffer_ + buffer_size_));
  VIXL_ASSERT(literal_pool_monitor_ == 0);
  memset(buffer_, 0, pc_ - buffer_);
  finalized_ = false;
#endif
  pc_ = buffer_;
  literals_.clear();
  next_literal_pool_check_ = pc_ + kLiteralPoolCheckInterval;
}


void Assembler::FinalizeCode() {
  EmitLiteralPool();
#ifdef DEBUG
  finalized_ = true;
#endif
}


void Assembler::bind(Label* label) {
  label->is_bound_ = true;
  label->target_ = pc_;
  while (label->IsLinked()) {
    // Get the address of the following instruction in the chain.
    Instruction* next_link = label->link_->ImmPCOffsetTarget();
    // Update the instruction target.
    label->link_->SetImmPCOffsetTarget(label->target_);
    // Update the label's link.
    // If the offset of the branch we just updated was 0 (kEndOfChain) we are
    // done.
    label->link_ = (label->link_ != next_link) ? next_link : NULL;
  }
}


int Assembler::UpdateAndGetByteOffsetTo(Label* label) {
  int offset;
  VIXL_STATIC_ASSERT(sizeof(*pc_) == 1);
  if (label->IsBound()) {
    offset = label->target() - pc_;
  } else if (label->IsLinked()) {
    offset = label->link() - pc_;
  } else {
    offset = Label::kEndOfChain;
  }
  label->set_link(pc_);
  return offset;
}


// Code generation.
void Assembler::br(const Register& xn) {
  VIXL_ASSERT(xn.Is64Bits());
  Emit(BR | Rn(xn));
}


void Assembler::blr(const Register& xn) {
  VIXL_ASSERT(xn.Is64Bits());
  Emit(BLR | Rn(xn));
}


void Assembler::ret(const Register& xn) {
  VIXL_ASSERT(xn.Is64Bits());
  Emit(RET | Rn(xn));
}


void Assembler::b(int imm26) {
  Emit(B | ImmUncondBranch(imm26));
}


void Assembler::b(int imm19, Condition cond) {
  Emit(B_cond | ImmCondBranch(imm19) | cond);
}


void Assembler::b(Label* label) {
  b(UpdateAndGetInstructionOffsetTo(label));
}


void Assembler::b(Label* label, Condition cond) {
  b(UpdateAndGetInstructionOffsetTo(label), cond);
}


void Assembler::bl(int imm26) {
  Emit(BL | ImmUncondBranch(imm26));
}


void Assembler::bl(Label* label) {
  bl(UpdateAndGetInstructionOffsetTo(label));
}


void Assembler::cbz(const Register& rt,
                    int imm19) {
  Emit(SF(rt) | CBZ | ImmCmpBranch(imm19) | Rt(rt));
}


void Assembler::cbz(const Register& rt,
                    Label* label) {
  cbz(rt, UpdateAndGetInstructionOffsetTo(label));
}


void Assembler::cbnz(const Register& rt,
                     int imm19) {
  Emit(SF(rt) | CBNZ | ImmCmpBranch(imm19) | Rt(rt));
}


void Assembler::cbnz(const Register& rt,
                     Label* label) {
  cbnz(rt, UpdateAndGetInstructionOffsetTo(label));
}


void Assembler::tbz(const Register& rt,
                    unsigned bit_pos,
                    int imm14) {
  VIXL_ASSERT(rt.Is64Bits() || (rt.Is32Bits() && (bit_pos < kWRegSize)));
  Emit(TBZ | ImmTestBranchBit(bit_pos) | ImmTestBranch(imm14) | Rt(rt));
}


void Assembler::tbz(const Register& rt,
                    unsigned bit_pos,
                    Label* label) {
  tbz(rt, bit_pos, UpdateAndGetInstructionOffsetTo(label));
}


void Assembler::tbnz(const Register& rt,
                     unsigned bit_pos,
                     int imm14) {
  VIXL_ASSERT(rt.Is64Bits() || (rt.Is32Bits() && (bit_pos < kWRegSize)));
  Emit(TBNZ | ImmTestBranchBit(bit_pos) | ImmTestBranch(imm14) | Rt(rt));
}


void Assembler::tbnz(const Register& rt,
                     unsigned bit_pos,
                     Label* label) {
  tbnz(rt, bit_pos, UpdateAndGetInstructionOffsetTo(label));
}


void Assembler::adr(const Register& rd, int imm21) {
  VIXL_ASSERT(rd.Is64Bits());
  Emit(ADR | ImmPCRelAddress(imm21) | Rd(rd));
}


void Assembler::adr(const Register& rd, Label* label) {
  adr(rd, UpdateAndGetByteOffsetTo(label));
}


void Assembler::add(const Register& rd,
                    const Register& rn,
                    const Operand& operand) {
  AddSub(rd, rn, operand, LeaveFlags, ADD);
}


void Assembler::adds(const Register& rd,
                     const Register& rn,
                     const Operand& operand) {
  AddSub(rd, rn, operand, SetFlags, ADD);
}


void Assembler::cmn(const Register& rn,
                    const Operand& operand) {
  Register zr = AppropriateZeroRegFor(rn);
  adds(zr, rn, operand);
}


void Assembler::sub(const Register& rd,
                    const Register& rn,
                    const Operand& operand) {
  AddSub(rd, rn, operand, LeaveFlags, SUB);
}


void Assembler::subs(const Register& rd,
                     const Register& rn,
                     const Operand& operand) {
  AddSub(rd, rn, operand, SetFlags, SUB);
}


void Assembler::cmp(const Register& rn, const Operand& operand) {
  Register zr = AppropriateZeroRegFor(rn);
  subs(zr, rn, operand);
}


void Assembler::neg(const Register& rd, const Operand& operand) {
  Register zr = AppropriateZeroRegFor(rd);
  sub(rd, zr, operand);
}


void Assembler::negs(const Register& rd, const Operand& operand) {
  Register zr = AppropriateZeroRegFor(rd);
  subs(rd, zr, operand);
}


void Assembler::adc(const Register& rd,
                    const Register& rn,
                    const Operand& operand) {
  AddSubWithCarry(rd, rn, operand, LeaveFlags, ADC);
}


void Assembler::adcs(const Register& rd,
                     const Register& rn,
                     const Operand& operand) {
  AddSubWithCarry(rd, rn, operand, SetFlags, ADC);
}


void Assembler::sbc(const Register& rd,
                    const Register& rn,
                    const Operand& operand) {
  AddSubWithCarry(rd, rn, operand, LeaveFlags, SBC);
}


void Assembler::sbcs(const Register& rd,
                     const Register& rn,
                     const Operand& operand) {
  AddSubWithCarry(rd, rn, operand, SetFlags, SBC);
}


void Assembler::ngc(const Register& rd, const Operand& operand) {
  Register zr = AppropriateZeroRegFor(rd);
  sbc(rd, zr, operand);
}


void Assembler::ngcs(const Register& rd, const Operand& operand) {
  Register zr = AppropriateZeroRegFor(rd);
  sbcs(rd, zr, operand);
}


// Logical instructions.
void Assembler::and_(const Register& rd,
                     const Register& rn,
                     const Operand& operand) {
  Logical(rd, rn, operand, AND);
}


void Assembler::ands(const Register& rd,
                     const Register& rn,
                     const Operand& operand) {
  Logical(rd, rn, operand, ANDS);
}


void Assembler::tst(const Register& rn,
                    const Operand& operand) {
  ands(AppropriateZeroRegFor(rn), rn, operand);
}


void Assembler::bic(const Register& rd,
                    const Register& rn,
                    const Operand& operand) {
  Logical(rd, rn, operand, BIC);
}


void Assembler::bics(const Register& rd,
                     const Register& rn,
                     const Operand& operand) {
  Logical(rd, rn, operand, BICS);
}


void Assembler::orr(const Register& rd,
                    const Register& rn,
                    const Operand& operand) {
  Logical(rd, rn, operand, ORR);
}


void Assembler::orn(const Register& rd,
                    const Register& rn,
                    const Operand& operand) {
  Logical(rd, rn, operand, ORN);
}


void Assembler::eor(const Register& rd,
                    const Register& rn,
                    const Operand& operand) {
  Logical(rd, rn, operand, EOR);
}


void Assembler::eon(const Register& rd,
                    const Register& rn,
                    const Operand& operand) {
  Logical(rd, rn, operand, EON);
}


void Assembler::lslv(const Register& rd,
                     const Register& rn,
                     const Register& rm) {
  VIXL_ASSERT(rd.size() == rn.size());
  VIXL_ASSERT(rd.size() == rm.size());
  Emit(SF(rd) | LSLV | Rm(rm) | Rn(rn) | Rd(rd));
}


void Assembler::lsrv(const Register& rd,
                     const Register& rn,
                     const Register& rm) {
  VIXL_ASSERT(rd.size() == rn.size());
  VIXL_ASSERT(rd.size() == rm.size());
  Emit(SF(rd) | LSRV | Rm(rm) | Rn(rn) | Rd(rd));
}


void Assembler::asrv(const Register& rd,
                     const Register& rn,
                     const Register& rm) {
  VIXL_ASSERT(rd.size() == rn.size());
  VIXL_ASSERT(rd.size() == rm.size());
  Emit(SF(rd) | ASRV | Rm(rm) | Rn(rn) | Rd(rd));
}


void Assembler::rorv(const Register& rd,
                     const Register& rn,
                     const Register& rm) {
  VIXL_ASSERT(rd.size() == rn.size());
  VIXL_ASSERT(rd.size() == rm.size());
  Emit(SF(rd) | RORV | Rm(rm) | Rn(rn) | Rd(rd));
}


// Bitfield operations.
void Assembler::bfm(const Register& rd,
                     const Register& rn,
                     unsigned immr,
                     unsigned imms) {
  VIXL_ASSERT(rd.size() == rn.size());
  Instr N = SF(rd) >> (kSFOffset - kBitfieldNOffset);
  Emit(SF(rd) | BFM | N |
       ImmR(immr, rd.size()) | ImmS(imms, rn.size()) | Rn(rn) | Rd(rd));
}


void Assembler::sbfm(const Register& rd,
                     const Register& rn,
                     unsigned immr,
                     unsigned imms) {
  VIXL_ASSERT(rd.Is64Bits() || rn.Is32Bits());
  Instr N = SF(rd) >> (kSFOffset - kBitfieldNOffset);
  Emit(SF(rd) | SBFM | N |
       ImmR(immr, rd.size()) | ImmS(imms, rn.size()) | Rn(rn) | Rd(rd));
}


void Assembler::ubfm(const Register& rd,
                     const Register& rn,
                     unsigned immr,
                     unsigned imms) {
  VIXL_ASSERT(rd.size() == rn.size());
  Instr N = SF(rd) >> (kSFOffset - kBitfieldNOffset);
  Emit(SF(rd) | UBFM | N |
       ImmR(immr, rd.size()) | ImmS(imms, rn.size()) | Rn(rn) | Rd(rd));
}


void Assembler::extr(const Register& rd,
                     const Register& rn,
                     const Register& rm,
                     unsigned lsb) {
  VIXL_ASSERT(rd.size() == rn.size());
  VIXL_ASSERT(rd.size() == rm.size());
  Instr N = SF(rd) >> (kSFOffset - kBitfieldNOffset);
  Emit(SF(rd) | EXTR | N | Rm(rm) | ImmS(lsb, rn.size()) | Rn(rn) | Rd(rd));
}


void Assembler::csel(const Register& rd,
                     const Register& rn,
                     const Register& rm,
                     Condition cond) {
  ConditionalSelect(rd, rn, rm, cond, CSEL);
}


void Assembler::csinc(const Register& rd,
                      const Register& rn,
                      const Register& rm,
                      Condition cond) {
  ConditionalSelect(rd, rn, rm, cond, CSINC);
}


void Assembler::csinv(const Register& rd,
                      const Register& rn,
                      const Register& rm,
                      Condition cond) {
  ConditionalSelect(rd, rn, rm, cond, CSINV);
}


void Assembler::csneg(const Register& rd,
                      const Register& rn,
                      const Register& rm,
                      Condition cond) {
  ConditionalSelect(rd, rn, rm, cond, CSNEG);
}


void Assembler::cset(const Register &rd, Condition cond) {
  VIXL_ASSERT((cond != al) && (cond != nv));
  Register zr = AppropriateZeroRegFor(rd);
  csinc(rd, zr, zr, InvertCondition(cond));
}


void Assembler::csetm(const Register &rd, Condition cond) {
  VIXL_ASSERT((cond != al) && (cond != nv));
  Register zr = AppropriateZeroRegFor(rd);
  csinv(rd, zr, zr, InvertCondition(cond));
}


void Assembler::cinc(const Register &rd, const Register &rn, Condition cond) {
  VIXL_ASSERT((cond != al) && (cond != nv));
  csinc(rd, rn, rn, InvertCondition(cond));
}


void Assembler::cinv(const Register &rd, const Register &rn, Condition cond) {
  VIXL_ASSERT((cond != al) && (cond != nv));
  csinv(rd, rn, rn, InvertCondition(cond));
}


void Assembler::cneg(const Register &rd, const Register &rn, Condition cond) {
  VIXL_ASSERT((cond != al) && (cond != nv));
  csneg(rd, rn, rn, InvertCondition(cond));
}


void Assembler::ConditionalSelect(const Register& rd,
                                  const Register& rn,
                                  const Register& rm,
                                  Condition cond,
                                  ConditionalSelectOp op) {
  VIXL_ASSERT(rd.size() == rn.size());
  VIXL_ASSERT(rd.size() == rm.size());
  Emit(SF(rd) | op | Rm(rm) | Cond(cond) | Rn(rn) | Rd(rd));
}


void Assembler::ccmn(const Register& rn,
                     const Operand& operand,
                     StatusFlags nzcv,
                     Condition cond) {
  ConditionalCompare(rn, operand, nzcv, cond, CCMN);
}


void Assembler::ccmp(const Register& rn,
                     const Operand& operand,
                     StatusFlags nzcv,
                     Condition cond) {
  ConditionalCompare(rn, operand, nzcv, cond, CCMP);
}


void Assembler::DataProcessing3Source(const Register& rd,
                     const Register& rn,
                     const Register& rm,
                     const Register& ra,
                     DataProcessing3SourceOp op) {
  Emit(SF(rd) | op | Rm(rm) | Ra(ra) | Rn(rn) | Rd(rd));
}


void Assembler::mul(const Register& rd,
                    const Register& rn,
                    const Register& rm) {
  VIXL_ASSERT(AreSameSizeAndType(rd, rn, rm));
  DataProcessing3Source(rd, rn, rm, AppropriateZeroRegFor(rd), MADD);
}


void Assembler::madd(const Register& rd,
                     const Register& rn,
                     const Register& rm,
                     const Register& ra) {
  DataProcessing3Source(rd, rn, rm, ra, MADD);
}


void Assembler::mneg(const Register& rd,
                     const Register& rn,
                     const Register& rm) {
  VIXL_ASSERT(AreSameSizeAndType(rd, rn, rm));
  DataProcessing3Source(rd, rn, rm, AppropriateZeroRegFor(rd), MSUB);
}


void Assembler::msub(const Register& rd,
                     const Register& rn,
                     const Register& rm,
                     const Register& ra) {
  DataProcessing3Source(rd, rn, rm, ra, MSUB);
}


void Assembler::umaddl(const Register& rd,
                       const Register& rn,
                       const Register& rm,
                       const Register& ra) {
  VIXL_ASSERT(rd.Is64Bits() && ra.Is64Bits());
  VIXL_ASSERT(rn.Is32Bits() && rm.Is32Bits());
  DataProcessing3Source(rd, rn, rm, ra, UMADDL_x);
}


void Assembler::smaddl(const Register& rd,
                       const Register& rn,
                       const Register& rm,
                       const Register& ra) {
  VIXL_ASSERT(rd.Is64Bits() && ra.Is64Bits());
  VIXL_ASSERT(rn.Is32Bits() && rm.Is32Bits());
  DataProcessing3Source(rd, rn, rm, ra, SMADDL_x);
}


void Assembler::umsubl(const Register& rd,
                       const Register& rn,
                       const Register& rm,
                       const Register& ra) {
  VIXL_ASSERT(rd.Is64Bits() && ra.Is64Bits());
  VIXL_ASSERT(rn.Is32Bits() && rm.Is32Bits());
  DataProcessing3Source(rd, rn, rm, ra, UMSUBL_x);
}


void Assembler::smsubl(const Register& rd,
                       const Register& rn,
                       const Register& rm,
                       const Register& ra) {
  VIXL_ASSERT(rd.Is64Bits() && ra.Is64Bits());
  VIXL_ASSERT(rn.Is32Bits() && rm.Is32Bits());
  DataProcessing3Source(rd, rn, rm, ra, SMSUBL_x);
}


void Assembler::smull(const Register& rd,
                      const Register& rn,
                      const Register& rm) {
  VIXL_ASSERT(rd.Is64Bits());
  VIXL_ASSERT(rn.Is32Bits() && rm.Is32Bits());
  DataProcessing3Source(rd, rn, rm, xzr, SMADDL_x);
}


void Assembler::sdiv(const Register& rd,
                     const Register& rn,
                     const Register& rm) {
  VIXL_ASSERT(rd.size() == rn.size());
  VIXL_ASSERT(rd.size() == rm.size());
  Emit(SF(rd) | SDIV | Rm(rm) | Rn(rn) | Rd(rd));
}


void Assembler::smulh(const Register& xd,
                      const Register& xn,
                      const Register& xm) {
  VIXL_ASSERT(xd.Is64Bits() && xn.Is64Bits() && xm.Is64Bits());
  DataProcessing3Source(xd, xn, xm, xzr, SMULH_x);
}

void Assembler::udiv(const Register& rd,
                     const Register& rn,
                     const Register& rm) {
  VIXL_ASSERT(rd.size() == rn.size());
  VIXL_ASSERT(rd.size() == rm.size());
  Emit(SF(rd) | UDIV | Rm(rm) | Rn(rn) | Rd(rd));
}


void Assembler::rbit(const Register& rd,
                     const Register& rn) {
  DataProcessing1Source(rd, rn, RBIT);
}


void Assembler::rev16(const Register& rd,
                      const Register& rn) {
  DataProcessing1Source(rd, rn, REV16);
}


void Assembler::rev32(const Register& rd,
                      const Register& rn) {
  VIXL_ASSERT(rd.Is64Bits());
  DataProcessing1Source(rd, rn, REV);
}


void Assembler::rev(const Register& rd,
                    const Register& rn) {
  DataProcessing1Source(rd, rn, rd.Is64Bits() ? REV_x : REV_w);
}


void Assembler::clz(const Register& rd,
                    const Register& rn) {
  DataProcessing1Source(rd, rn, CLZ);
}


void Assembler::cls(const Register& rd,
                    const Register& rn) {
  DataProcessing1Source(rd, rn, CLS);
}


void Assembler::ldp(const CPURegister& rt,
                    const CPURegister& rt2,
                    const MemOperand& src) {
  LoadStorePair(rt, rt2, src, LoadPairOpFor(rt, rt2));
}


void Assembler::stp(const CPURegister& rt,
                    const CPURegister& rt2,
                    const MemOperand& dst) {
  LoadStorePair(rt, rt2, dst, StorePairOpFor(rt, rt2));
}


void Assembler::ldpsw(const Register& rt,
                      const Register& rt2,
                      const MemOperand& src) {
  VIXL_ASSERT(rt.Is64Bits());
  LoadStorePair(rt, rt2, src, LDPSW_x);
}


void Assembler::LoadStorePair(const CPURegister& rt,
                              const CPURegister& rt2,
                              const MemOperand& addr,
                              LoadStorePairOp op) {
  // 'rt' and 'rt2' can only be aliased for stores.
  VIXL_ASSERT(((op & LoadStorePairLBit) == 0) || !rt.Is(rt2));
  VIXL_ASSERT(AreSameSizeAndType(rt, rt2));

  Instr memop = op | Rt(rt) | Rt2(rt2) | RnSP(addr.base()) |
                ImmLSPair(addr.offset(), CalcLSPairDataSize(op));

  Instr addrmodeop;
  if (addr.IsImmediateOffset()) {
    addrmodeop = LoadStorePairOffsetFixed;
  } else {
    VIXL_ASSERT(addr.offset() != 0);
    if (addr.IsPreIndex()) {
      addrmodeop = LoadStorePairPreIndexFixed;
    } else {
      VIXL_ASSERT(addr.IsPostIndex());
      addrmodeop = LoadStorePairPostIndexFixed;
    }
  }
  Emit(addrmodeop | memop);
}


void Assembler::ldnp(const CPURegister& rt,
                     const CPURegister& rt2,
                     const MemOperand& src) {
  LoadStorePairNonTemporal(rt, rt2, src,
                           LoadPairNonTemporalOpFor(rt, rt2));
}


void Assembler::stnp(const CPURegister& rt,
                     const CPURegister& rt2,
                     const MemOperand& dst) {
  LoadStorePairNonTemporal(rt, rt2, dst,
                           StorePairNonTemporalOpFor(rt, rt2));
}


void Assembler::LoadStorePairNonTemporal(const CPURegister& rt,
                                         const CPURegister& rt2,
                                         const MemOperand& addr,
                                         LoadStorePairNonTemporalOp op) {
  VIXL_ASSERT(!rt.Is(rt2));
  VIXL_ASSERT(AreSameSizeAndType(rt, rt2));
  VIXL_ASSERT(addr.IsImmediateOffset());

  LSDataSize size = CalcLSPairDataSize(
    static_cast<LoadStorePairOp>(op & LoadStorePairMask));
  Emit(op | Rt(rt) | Rt2(rt2) | RnSP(addr.base()) |
       ImmLSPair(addr.offset(), size));
}


// Memory instructions.
void Assembler::ldrb(const Register& rt, const MemOperand& src) {
  LoadStore(rt, src, LDRB_w);
}


void Assembler::strb(const Register& rt, const MemOperand& dst) {
  LoadStore(rt, dst, STRB_w);
}


void Assembler::ldrsb(const Register& rt, const MemOperand& src) {
  LoadStore(rt, src, rt.Is64Bits() ? LDRSB_x : LDRSB_w);
}


void Assembler::ldrh(const Register& rt, const MemOperand& src) {
  LoadStore(rt, src, LDRH_w);
}


void Assembler::strh(const Register& rt, const MemOperand& dst) {
  LoadStore(rt, dst, STRH_w);
}


void Assembler::ldrsh(const Register& rt, const MemOperand& src) {
  LoadStore(rt, src, rt.Is64Bits() ? LDRSH_x : LDRSH_w);
}


void Assembler::ldr(const CPURegister& rt, const MemOperand& src) {
  LoadStore(rt, src, LoadOpFor(rt));
}


void Assembler::str(const CPURegister& rt, const MemOperand& src) {
  LoadStore(rt, src, StoreOpFor(rt));
}


void Assembler::ldrsw(const Register& rt, const MemOperand& src) {
  VIXL_ASSERT(rt.Is64Bits());
  LoadStore(rt, src, LDRSW_x);
}


void Assembler::ldr(const Register& rt, uint64_t imm) {
  LoadLiteral(rt, imm, rt.Is64Bits() ? LDR_x_lit : LDR_w_lit);
}


void Assembler::ldr(const FPRegister& ft, double imm) {
  VIXL_ASSERT(ft.Is64Bits());
  LoadLiteral(ft, double_to_rawbits(imm), LDR_d_lit);
}


void Assembler::ldr(const FPRegister& ft, float imm) {
  VIXL_ASSERT(ft.Is32Bits());
  LoadLiteral(ft, float_to_rawbits(imm), LDR_s_lit);
}


void Assembler::mov(const Register& rd, const Register& rm) {
  // Moves involving the stack pointer are encoded as add immediate with
  // second operand of zero. Otherwise, orr with first operand zr is
  // used.
  if (rd.IsSP() || rm.IsSP()) {
    add(rd, rm, 0);
  } else {
    orr(rd, AppropriateZeroRegFor(rd), rm);
  }
}


void Assembler::mvn(const Register& rd, const Operand& operand) {
  orn(rd, AppropriateZeroRegFor(rd), operand);
}


void Assembler::mrs(const Register& rt, SystemRegister sysreg) {
  VIXL_ASSERT(rt.Is64Bits());
  Emit(MRS | ImmSystemRegister(sysreg) | Rt(rt));
}


void Assembler::msr(SystemRegister sysreg, const Register& rt) {
  VIXL_ASSERT(rt.Is64Bits());
  Emit(MSR | Rt(rt) | ImmSystemRegister(sysreg));
}


void Assembler::hint(SystemHint code) {
  Emit(HINT | ImmHint(code) | Rt(xzr));
}


void Assembler::dmb(BarrierDomain domain, BarrierType type) {
  Emit(DMB | ImmBarrierDomain(domain) | ImmBarrierType(type));
}


void Assembler::dsb(BarrierDomain domain, BarrierType type) {
  Emit(DSB | ImmBarrierDomain(domain) | ImmBarrierType(type));
}


void Assembler::isb() {
  Emit(ISB | ImmBarrierDomain(FullSystem) | ImmBarrierType(BarrierAll));
}


void Assembler::fmov(const FPRegister& fd, double imm) {
  VIXL_ASSERT(fd.Is64Bits());
  VIXL_ASSERT(IsImmFP64(imm));
  Emit(FMOV_d_imm | Rd(fd) | ImmFP64(imm));
}


void Assembler::fmov(const FPRegister& fd, float imm) {
  VIXL_ASSERT(fd.Is32Bits());
  VIXL_ASSERT(IsImmFP32(imm));
  Emit(FMOV_s_imm | Rd(fd) | ImmFP32(imm));
}


void Assembler::fmov(const Register& rd, const FPRegister& fn) {
  VIXL_ASSERT(rd.size() == fn.size());
  FPIntegerConvertOp op = rd.Is32Bits() ? FMOV_ws : FMOV_xd;
  Emit(op | Rd(rd) | Rn(fn));
}


void Assembler::fmov(const FPRegister& fd, const Register& rn) {
  VIXL_ASSERT(fd.size() == rn.size());
  FPIntegerConvertOp op = fd.Is32Bits() ? FMOV_sw : FMOV_dx;
  Emit(op | Rd(fd) | Rn(rn));
}


void Assembler::fmov(const FPRegister& fd, const FPRegister& fn) {
  VIXL_ASSERT(fd.size() == fn.size());
  Emit(FPType(fd) | FMOV | Rd(fd) | Rn(fn));
}


void Assembler::fadd(const FPRegister& fd,
                     const FPRegister& fn,
                     const FPRegister& fm) {
  FPDataProcessing2Source(fd, fn, fm, FADD);
}


void Assembler::fsub(const FPRegister& fd,
                     const FPRegister& fn,
                     const FPRegister& fm) {
  FPDataProcessing2Source(fd, fn, fm, FSUB);
}


void Assembler::fmul(const FPRegister& fd,
                     const FPRegister& fn,
                     const FPRegister& fm) {
  FPDataProcessing2Source(fd, fn, fm, FMUL);
}


void Assembler::fmadd(const FPRegister& fd,
                      const FPRegister& fn,
                      const FPRegister& fm,
                      const FPRegister& fa) {
  FPDataProcessing3Source(fd, fn, fm, fa, fd.Is32Bits() ? FMADD_s : FMADD_d);
}


void Assembler::fmsub(const FPRegister& fd,
                      const FPRegister& fn,
                      const FPRegister& fm,
                      const FPRegister& fa) {
  FPDataProcessing3Source(fd, fn, fm, fa, fd.Is32Bits() ? FMSUB_s : FMSUB_d);
}


void Assembler::fnmadd(const FPRegister& fd,
                       const FPRegister& fn,
                       const FPRegister& fm,
                       const FPRegister& fa) {
  FPDataProcessing3Source(fd, fn, fm, fa, fd.Is32Bits() ? FNMADD_s : FNMADD_d);
}


void Assembler::fnmsub(const FPRegister& fd,
                       const FPRegister& fn,
                       const FPRegister& fm,
                       const FPRegister& fa) {
  FPDataProcessing3Source(fd, fn, fm, fa, fd.Is32Bits() ? FNMSUB_s : FNMSUB_d);
}


void Assembler::fdiv(const FPRegister& fd,
                     const FPRegister& fn,
                     const FPRegister& fm) {
  FPDataProcessing2Source(fd, fn, fm, FDIV);
}


void Assembler::fmax(const FPRegister& fd,
                     const FPRegister& fn,
                     const FPRegister& fm) {
  FPDataProcessing2Source(fd, fn, fm, FMAX);
}


void Assembler::fmaxnm(const FPRegister& fd,
                       const FPRegister& fn,
                       const FPRegister& fm) {
  FPDataProcessing2Source(fd, fn, fm, FMAXNM);
}


void Assembler::fmin(const FPRegister& fd,
                     const FPRegister& fn,
                     const FPRegister& fm) {
  FPDataProcessing2Source(fd, fn, fm, FMIN);
}


void Assembler::fminnm(const FPRegister& fd,
                       const FPRegister& fn,
                       const FPRegister& fm) {
  FPDataProcessing2Source(fd, fn, fm, FMINNM);
}


void Assembler::fabs(const FPRegister& fd,
                     const FPRegister& fn) {
  VIXL_ASSERT(fd.SizeInBits() == fn.SizeInBits());
  FPDataProcessing1Source(fd, fn, FABS);
}


void Assembler::fneg(const FPRegister& fd,
                     const FPRegister& fn) {
  VIXL_ASSERT(fd.SizeInBits() == fn.SizeInBits());
  FPDataProcessing1Source(fd, fn, FNEG);
}


void Assembler::fsqrt(const FPRegister& fd,
                      const FPRegister& fn) {
  VIXL_ASSERT(fd.SizeInBits() == fn.SizeInBits());
  FPDataProcessing1Source(fd, fn, FSQRT);
}


void Assembler::frinta(const FPRegister& fd,
                       const FPRegister& fn) {
  VIXL_ASSERT(fd.SizeInBits() == fn.SizeInBits());
  FPDataProcessing1Source(fd, fn, FRINTA);
}


void Assembler::frintm(const FPRegister& fd,
                       const FPRegister& fn) {
  VIXL_ASSERT(fd.SizeInBits() == fn.SizeInBits());
  FPDataProcessing1Source(fd, fn, FRINTM);
}


void Assembler::frintn(const FPRegister& fd,
                       const FPRegister& fn) {
  VIXL_ASSERT(fd.SizeInBits() == fn.SizeInBits());
  FPDataProcessing1Source(fd, fn, FRINTN);
}


void Assembler::frintz(const FPRegister& fd,
                       const FPRegister& fn) {
  VIXL_ASSERT(fd.SizeInBits() == fn.SizeInBits());
  FPDataProcessing1Source(fd, fn, FRINTZ);
}


void Assembler::fcmp(const FPRegister& fn,
                     const FPRegister& fm) {
  VIXL_ASSERT(fn.size() == fm.size());
  Emit(FPType(fn) | FCMP | Rm(fm) | Rn(fn));
}


void Assembler::fcmp(const FPRegister& fn,
                     double value) {
  USE(value);
  // Although the fcmp instruction can strictly only take an immediate value of
  // +0.0, we don't need to check for -0.0 because the sign of 0.0 doesn't
  // affect the result of the comparison.
  VIXL_ASSERT(value == 0.0);
  Emit(FPType(fn) | FCMP_zero | Rn(fn));
}


void Assembler::fccmp(const FPRegister& fn,
                      const FPRegister& fm,
                      StatusFlags nzcv,
                      Condition cond) {
  VIXL_ASSERT(fn.size() == fm.size());
  Emit(FPType(fn) | FCCMP | Rm(fm) | Cond(cond) | Rn(fn) | Nzcv(nzcv));
}


void Assembler::fcsel(const FPRegister& fd,
                      const FPRegister& fn,
                      const FPRegister& fm,
                      Condition cond) {
  VIXL_ASSERT(fd.size() == fn.size());
  VIXL_ASSERT(fd.size() == fm.size());
  Emit(FPType(fd) | FCSEL | Rm(fm) | Cond(cond) | Rn(fn) | Rd(fd));
}


void Assembler::FPConvertToInt(const Register& rd,
                               const FPRegister& fn,
                               FPIntegerConvertOp op) {
  Emit(SF(rd) | FPType(fn) | op | Rn(fn) | Rd(rd));
}


void Assembler::fcvt(const FPRegister& fd,
                     const FPRegister& fn) {
  if (fd.Is64Bits()) {
    // Convert float to double.
    VIXL_ASSERT(fn.Is32Bits());
    FPDataProcessing1Source(fd, fn, FCVT_ds);
  } else {
    // Convert double to float.
    VIXL_ASSERT(fn.Is64Bits());
    FPDataProcessing1Source(fd, fn, FCVT_sd);
  }
}


void Assembler::fcvtau(const Register& rd, const FPRegister& fn) {
  FPConvertToInt(rd, fn, FCVTAU);
}


void Assembler::fcvtas(const Register& rd, const FPRegister& fn) {
  FPConvertToInt(rd, fn, FCVTAS);
}


void Assembler::fcvtmu(const Register& rd, const FPRegister& fn) {
  FPConvertToInt(rd, fn, FCVTMU);
}


void Assembler::fcvtms(const Register& rd, const FPRegister& fn) {
  FPConvertToInt(rd, fn, FCVTMS);
}


void Assembler::fcvtnu(const Register& rd, const FPRegister& fn) {
  FPConvertToInt(rd, fn, FCVTNU);
}


void Assembler::fcvtns(const Register& rd, const FPRegister& fn) {
  FPConvertToInt(rd, fn, FCVTNS);
}


void Assembler::fcvtzu(const Register& rd, const FPRegister& fn) {
  FPConvertToInt(rd, fn, FCVTZU);
}


void Assembler::fcvtzs(const Register& rd, const FPRegister& fn) {
  FPConvertToInt(rd, fn, FCVTZS);
}


void Assembler::scvtf(const FPRegister& fd,
                      const Register& rn,
                      unsigned fbits) {
  if (fbits == 0) {
    Emit(SF(rn) | FPType(fd) | SCVTF | Rn(rn) | Rd(fd));
  } else {
    Emit(SF(rn) | FPType(fd) | SCVTF_fixed | FPScale(64 - fbits) | Rn(rn) |
         Rd(fd));
  }
}


void Assembler::ucvtf(const FPRegister& fd,
                      const Register& rn,
                      unsigned fbits) {
  if (fbits == 0) {
    Emit(SF(rn) | FPType(fd) | UCVTF | Rn(rn) | Rd(fd));
  } else {
    Emit(SF(rn) | FPType(fd) | UCVTF_fixed | FPScale(64 - fbits) | Rn(rn) |
         Rd(fd));
  }
}


// Note:
// Below, a difference in case for the same letter indicates a
// negated bit.
// If b is 1, then B is 0.
Instr Assembler::ImmFP32(float imm) {
  VIXL_ASSERT(IsImmFP32(imm));
  // bits: aBbb.bbbc.defg.h000.0000.0000.0000.0000
  uint32_t bits = float_to_rawbits(imm);
  // bit7: a000.0000
  uint32_t bit7 = ((bits >> 31) & 0x1) << 7;
  // bit6: 0b00.0000
  uint32_t bit6 = ((bits >> 29) & 0x1) << 6;
  // bit5_to_0: 00cd.efgh
  uint32_t bit5_to_0 = (bits >> 19) & 0x3f;

  return (bit7 | bit6 | bit5_to_0) << ImmFP_offset;
}


Instr Assembler::ImmFP64(double imm) {
  VIXL_ASSERT(IsImmFP64(imm));
  // bits: aBbb.bbbb.bbcd.efgh.0000.0000.0000.0000
  //       0000.0000.0000.0000.0000.0000.0000.0000
  uint64_t bits = double_to_rawbits(imm);
  // bit7: a000.0000
  uint32_t bit7 = ((bits >> 63) & 0x1) << 7;
  // bit6: 0b00.0000
  uint32_t bit6 = ((bits >> 61) & 0x1) << 6;
  // bit5_to_0: 00cd.efgh
  uint32_t bit5_to_0 = (bits >> 48) & 0x3f;

  return (bit7 | bit6 | bit5_to_0) << ImmFP_offset;
}


// Code generation helpers.
void Assembler::MoveWide(const Register& rd,
                         uint64_t imm,
                         int shift,
                         MoveWideImmediateOp mov_op) {
  if (shift >= 0) {
    // Explicit shift specified.
    VIXL_ASSERT((shift == 0) || (shift == 16) ||
                (shift == 32) || (shift == 48));
    VIXL_ASSERT(rd.Is64Bits() || (shift == 0) || (shift == 16));
    shift /= 16;
  } else {
    // Calculate a new immediate and shift combination to encode the immediate
    // argument.
    shift = 0;
    if ((imm & UINT64_C(0xffffffffffff0000)) == 0) {
      // Nothing to do.
    } else if ((imm & UINT64_C(0xffffffff0000ffff)) == 0) {
      imm >>= 16;
      shift = 1;
    } else if ((imm & UINT64_C(0xffff0000ffffffff)) == 0) {
      VIXL_ASSERT(rd.Is64Bits());
      imm >>= 32;
      shift = 2;
    } else if ((imm & UINT64_C(0x0000ffffffffffff)) == 0) {
      VIXL_ASSERT(rd.Is64Bits());
      imm >>= 48;
      shift = 3;
    }
  }

  VIXL_ASSERT(is_uint16(imm));

  Emit(SF(rd) | MoveWideImmediateFixed | mov_op |
       Rd(rd) | ImmMoveWide(imm) | ShiftMoveWide(shift));
}


void Assembler::AddSub(const Register& rd,
                       const Register& rn,
                       const Operand& operand,
                       FlagsUpdate S,
                       AddSubOp op) {
  VIXL_ASSERT(rd.size() == rn.size());
  if (operand.IsImmediate()) {
    int64_t immediate = operand.immediate();
    VIXL_ASSERT(IsImmAddSub(immediate));
    Instr dest_reg = (S == SetFlags) ? Rd(rd) : RdSP(rd);
    Emit(SF(rd) | AddSubImmediateFixed | op | Flags(S) |
         ImmAddSub(immediate) | dest_reg | RnSP(rn));
  } else if (operand.IsShiftedRegister()) {
    VIXL_ASSERT(operand.reg().size() == rd.size());
    VIXL_ASSERT(operand.shift() != ROR);

    // For instructions of the form:
    //   add/sub   wsp, <Wn>, <Wm> [, LSL #0-3 ]
    //   add/sub   <Wd>, wsp, <Wm> [, LSL #0-3 ]
    //   add/sub   wsp, wsp, <Wm> [, LSL #0-3 ]
    //   adds/subs <Wd>, wsp, <Wm> [, LSL #0-3 ]
    // or their 64-bit register equivalents, convert the operand from shifted to
    // extended register mode, and emit an add/sub extended instruction.
    if (rn.IsSP() || rd.IsSP()) {
      VIXL_ASSERT(!(rd.IsSP() && (S == SetFlags)));
      DataProcExtendedRegister(rd, rn, operand.ToExtendedRegister(), S,
                               AddSubExtendedFixed | op);
    } else {
      DataProcShiftedRegister(rd, rn, operand, S, AddSubShiftedFixed | op);
    }
  } else {
    VIXL_ASSERT(operand.IsExtendedRegister());
    DataProcExtendedRegister(rd, rn, operand, S, AddSubExtendedFixed | op);
  }
}


void Assembler::AddSubWithCarry(const Register& rd,
                                const Register& rn,
                                const Operand& operand,
                                FlagsUpdate S,
                                AddSubWithCarryOp op) {
  VIXL_ASSERT(rd.size() == rn.size());
  VIXL_ASSERT(rd.size() == operand.reg().size());
  VIXL_ASSERT(operand.IsShiftedRegister() && (operand.shift_amount() == 0));
  Emit(SF(rd) | op | Flags(S) | Rm(operand.reg()) | Rn(rn) | Rd(rd));
}


void Assembler::hlt(int code) {
  VIXL_ASSERT(is_uint16(code));
  Emit(HLT | ImmException(code));
}


void Assembler::brk(int code) {
  VIXL_ASSERT(is_uint16(code));
  Emit(BRK | ImmException(code));
}


void Assembler::Logical(const Register& rd,
                        const Register& rn,
                        const Operand& operand,
                        LogicalOp op) {
  VIXL_ASSERT(rd.size() == rn.size());
  if (operand.IsImmediate()) {
    int64_t immediate = operand.immediate();
    unsigned reg_size = rd.size();

    VIXL_ASSERT(immediate != 0);
    VIXL_ASSERT(immediate != -1);
    VIXL_ASSERT(rd.Is64Bits() || is_uint32(immediate));

    // If the operation is NOT, invert the operation and immediate.
    if ((op & NOT) == NOT) {
      op = static_cast<LogicalOp>(op & ~NOT);
      immediate = rd.Is64Bits() ? ~immediate : (~immediate & kWRegMask);
    }

    unsigned n, imm_s, imm_r;
    if (IsImmLogical(immediate, reg_size, &n, &imm_s, &imm_r)) {
      // Immediate can be encoded in the instruction.
      LogicalImmediate(rd, rn, n, imm_s, imm_r, op);
    } else {
      // This case is handled in the macro assembler.
      VIXL_UNREACHABLE();
    }
  } else {
    VIXL_ASSERT(operand.IsShiftedRegister());
    VIXL_ASSERT(operand.reg().size() == rd.size());
    Instr dp_op = static_cast<Instr>(op | LogicalShiftedFixed);
    DataProcShiftedRegister(rd, rn, operand, LeaveFlags, dp_op);
  }
}


void Assembler::LogicalImmediate(const Register& rd,
                                 const Register& rn,
                                 unsigned n,
                                 unsigned imm_s,
                                 unsigned imm_r,
                                 LogicalOp op) {
  unsigned reg_size = rd.size();
  Instr dest_reg = (op == ANDS) ? Rd(rd) : RdSP(rd);
  Emit(SF(rd) | LogicalImmediateFixed | op | BitN(n, reg_size) |
       ImmSetBits(imm_s, reg_size) | ImmRotate(imm_r, reg_size) | dest_reg |
       Rn(rn));
}


void Assembler::ConditionalCompare(const Register& rn,
                                   const Operand& operand,
                                   StatusFlags nzcv,
                                   Condition cond,
                                   ConditionalCompareOp op) {
  Instr ccmpop;
  if (operand.IsImmediate()) {
    int64_t immediate = operand.immediate();
    VIXL_ASSERT(IsImmConditionalCompare(immediate));
    ccmpop = ConditionalCompareImmediateFixed | op | ImmCondCmp(immediate);
  } else {
    VIXL_ASSERT(operand.IsShiftedRegister() && (operand.shift_amount() == 0));
    ccmpop = ConditionalCompareRegisterFixed | op | Rm(operand.reg());
  }
  Emit(SF(rn) | ccmpop | Cond(cond) | Rn(rn) | Nzcv(nzcv));
}


void Assembler::DataProcessing1Source(const Register& rd,
                                      const Register& rn,
                                      DataProcessing1SourceOp op) {
  VIXL_ASSERT(rd.size() == rn.size());
  Emit(SF(rn) | op | Rn(rn) | Rd(rd));
}


void Assembler::FPDataProcessing1Source(const FPRegister& fd,
                                        const FPRegister& fn,
                                        FPDataProcessing1SourceOp op) {
  Emit(FPType(fn) | op | Rn(fn) | Rd(fd));
}


void Assembler::FPDataProcessing2Source(const FPRegister& fd,
                                        const FPRegister& fn,
                                        const FPRegister& fm,
                                        FPDataProcessing2SourceOp op) {
  VIXL_ASSERT(fd.size() == fn.size());
  VIXL_ASSERT(fd.size() == fm.size());
  Emit(FPType(fd) | op | Rm(fm) | Rn(fn) | Rd(fd));
}


void Assembler::FPDataProcessing3Source(const FPRegister& fd,
                                        const FPRegister& fn,
                                        const FPRegister& fm,
                                        const FPRegister& fa,
                                        FPDataProcessing3SourceOp op) {
  VIXL_ASSERT(AreSameSizeAndType(fd, fn, fm, fa));
  Emit(FPType(fd) | op | Rm(fm) | Rn(fn) | Rd(fd) | Ra(fa));
}


void Assembler::EmitShift(const Register& rd,
                          const Register& rn,
                          Shift shift,
                          unsigned shift_amount) {
  switch (shift) {
    case LSL:
      lsl(rd, rn, shift_amount);
      break;
    case LSR:
      lsr(rd, rn, shift_amount);
      break;
    case ASR:
      asr(rd, rn, shift_amount);
      break;
    case ROR:
      ror(rd, rn, shift_amount);
      break;
    default:
      VIXL_UNREACHABLE();
  }
}


void Assembler::EmitExtendShift(const Register& rd,
                                const Register& rn,
                                Extend extend,
                                unsigned left_shift) {
  VIXL_ASSERT(rd.size() >= rn.size());
  unsigned reg_size = rd.size();
  // Use the correct size of register.
  Register rn_ = Register(rn.code(), rd.size());
  // Bits extracted are high_bit:0.
  unsigned high_bit = (8 << (extend & 0x3)) - 1;
  // Number of bits left in the result that are not introduced by the shift.
  unsigned non_shift_bits = (reg_size - left_shift) & (reg_size - 1);

  if ((non_shift_bits > high_bit) || (non_shift_bits == 0)) {
    switch (extend) {
      case UXTB:
      case UXTH:
      case UXTW: ubfm(rd, rn_, non_shift_bits, high_bit); break;
      case SXTB:
      case SXTH:
      case SXTW: sbfm(rd, rn_, non_shift_bits, high_bit); break;
      case UXTX:
      case SXTX: {
        VIXL_ASSERT(rn.size() == kXRegSize);
        // Nothing to extend. Just shift.
        lsl(rd, rn_, left_shift);
        break;
      }
      default: VIXL_UNREACHABLE();
    }
  } else {
    // No need to extend as the extended bits would be shifted away.
    lsl(rd, rn_, left_shift);
  }
}


void Assembler::DataProcShiftedRegister(const Register& rd,
                                        const Register& rn,
                                        const Operand& operand,
                                        FlagsUpdate S,
                                        Instr op) {
  VIXL_ASSERT(operand.IsShiftedRegister());
  VIXL_ASSERT(rn.Is64Bits() || (rn.Is32Bits() &&
              is_uint5(operand.shift_amount())));
  Emit(SF(rd) | op | Flags(S) |
       ShiftDP(operand.shift()) | ImmDPShift(operand.shift_amount()) |
       Rm(operand.reg()) | Rn(rn) | Rd(rd));
}


void Assembler::DataProcExtendedRegister(const Register& rd,
                                         const Register& rn,
                                         const Operand& operand,
                                         FlagsUpdate S,
                                         Instr op) {
  Instr dest_reg = (S == SetFlags) ? Rd(rd) : RdSP(rd);
  Emit(SF(rd) | op | Flags(S) | Rm(operand.reg()) |
       ExtendMode(operand.extend()) | ImmExtendShift(operand.shift_amount()) |
       dest_reg | RnSP(rn));
}


bool Assembler::IsImmAddSub(int64_t immediate) {
  return is_uint12(immediate) ||
         (is_uint12(immediate >> 12) && ((immediate & 0xfff) == 0));
}

void Assembler::LoadStore(const CPURegister& rt,
                          const MemOperand& addr,
                          LoadStoreOp op) {
  Instr memop = op | Rt(rt) | RnSP(addr.base());
  ptrdiff_t offset = addr.offset();

  if (addr.IsImmediateOffset()) {
    LSDataSize size = CalcLSDataSize(op);
    if (IsImmLSScaled(offset, size)) {
      // Use the scaled addressing mode.
      Emit(LoadStoreUnsignedOffsetFixed | memop |
           ImmLSUnsigned(offset >> size));
    } else if (IsImmLSUnscaled(offset)) {
      // Use the unscaled addressing mode.
      Emit(LoadStoreUnscaledOffsetFixed | memop | ImmLS(offset));
    } else {
      // This case is handled in the macro assembler.
      VIXL_UNREACHABLE();
    }
  } else if (addr.IsRegisterOffset()) {
    Extend ext = addr.extend();
    Shift shift = addr.shift();
    unsigned shift_amount = addr.shift_amount();

    // LSL is encoded in the option field as UXTX.
    if (shift == LSL) {
      ext = UXTX;
    }

    // Shifts are encoded in one bit, indicating a left shift by the memory
    // access size.
    VIXL_ASSERT((shift_amount == 0) ||
           (shift_amount == static_cast<unsigned>(CalcLSDataSize(op))));
    Emit(LoadStoreRegisterOffsetFixed | memop | Rm(addr.regoffset()) |
         ExtendMode(ext) | ImmShiftLS((shift_amount > 0) ? 1 : 0));
  } else {
    if (IsImmLSUnscaled(offset)) {
      if (addr.IsPreIndex()) {
        Emit(LoadStorePreIndexFixed | memop | ImmLS(offset));
      } else {
        VIXL_ASSERT(addr.IsPostIndex());
        Emit(LoadStorePostIndexFixed | memop | ImmLS(offset));
      }
    } else {
      // This case is handled in the macro assembler.
      VIXL_UNREACHABLE();
    }
  }
}


bool Assembler::IsImmLSUnscaled(ptrdiff_t offset) {
  return is_int9(offset);
}


bool Assembler::IsImmLSScaled(ptrdiff_t offset, LSDataSize size) {
  bool offset_is_size_multiple = (((offset >> size) << size) == offset);
  return offset_is_size_multiple && is_uint12(offset >> size);
}


void Assembler::LoadLiteral(const CPURegister& rt,
                            uint64_t imm,
                            LoadLiteralOp op) {
  VIXL_ASSERT(is_int32(imm) || is_uint32(imm) || (rt.Is64Bits()));

  BlockLiteralPoolScope scope(this);
  RecordLiteral(imm, rt.SizeInBytes());
  Emit(op | ImmLLiteral(0) | Rt(rt));
}


// Test if a given value can be encoded in the immediate field of a logical
// instruction.
// If it can be encoded, the function returns true, and values pointed to by n,
// imm_s and imm_r are updated with immediates encoded in the format required
// by the corresponding fields in the logical instruction.
// If it can not be encoded, the function returns false, and the values pointed
// to by n, imm_s and imm_r are undefined.
bool Assembler::IsImmLogical(uint64_t value,
                             unsigned width,
                             unsigned* n,
                             unsigned* imm_s,
                             unsigned* imm_r) {
  VIXL_ASSERT((n != NULL) && (imm_s != NULL) && (imm_r != NULL));
  VIXL_ASSERT((width == kWRegSize) || (width == kXRegSize));

  // Logical immediates are encoded using parameters n, imm_s and imm_r using
  // the following table:
  //
  //  N   imms    immr    size        S             R
  //  1  ssssss  rrrrrr    64    UInt(ssssss)  UInt(rrrrrr)
  //  0  0sssss  xrrrrr    32    UInt(sssss)   UInt(rrrrr)
  //  0  10ssss  xxrrrr    16    UInt(ssss)    UInt(rrrr)
  //  0  110sss  xxxrrr     8    UInt(sss)     UInt(rrr)
  //  0  1110ss  xxxxrr     4    UInt(ss)      UInt(rr)
  //  0  11110s  xxxxxr     2    UInt(s)       UInt(r)
  // (s bits must not be all set)
  //
  // A pattern is constructed of size bits, where the least significant S+1
  // bits are set. The pattern is rotated right by R, and repeated across a
  // 32 or 64-bit value, depending on destination register width.
  //
  // To test if an arbitrary immediate can be encoded using this scheme, an
  // iterative algorithm is used.
  //
  // TODO: This code does not consider using X/W register overlap to support
  // 64-bit immediates where the top 32-bits are zero, and the bottom 32-bits
  // are an encodable logical immediate.

  // 1. If the value has all set or all clear bits, it can't be encoded.
  if ((value == 0) || (value == kXRegMask) ||
      ((width == kWRegSize) && (value == kWRegMask))) {
    return false;
  }

  unsigned lead_zero = CountLeadingZeros(value, width);
  unsigned lead_one = CountLeadingZeros(~value, width);
  unsigned trail_zero = CountTrailingZeros(value, width);
  unsigned trail_one = CountTrailingZeros(~value, width);
  unsigned set_bits = CountSetBits(value, width);

  // The fixed bits in the immediate s field.
  // If width == 64 (X reg), start at 0xFFFFFF80.
  // If width == 32 (W reg), start at 0xFFFFFFC0, as the iteration for 64-bit
  // widths won't be executed.
  int imm_s_fixed = (width == kXRegSize) ? -128 : -64;
  int imm_s_mask = 0x3F;

  for (;;) {
    // 2. If the value is two bits wide, it can be encoded.
    if (width == 2) {
      *n = 0;
      *imm_s = 0x3C;
      *imm_r = (value & 3) - 1;
      return true;
    }

    *n = (width == 64) ? 1 : 0;
    *imm_s = ((imm_s_fixed | (set_bits - 1)) & imm_s_mask);
    if ((lead_zero + set_bits) == width) {
      *imm_r = 0;
    } else {
      *imm_r = (lead_zero > 0) ? (width - trail_zero) : lead_one;
    }

    // 3. If the sum of leading zeros, trailing zeros and set bits is equal to
    //    the bit width of the value, it can be encoded.
    if (lead_zero + trail_zero + set_bits == width) {
      return true;
    }

    // 4. If the sum of leading ones, trailing ones and unset bits in the
    //    value is equal to the bit width of the value, it can be encoded.
    if (lead_one + trail_one + (width - set_bits) == width) {
      return true;
    }

    // 5. If the most-significant half of the bitwise value is equal to the
    //    least-significant half, return to step 2 using the least-significant
    //    half of the value.
    uint64_t mask = (UINT64_C(1) << (width >> 1)) - 1;
    if ((value & mask) == ((value >> (width >> 1)) & mask)) {
      width >>= 1;
      set_bits >>= 1;
      imm_s_fixed >>= 1;
      continue;
    }

    // 6. Otherwise, the value can't be encoded.
    return false;
  }
}

bool Assembler::IsImmConditionalCompare(int64_t immediate) {
  return is_uint5(immediate);
}


bool Assembler::IsImmFP32(float imm) {
  // Valid values will have the form:
  // aBbb.bbbc.defg.h000.0000.0000.0000.0000
  uint32_t bits = float_to_rawbits(imm);
  // bits[19..0] are cleared.
  if ((bits & 0x7ffff) != 0) {
    return false;
  }

  // bits[29..25] are all set or all cleared.
  uint32_t b_pattern = (bits >> 16) & 0x3e00;
  if (b_pattern != 0 && b_pattern != 0x3e00) {
    return false;
  }

  // bit[30] and bit[29] are opposite.
  if (((bits ^ (bits << 1)) & 0x40000000) == 0) {
    return false;
  }

  return true;
}


bool Assembler::IsImmFP64(double imm) {
  // Valid values will have the form:
  // aBbb.bbbb.bbcd.efgh.0000.0000.0000.0000
  // 0000.0000.0000.0000.0000.0000.0000.0000
  uint64_t bits = double_to_rawbits(imm);
  // bits[47..0] are cleared.
  if ((bits & INT64_C(0x0000ffffffffffff)) != 0) {
    return false;
  }

  // bits[61..54] are all set or all cleared.
  uint32_t b_pattern = (bits >> 48) & 0x3fc0;
  if ((b_pattern != 0) && (b_pattern != 0x3fc0)) {
    return false;
  }

  // bit[62] and bit[61] are opposite.
  if (((bits ^ (bits << 1)) & (UINT64_C(1) << 62)) == 0) {
    return false;
  }

  return true;
}


LoadStoreOp Assembler::LoadOpFor(const CPURegister& rt) {
  VIXL_ASSERT(rt.IsValid());
  if (rt.IsRegister()) {
    return rt.Is64Bits() ? LDR_x : LDR_w;
  } else {
    VIXL_ASSERT(rt.IsFPRegister());
    return rt.Is64Bits() ? LDR_d : LDR_s;
  }
}


LoadStorePairOp Assembler::LoadPairOpFor(const CPURegister& rt,
    const CPURegister& rt2) {
  VIXL_ASSERT(AreSameSizeAndType(rt, rt2));
  USE(rt2);
  if (rt.IsRegister()) {
    return rt.Is64Bits() ? LDP_x : LDP_w;
  } else {
    VIXL_ASSERT(rt.IsFPRegister());
    return rt.Is64Bits() ? LDP_d : LDP_s;
  }
}


LoadStoreOp Assembler::StoreOpFor(const CPURegister& rt) {
  VIXL_ASSERT(rt.IsValid());
  if (rt.IsRegister()) {
    return rt.Is64Bits() ? STR_x : STR_w;
  } else {
    VIXL_ASSERT(rt.IsFPRegister());
    return rt.Is64Bits() ? STR_d : STR_s;
  }
}


LoadStorePairOp Assembler::StorePairOpFor(const CPURegister& rt,
    const CPURegister& rt2) {
  VIXL_ASSERT(AreSameSizeAndType(rt, rt2));
  USE(rt2);
  if (rt.IsRegister()) {
    return rt.Is64Bits() ? STP_x : STP_w;
  } else {
    VIXL_ASSERT(rt.IsFPRegister());
    return rt.Is64Bits() ? STP_d : STP_s;
  }
}


LoadStorePairNonTemporalOp Assembler::LoadPairNonTemporalOpFor(
    const CPURegister& rt, const CPURegister& rt2) {
  VIXL_ASSERT(AreSameSizeAndType(rt, rt2));
  USE(rt2);
  if (rt.IsRegister()) {
    return rt.Is64Bits() ? LDNP_x : LDNP_w;
  } else {
    VIXL_ASSERT(rt.IsFPRegister());
    return rt.Is64Bits() ? LDNP_d : LDNP_s;
  }
}


LoadStorePairNonTemporalOp Assembler::StorePairNonTemporalOpFor(
    const CPURegister& rt, const CPURegister& rt2) {
  VIXL_ASSERT(AreSameSizeAndType(rt, rt2));
  USE(rt2);
  if (rt.IsRegister()) {
    return rt.Is64Bits() ? STNP_x : STNP_w;
  } else {
    VIXL_ASSERT(rt.IsFPRegister());
    return rt.Is64Bits() ? STNP_d : STNP_s;
  }
}


void Assembler::RecordLiteral(int64_t imm, unsigned size) {
  literals_.push_front(new Literal(pc_, imm, size));
}


// Check if a literal pool should be emitted. Currently a literal is emitted
// when:
//  * the distance to the first literal load handled by this pool is greater
//    than the recommended distance and the literal pool can be emitted without
//    generating a jump over it.
//  * the distance to the first literal load handled by this pool is greater
//    than twice the recommended distance.
// TODO: refine this heuristic using real world data.
void Assembler::CheckLiteralPool(LiteralPoolEmitOption option) {
  if (IsLiteralPoolBlocked()) {
    // Literal pool emission is forbidden, no point in doing further checks.
    return;
  }

  if (literals_.empty()) {
    // No literal pool to emit.
    next_literal_pool_check_ += kLiteralPoolCheckInterval;
    return;
  }

  intptr_t distance = pc_ - literals_.back()->pc_;
  if ((distance < kRecommendedLiteralPoolRange) ||
      ((option == JumpRequired) &&
       (distance < (2 * kRecommendedLiteralPoolRange)))) {
    // We prefer not to have to jump over the literal pool.
    next_literal_pool_check_ += kLiteralPoolCheckInterval;
    return;
  }

  EmitLiteralPool(option);
}


void Assembler::EmitLiteralPool(LiteralPoolEmitOption option) {
  // Prevent recursive calls while emitting the literal pool.
  BlockLiteralPoolScope scope(this);

  Label marker;
  Label start_of_pool;
  Label end_of_pool;

  if (option == JumpRequired) {
    b(&end_of_pool);
  }

  // Leave space for a literal pool marker. This is populated later, once the
  // size of the pool is known.
  bind(&marker);
  nop();

  // Now populate the literal pool.
  bind(&start_of_pool);
  std::list<Literal*>::iterator it;
  for (it = literals_.begin(); it != literals_.end(); it++) {
    // Update the load-literal instruction to point to this pool entry.
    Instruction* load_literal = (*it)->pc_;
    load_literal->SetImmLLiteral(pc_);
    // Copy the data into the pool.
    uint64_t value= (*it)->value_;
    unsigned size = (*it)->size_;
    VIXL_ASSERT((size == kXRegSizeInBytes) || (size == kWRegSizeInBytes));
    VIXL_ASSERT((pc_ + size) <= (buffer_ + buffer_size_));
    memcpy(pc_, &value, size);
    pc_ += size;
    delete *it;
  }
  literals_.clear();
  bind(&end_of_pool);

  // The pool size should always be a multiple of four bytes because that is the
  // scaling applied by the LDR(literal) instruction, even for X-register loads.
  VIXL_ASSERT((SizeOfCodeGeneratedSince(&start_of_pool) % 4) == 0);
  uint64_t pool_size = SizeOfCodeGeneratedSince(&start_of_pool) / 4;

  // Literal pool marker indicating the size in words of the literal pool.
  // We use a literal load to the zero register, the offset indicating the
  // size in words. This instruction can encode a large enough offset to span
  // the entire pool at its maximum size.
  Instr marker_instruction = LDR_x_lit | ImmLLiteral(pool_size) | Rt(xzr);
  memcpy(marker.target(), &marker_instruction, kInstructionSize);

  next_literal_pool_check_ = pc_ + kLiteralPoolCheckInterval;
}


// Return the size in bytes, required by the literal pool entries. This does
// not include any marker or branch over the literal pool itself.
size_t Assembler::LiteralPoolSize() {
  size_t size = 0;

  std::list<Literal*>::iterator it;
  for (it = literals_.begin(); it != literals_.end(); it++) {
    size += (*it)->size_;
  }

  return size;
}


bool AreAliased(const CPURegister& reg1, const CPURegister& reg2,
                const CPURegister& reg3, const CPURegister& reg4,
                const CPURegister& reg5, const CPURegister& reg6,
                const CPURegister& reg7, const CPURegister& reg8) {
  int number_of_valid_regs = 0;
  int number_of_valid_fpregs = 0;

  RegList unique_regs = 0;
  RegList unique_fpregs = 0;

  const CPURegister regs[] = {reg1, reg2, reg3, reg4, reg5, reg6, reg7, reg8};

  for (unsigned i = 0; i < sizeof(regs) / sizeof(regs[0]); i++) {
    if (regs[i].IsRegister()) {
      number_of_valid_regs++;
      unique_regs |= regs[i].Bit();
    } else if (regs[i].IsFPRegister()) {
      number_of_valid_fpregs++;
      unique_fpregs |= regs[i].Bit();
    } else {
      VIXL_ASSERT(!regs[i].IsValid());
    }
  }

  int number_of_unique_regs =
    CountSetBits(unique_regs, sizeof(unique_regs) * 8);
  int number_of_unique_fpregs =
    CountSetBits(unique_fpregs, sizeof(unique_fpregs) * 8);

  VIXL_ASSERT(number_of_valid_regs >= number_of_unique_regs);
  VIXL_ASSERT(number_of_valid_fpregs >= number_of_unique_fpregs);

  return (number_of_valid_regs != number_of_unique_regs) ||
         (number_of_valid_fpregs != number_of_unique_fpregs);
}


bool AreSameSizeAndType(const CPURegister& reg1, const CPURegister& reg2,
                        const CPURegister& reg3, const CPURegister& reg4,
                        const CPURegister& reg5, const CPURegister& reg6,
                        const CPURegister& reg7, const CPURegister& reg8) {
  VIXL_ASSERT(reg1.IsValid());
  bool match = true;
  match &= !reg2.IsValid() || reg2.IsSameSizeAndType(reg1);
  match &= !reg3.IsValid() || reg3.IsSameSizeAndType(reg1);
  match &= !reg4.IsValid() || reg4.IsSameSizeAndType(reg1);
  match &= !reg5.IsValid() || reg5.IsSameSizeAndType(reg1);
  match &= !reg6.IsValid() || reg6.IsSameSizeAndType(reg1);
  match &= !reg7.IsValid() || reg7.IsSameSizeAndType(reg1);
  match &= !reg8.IsValid() || reg8.IsSameSizeAndType(reg1);
  return match;
}


}  // namespace vixl
