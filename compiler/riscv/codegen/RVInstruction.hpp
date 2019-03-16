/*******************************************************************************
 * Copyright (c) 2000, 2019 Boris Shingarov, Jan Vrany
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

#ifndef RVINSTRUCTION_INCL
#define RVINSTRUCTION_INCL

#include "codegen/ARM64Instruction.hpp"

#define RISCV_INSTRUCTION_LENGTH 4

namespace TR
{

class ItypeInstruction : public ARM64Trg1Src1Instruction
   {

   uint32_t _imm;

   public:

   ItypeInstruction(TR::InstOpCode::Mnemonic op,
         TR::Node          *n,
         TR::Register      *treg,
         TR::Register      *sreg,
         uint32_t          imm,
         TR::CodeGenerator *codeGen)

      : ARM64Trg1Src1Instruction(op, n, treg, sreg, codeGen),
        _imm(imm)
      {
      }

   ItypeInstruction(TR::InstOpCode::Mnemonic op,
         TR::Node          *n,
         TR::Register      *treg,
         TR::Register      *sreg,
         uint32_t          imm,
         TR::Instruction   *precedingInstruction,
         TR::CodeGenerator *codeGen)
      : ARM64Trg1Src1Instruction(op, n, treg, sreg, precedingInstruction, codeGen),
        _imm(imm)
      {
      }

   uint32_t getSourceImmediate()            {return _imm;}
   uint32_t setSourceImmediate(uint32_t si) {return (_imm = si);}

   virtual uint8_t *generateBinaryEncoding();

   };


class LoadInstruction : public ARM64Trg1MemInstruction
   {
   public:

   LoadInstruction(TR::InstOpCode::Mnemonic op,
         TR::Node          *n,
         TR::Register      *treg,
         TR::MemoryReference *mr,
         TR::CodeGenerator *codeGen)
      : ARM64Trg1MemInstruction(op, n, treg, mr, codeGen)
      {
      }


   LoadInstruction(TR::InstOpCode::Mnemonic op,
         TR::Node          *n,
         TR::Register      *treg,
         TR::MemoryReference *mr,
         TR::Instruction   *precedingInstruction,
         TR::CodeGenerator *codeGen)
      : ARM64Trg1MemInstruction(op, n, treg, mr, precedingInstruction, codeGen)
      {
      }

   virtual uint8_t *generateBinaryEncoding();

   virtual int32_t estimateBinaryLength(int32_t currentEstimate);
   };


class StypeInstruction : public ARM64Src2Instruction
   {

   uint32_t _imm;

   public:

   StypeInstruction(TR::InstOpCode::Mnemonic op,
         TR::Node          *n,
         TR::Register      *s1reg,
         TR::Register      *s2reg,
         uint32_t          imm,
         TR::CodeGenerator *codeGen)

      : ARM64Src2Instruction(op, n, s1reg, s2reg, codeGen),
        _imm(imm)
      {
      }

   StypeInstruction(TR::InstOpCode::Mnemonic op,
         TR::Node          *n,
         TR::Register      *s1reg,
         TR::Register      *s2reg,
         uint32_t          imm,
         TR::Instruction   *precedingInstruction,
         TR::CodeGenerator *codeGen)
      : ARM64Src2Instruction(op, n, s1reg, s2reg, precedingInstruction, codeGen),
        _imm(imm)
      {
      }

   uint32_t getSourceImmediate()            {return _imm;}
   uint32_t setSourceImmediate(uint32_t si) {return (_imm = si);}

   virtual uint8_t *generateBinaryEncoding();

   };

class StoreInstruction : public ARM64MemSrc1Instruction
   {
   public:

   StoreInstruction(TR::InstOpCode::Mnemonic op,
         TR::Node          *n,
         TR::MemoryReference *mr,
         TR::Register      *sreg,
         TR::CodeGenerator *codeGen)
      : ARM64MemSrc1Instruction(op, n, mr, sreg, codeGen)
      {
      }

   StoreInstruction(TR::InstOpCode::Mnemonic op,
         TR::Node          *n,
         TR::MemoryReference *mr,
         TR::Register      *sreg,
         TR::Instruction   *precedingInstruction,
         TR::CodeGenerator *codeGen)
      : ARM64MemSrc1Instruction(op, n, mr, sreg, precedingInstruction, codeGen)
      {
      }

   virtual uint8_t *generateBinaryEncoding();

   virtual int32_t estimateBinaryLength(int32_t currentEstimate);
   };

class BtypeInstruction : public ARM64LabelInstruction
   {
   protected:

   TR::Register    *_source1Register;
   TR::Register    *_source2Register;

   public:
     BtypeInstruction(
       TR::InstOpCode::Mnemonic op,
       TR::Node                 *n,
       TR::LabelSymbol          *sym,
       TR::Register             *src1,
       TR::Register             *src2,
       TR::CodeGenerator        *cg
       )
     : ARM64LabelInstruction(op, n, sym, cg), _source1Register(src1), _source2Register(src2)
     {
     useRegister(_source1Register);
     useRegister(_source2Register);
     }

     BtypeInstruction(
       TR::InstOpCode::Mnemonic op,
       TR::Node                 *n,
       TR::LabelSymbol          *sym,
       TR::Register             *src1,
       TR::Register             *src2,
       TR::Instruction          *precedingInstruction,
       TR::CodeGenerator        *cg
       )
     : ARM64LabelInstruction(op, n, sym, precedingInstruction, cg), _source1Register(src1), _source2Register(src2)
     {
     useRegister(_source1Register);
     useRegister(_source2Register);
     }

   /**
    * @brief Gets source register
    * @return source register
    */
   TR::Register *getSource1Register() {return _source1Register;}
   /**
    * @brief Sets source register
    * @param[in] tr : source register
    * @return source register
    */
   TR::Register *setSource1Register(TR::Register *tr) {return (_source1Register = tr);}

   /**
    * @brief Gets source register 2
    * @return source register 2
    */
   TR::Register *getSource2Register() {return _source2Register;}
   /**
    * @brief Sets source register 2
    * @param[in] sr : source register 2
    * @return source register 2
    */
   TR::Register *setSource2Register(TR::Register *sr) {return (_source2Register = sr);}

   /**
    * @brief Gets i-th source register
    * @param[in] i : index
    * @return i-th source register or NULL
    */
   virtual TR::Register *getSourceRegister(uint32_t i) {if      (i==0) return getSource1Register();
                                                        else if (i==1) return _source2Register; return NULL;}

   /**
    * @brief Sets source register 2 in binary encoding
    * @param[in] instruction : instruction cursor
    */
   void insertSource2Register(uint32_t *instruction)
      {
      TR::RealRegister *source2 = toRealRegister(_source2Register);
      source2->setRegisterFieldRM(instruction);
      }

   /**
    * @brief Answers whether this instruction references the given virtual register
    * @param[in] reg : virtual register
    * @return true when the instruction references the virtual register
    */
   virtual bool refsRegister(TR::Register *reg);
   /**
    * @brief Answers whether this instruction uses the given virtual register
    * @param[in] reg : virtual register
    * @return true when the instruction uses the virtual register
    */
   virtual bool usesRegister(TR::Register *reg);
   /**
    * @brief Answers whether this instruction defines the given virtual register
    * @param[in] reg : virtual register
    * @return true when the instruction defines the virtual register
    */
   virtual bool defsRegister(TR::Register *reg);
   /**
    * @brief Answers whether this instruction defines the given real register
    * @param[in] reg : real register
    * @return true when the instruction defines the real register
    */
   virtual bool defsRealRegister(TR::Register *reg);
   /**
    * @brief Assigns registers
    * @param[in] kindToBeAssigned : register kind
    */
   virtual void assignRegisters(TR_RegisterKinds kindToBeAssigned);



   virtual uint8_t *generateBinaryEncoding();
   };

