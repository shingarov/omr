/*******************************************************************************
 * Copyright (c) 2000, 2016 IBM Corp. and others
 *
 * This program and the accompanying materials are made available under
 * the terms of the Eclipse Public License 2.0 which accompanies this
 * distribution and is available at http://eclipse.org/legal/epl-2.0
 * or the Apache License, Version 2.0 which accompanies this distribution
 * and is available at https://www.apache.org/licenses/LICENSE-2.0.
 *
 * This Source Code may also be made available under the following Secondary
 * Licenses when the conditions for such availability set forth in the
 * Eclipse Public License, v. 2.0 are satisfied: GNU General Public License,
 * version 2 with the GNU Classpath Exception [1] and GNU General Public
 * License, version 2 with the OpenJDK Assembly Exception [2].
 *
 * [1] https://www.gnu.org/software/classpath/license.html
 * [2] http://openjdk.java.net/legal/assembly-exception.html
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
 *******************************************************************************/

#include "codegen/RVInstruction.hpp"

#include <stddef.h>                               // for NULL
#include <stdint.h>                               // for int32_t, uint32_t, etc
#include <riscv.h>
#include "codegen/CodeGenerator.hpp"              // for CodeGenerator, etc
#include "codegen/InstOpCode.hpp"                 // for InstOpCode, etc
#include "codegen/Instruction.hpp"                // for Instruction
#include "codegen/Machine.hpp"                    // for Machine, etc
#include "codegen/MemoryReference.hpp"            // for MemoryReference
#include "codegen/RealRegister.hpp"               // for RealRegister, etc
#include "codegen/Register.hpp"                   // for Register
#include "codegen/RegisterConstants.hpp"          // for TR_RegisterKinds, etc
#include "codegen/RegisterDependency.hpp"
#include "codegen/RegisterDependencyStruct.hpp"   // for RegisterDependency
#include "codegen/Relocation.hpp"              // for TR::ExternalRelocation, etc
#include "compile/Compilation.hpp"             // for Compilation
#include "compile/ResolvedMethod.hpp"          // for TR_ResolvedMethod
#include "control/Options.hpp"
#include "control/Options_inlines.hpp"
#include "env/CompilerEnv.hpp"
#include "env/Processors.hpp"
#include "env/TRMemory.hpp"
#include "il/DataTypes.hpp"                       // for CONSTANT64
#include "il/ILOps.hpp"                           // for ILOpCode
#include "il/Node.hpp"                            // for Node
#include "il/symbol/LabelSymbol.hpp"              // for LabelSymbol
#include "infra/Assert.hpp"                       // for TR_ASSERT
#include "codegen/GenerateInstructions.hpp"
#include "codegen/ARM64OutOfLineCodeSection.hpp"

#define TR_RISCV_RTYPE(insn, rd, rs1, rs2) \
  ((insn) | ((rd) << OP_SH_RD) | ((rs1) << OP_SH_RS1) | ((rs2) << OP_SH_RS2))
#define TR_RISCV_ITYPE(insn, rd, rs1, imm) \
  ((insn) | ((rd) << OP_SH_RD) | ((rs1) << OP_SH_RS1) | ENCODE_ITYPE_IMM(imm))
#define TR_RISCV_STYPE(insn, rs1, rs2, imm) \
  ((insn) | ((rs1) << OP_SH_RS1) | ((rs2) << OP_SH_RS2) | ENCODE_STYPE_IMM(imm))
#define TR_RISCV_SBTYPE(insn, rs1, rs2, target) \
  ((insn) | ((rs1) << OP_SH_RS1) | ((rs2) << OP_SH_RS2) | ENCODE_SBTYPE_IMM(target))
#define TR_RISCV_UTYPE(insn, rd, bigimm) \
  ((insn) | ((rd) << OP_SH_RD) | ENCODE_UTYPE_IMM(bigimm))
#define TR_RISCV_UJTYPE(insn, rd, target) \
  ((insn) | ((rd) << OP_SH_RD) | ENCODE_UJTYPE_IMM(target))


uint8_t *TR::ItypeInstruction::generateBinaryEncoding() {
   uint8_t        *instructionStart = cg()->getBinaryBufferCursor();
   uint8_t        *cursor           = instructionStart;
   uint32_t *iPtr = (uint32_t*)instructionStart;

   *iPtr = TR_RISCV_ITYPE ((uint32_t)(getOpCode().getOpCodeBinaryEncoding()),
                         toRealRegister(getTargetRegister())->binaryRegCode(),
                         toRealRegister(getSource1Register())->binaryRegCode(),
                         getSourceImmediate());

   cursor += RISCV_INSTRUCTION_LENGTH;
   setBinaryLength(RISCV_INSTRUCTION_LENGTH);
   setBinaryEncoding(instructionStart);
   return cursor;
}

uint8_t *TR::LoadInstruction::generateBinaryEncoding() {
   uint8_t        *instructionStart = cg()->getBinaryBufferCursor();
   uint8_t        *cursor           = instructionStart;
   uint32_t *iPtr = (uint32_t*)instructionStart;

   TR_ASSERT(getMemoryIndex() == nullptr, "Unsupported addressing mode");

   *iPtr = TR_RISCV_ITYPE ((uint32_t)(getOpCode().getOpCodeBinaryEncoding()),
                         toRealRegister(getTargetRegister())->binaryRegCode(),
                         toRealRegister(getMemoryBase())->binaryRegCode(),
                         getMemoryReference()->getOffset(true));

   cursor += RISCV_INSTRUCTION_LENGTH;
   setBinaryLength(RISCV_INSTRUCTION_LENGTH);
   setBinaryEncoding(instructionStart);
   return cursor;
}


int32_t TR::LoadInstruction::estimateBinaryLength(int32_t currentEstimate)
   {
   setEstimatedBinaryLength(RISCV_INSTRUCTION_LENGTH);
   return currentEstimate + self()->getEstimatedBinaryLength();
   }

uint8_t *TR::StypeInstruction::generateBinaryEncoding() {
   uint8_t        *instructionStart = cg()->getBinaryBufferCursor();
   uint8_t        *cursor           = instructionStart;
   uint32_t *iPtr = (uint32_t*)instructionStart;

   *iPtr = TR_RISCV_STYPE ((uint32_t)(getOpCode().getOpCodeBinaryEncoding()),
                         toRealRegister(getSource1Register())->binaryRegCode(),
                         toRealRegister(getSource2Register())->binaryRegCode(),
                         getSourceImmediate());

   cursor += RISCV_INSTRUCTION_LENGTH;
   setBinaryLength(RISCV_INSTRUCTION_LENGTH);
   setBinaryEncoding(instructionStart);
   return cursor;
}

uint8_t *TR::StoreInstruction::generateBinaryEncoding() {
   uint8_t        *instructionStart = cg()->getBinaryBufferCursor();
   uint8_t        *cursor           = instructionStart;
   uint32_t *iPtr = (uint32_t*)instructionStart;

   TR_ASSERT(getMemoryIndex() == nullptr, "Unsupported addressing mode");

   *iPtr = TR_RISCV_STYPE ((uint32_t)(getOpCode().getOpCodeBinaryEncoding()),
                         toRealRegister(getMemoryBase())->binaryRegCode(),
                         toRealRegister(getSource1Register())->binaryRegCode(),
                         getMemoryReference()->getOffset(true));

   cursor += RISCV_INSTRUCTION_LENGTH;
   setBinaryLength(RISCV_INSTRUCTION_LENGTH);
   setBinaryEncoding(instructionStart);
   return cursor;
}


int32_t TR::StoreInstruction::estimateBinaryLength(int32_t currentEstimate)
   {
   setEstimatedBinaryLength(RISCV_INSTRUCTION_LENGTH);
   return currentEstimate + self()->getEstimatedBinaryLength();
   }