class UtypeInstruction : public ARM64Trg1Instruction
   {
   protected:

   uint32_t _imm;

   public:

   UtypeInstruction(TR::InstOpCode::Mnemonic op,
         TR::Node          *n,
         uint32_t          imm,
         TR::Register      *reg,
         TR::CodeGenerator *codeGen)
      : ARM64Trg1Instruction(op, n, reg, codeGen), _imm(imm)
      {
      }

   UtypeInstruction(TR::InstOpCode::Mnemonic op,
         TR::Node          *n,
         uint32_t          imm,
         TR::Register      *reg,
         TR::Instruction   *precedingInstruction,
         TR::CodeGenerator *codeGen)
      : ARM64Trg1Instruction(op, n, reg, precedingInstruction, codeGen), _imm(imm)
      {
      }

   virtual uint8_t *generateBinaryEncoding();
   };

class JtypeInstruction : public ARM64ImmSymInstruction
   {
   TR::Register *_target1Register;

   public:

   JtypeInstruction(TR::InstOpCode::Mnemonic op,
         TR::Node          *n,
         TR::Register      *trgReg,
         uintptr_t        imm,
         TR::RegisterDependencyConditions *cond,
         TR::SymbolReference *sr,
         TR::Snippet       *s,
         TR::CodeGenerator *codeGen)

      : ARM64ImmSymInstruction(op, n, imm, cond, sr, s, codeGen),
        _target1Register(trgReg)
      {
      }

   JtypeInstruction(TR::InstOpCode::Mnemonic op,
         TR::Node          *n,
         TR::Register      *trgReg,
         uintptr_t        imm,
         TR::RegisterDependencyConditions *cond,
         TR::SymbolReference *sr,
         TR::Snippet       *s,
         TR::Instruction   *precedingInstruction,
         TR::CodeGenerator *codeGen)
      : ARM64ImmSymInstruction(op, n, imm, cond, sr, s, precedingInstruction, codeGen),
        _target1Register(trgReg)
      {
      }

   TR::Register *getTargetRegister()               {return _target1Register;}
   TR::Register *setTargetRegister(TR::Register *r) {return (_target1Register = r);}
   virtual TR::Register *getTargetRegister(uint32_t i)       {if (i==0) return _target1Register; return NULL;}

   virtual uint8_t *generateBinaryEncoding();
   };

} // namespace TR

TR::Instruction *generateITYPE( TR::InstOpCode::Mnemonic op,
                                TR::Node          *n,
                                TR::Register      *treg,
                                TR::Register      *sreg,
                                uint32_t          imm,
                                TR::CodeGenerator *cg,
                                TR::Instruction   *previous = NULL);

TR::Instruction *generateLOAD(  TR::InstOpCode::Mnemonic op,
                                TR::Node          *n,
                                TR::Register      *trgReg,
                                TR::MemoryReference *memRef,
                                TR::CodeGenerator *cg,
                                TR::Instruction   *previous = NULL);

TR::Instruction *generateSTYPE( TR::InstOpCode::Mnemonic op,
                                TR::Node          *n,
                                TR::Register      *s1reg,
                                TR::Register      *s2reg,
                                uint32_t          imm,
                                TR::CodeGenerator *cg,
                                TR::Instruction   *previous = NULL);

TR::Instruction *generateSTORE( TR::InstOpCode::Mnemonic op,
                                TR::Node          *n,
                                TR::MemoryReference *memRef,
                                TR::Register      *srcReg,
                                TR::CodeGenerator *cg,
                                TR::Instruction   *previous = NULL);

TR::Instruction *generateBTYPE( TR::InstOpCode::Mnemonic op,
                                TR::Node          *n,
                                TR::LabelSymbol   *sym,
                                TR::Register      *src1,
                                TR::Register      *src2,
                                TR::CodeGenerator *cg,
                                TR::Instruction   *previous = NULL);

TR::Instruction *generateUTYPE( TR::InstOpCode::Mnemonic op,
                                TR::Node          *n,
                                uint32_t          imm,
                                TR::Register      *reg,
                                TR::CodeGenerator *cg,
                                TR::Instruction   *previous = NULL);

TR::Instruction *generateJTYPE( TR::InstOpCode::Mnemonic op,
                                TR::Node          *n,
                                TR::Register      *trgReg,
                                uintptr_t        imm,
                                TR::RegisterDependencyConditions *cond,
                                TR::SymbolReference *sr,
                                TR::Snippet       *s,
                                TR::CodeGenerator *cg,
                                TR::Instruction   *previous = NULL);

#endif // RVINSTRUCTION_INCL