uint8_t *TR::BtypeInstruction::generateBinaryEncoding()
   {
   uint8_t        *instructionStart = cg()->getBinaryBufferCursor();
   uint8_t        *cursor           = instructionStart;
   uint32_t *iPtr = (uint32_t*)instructionStart;

   TR::LabelSymbol *label            = getLabelSymbol();


   *iPtr = TR_RISCV_SBTYPE ((uint32_t)(getOpCode().getOpCodeBinaryEncoding()),
                         toRealRegister(getSource1Register())->binaryRegCode(),
                         toRealRegister(getSource2Register())->binaryRegCode(),
                         0 /* to fix up in the future */ );

   if (label->getCodeLocation() != NULL) {
           // we already know the address
           int32_t delta = label->getCodeLocation() - cursor;
           *iPtr |= ENCODE_SBTYPE_IMM(delta);
   } else {
           cg()->addRelocation(new (cg()->trHeapMemory()) TR::LabelRelative16BitRelocation(cursor, label));
   }

   cursor += RISCV_INSTRUCTION_LENGTH;
   setBinaryLength(RISCV_INSTRUCTION_LENGTH);
   setBinaryEncoding(instructionStart);
   return cursor;
   }

bool TR::BtypeInstruction::refsRegister(TR::Register *reg)
   {
   return (reg == getSource1Register() || reg == getSource2Register());
   }

bool TR::BtypeInstruction::usesRegister(TR::Register *reg)
   {
   return (reg == getSource1Register() || reg == getSource2Register());
   }

bool TR::BtypeInstruction::defsRegister(TR::Register *reg)
   {
   return false;
   }

bool TR::BtypeInstruction::defsRealRegister(TR::Register *reg)
   {
   return false;
   }

void TR::BtypeInstruction::assignRegisters(TR_RegisterKinds kindToBeAssigned)
   {
   TR::Machine *machine = cg()->machine();
   TR::Register *source1Virtual = getSource1Register();
   TR::Register *source2Virtual = getSource2Register();

   if (getDependencyConditions())
      getDependencyConditions()->assignPostConditionRegisters(this, kindToBeAssigned, cg());

   source1Virtual->block();
   TR::RealRegister *assignedSource2Register = machine->assignOneRegister(this, source2Virtual);
   source1Virtual->unblock();

   source2Virtual->block();
   TR::RealRegister *assignedSource1Register = machine->assignOneRegister(this, source1Virtual);
   source2Virtual->unblock();

   if (getDependencyConditions())
      getDependencyConditions()->assignPreConditionRegisters(this->getPrev(), kindToBeAssigned, cg());

   setSource1Register(assignedSource1Register);
   setSource2Register(assignedSource2Register);
   }


uint8_t *TR::UtypeInstruction::generateBinaryEncoding() {
   uint8_t        *instructionStart = cg()->getBinaryBufferCursor();
   uint8_t        *cursor           = instructionStart;
   uint32_t *iPtr = (uint32_t*)instructionStart;

   *iPtr = TR_RISCV_UTYPE ((uint32_t)(getOpCode().getOpCodeBinaryEncoding()),
                         toRealRegister(getTargetRegister())->binaryRegCode(),
                         _imm);

   cursor += RISCV_INSTRUCTION_LENGTH;
   setBinaryLength(RISCV_INSTRUCTION_LENGTH);
   setBinaryEncoding(instructionStart);
   return cursor;
}

uint8_t *TR::JtypeInstruction::generateBinaryEncoding() {
   uint8_t        *instructionStart = cg()->getBinaryBufferCursor();
   uint8_t        *cursor           = instructionStart;
   uint32_t *iPtr = (uint32_t*)instructionStart;
   ptrdiff_t offset = 0;

   // First, compute the value of _imm
   {
      TR::ResolvedMethodSymbol *sym = getSymbolReference()->getSymbol()->getResolvedMethodSymbol();
      TR_ResolvedMethod *resolvedMethod = sym == NULL ? NULL : sym->getResolvedMethod();

      if (resolvedMethod != NULL && resolvedMethod->isSameMethod(cg()->comp()->getCurrentMethod()))
         {
         offset = cg()->getCodeStart() - cursor;
         // to guard against silent truncation
         TR_ASSERT( (INT32_MIN <= offset) && (offset <= INT32_MAX), "Jump offset out of range" );
         }
      else
         {
         TR_ASSERT(0, "Non-recursive calls not (yet) supported");
         }
   }


   TR_ASSERT(VALID_UJTYPE_IMM(offset), "Jump offset out of range");
   *iPtr = TR_RISCV_UJTYPE ((uint32_t)(getOpCode().getOpCodeBinaryEncoding()),
                         toRealRegister(getTargetRegister(0))->binaryRegCode(),
                         offset);

   cursor += RISCV_INSTRUCTION_LENGTH;
   setBinaryLength(RISCV_INSTRUCTION_LENGTH);
   setBinaryEncoding(instructionStart);
   return cursor;

}

TR::Instruction *generateITYPE( TR::InstOpCode::Mnemonic op,
                                TR::Node          *n,
                                TR::Register      *treg,
                                TR::Register      *sreg,
                                uint32_t          imm,
                                TR::CodeGenerator *cg,
                                TR::Instruction   *previous)
   {
   if (previous)
      return new (cg->trHeapMemory()) TR::ItypeInstruction(op, n, treg, sreg, imm, previous, cg);
   else
      return new (cg->trHeapMemory()) TR::ItypeInstruction(op, n, treg, sreg, imm, cg);
   }

TR::Instruction *generateLOAD(  TR::InstOpCode::Mnemonic op,
                                TR::Node          *n,
                                TR::Register      *trgReg,
                                TR::MemoryReference *memRef,
                                TR::CodeGenerator *cg,
                                TR::Instruction   *previous)
   {
   if (previous)
      return new (cg->trHeapMemory()) TR::LoadInstruction(op, n, trgReg, memRef, previous, cg);
   else
      return new (cg->trHeapMemory()) TR::LoadInstruction(op, n, trgReg, memRef, cg);
   }

TR::Instruction *generateSTYPE( TR::InstOpCode::Mnemonic op,
                                TR::Node          *n,
                                TR::Register      *s1reg,
                                TR::Register      *s2reg,
                                uint32_t          imm,
                                TR::CodeGenerator *cg,
                                TR::Instruction   *previous)
   {
   if (previous)
      return new (cg->trHeapMemory()) TR::StypeInstruction(op, n, s1reg, s2reg, imm, previous, cg);
   else
      return new (cg->trHeapMemory()) TR::StypeInstruction(op, n, s1reg, s2reg, imm, cg);
   }

TR::Instruction *generateSTORE( TR::InstOpCode::Mnemonic op,
                                TR::Node          *n,
                                TR::MemoryReference *memRef,
                                TR::Register      *srcReg,
                                TR::CodeGenerator *cg,
                                TR::Instruction   *previous)
   {
   if (previous)
      return new (cg->trHeapMemory()) TR::StoreInstruction(op, n, memRef, srcReg, previous, cg);
   else
      return new (cg->trHeapMemory()) TR::StoreInstruction(op, n, memRef, srcReg, cg);
   }

TR::Instruction *generateBTYPE( TR::InstOpCode::Mnemonic op,
                                TR::Node          *n,
                                TR::LabelSymbol   *sym,
                                TR::Register      *src1,
                                TR::Register      *src2,
                                TR::CodeGenerator *cg,
                                TR::Instruction   *previous)
   {
   if (previous)
      return new (cg->trHeapMemory()) TR::BtypeInstruction(op, n, sym, src1, src2, previous, cg);
   else
      return new (cg->trHeapMemory()) TR::BtypeInstruction(op, n, sym, src1, src2, cg);
   }

TR::Instruction *generateUTYPE( TR::InstOpCode::Mnemonic op,
                                TR::Node          *n,
                                uint32_t          imm,
                                TR::Register      *reg,
                                TR::CodeGenerator *cg,
                                TR::Instruction   *previous)
   {
   if (previous)
      return new (cg->trHeapMemory()) TR::UtypeInstruction(op, n, imm, reg, previous, cg);
   else
      return new (cg->trHeapMemory()) TR::UtypeInstruction(op, n, imm, reg, cg);
   }

TR::Instruction *generateJTYPE( TR::InstOpCode::Mnemonic op,
                                TR::Node          *n,
                                TR::Register      *trgReg,
                                uintptr_t        imm,
                                TR::RegisterDependencyConditions *cond,
                                TR::SymbolReference *sr,
                                TR::Snippet       *s,
                                TR::CodeGenerator *cg,
                                TR::Instruction   *previous)
   {
   if (previous)
      return new (cg->trHeapMemory()) TR::JtypeInstruction(op, n, trgReg, imm, cond, sr, s, previous, cg);
   else
      return new (cg->trHeapMemory()) TR::JtypeInstruction(op, n, trgReg, imm, cond, sr, s, cg);
   }
