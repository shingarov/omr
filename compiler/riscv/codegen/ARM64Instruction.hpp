/*******************************************************************************
 * Copyright (c) 2018, 2019 IBM Corp. and others
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

#ifndef ARM64INSTRUCTION_INCL
#define ARM64INSTRUCTION_INCL

#include <stddef.h>
#include <stdint.h>

#include "codegen/CodeGenerator.hpp"
#include "codegen/Instruction.hpp"
#include "codegen/MemoryReference.hpp"
#include "il/symbol/LabelSymbol.hpp"
#include "infra/Assert.hpp"

namespace TR { class SymbolReference; }

#define ARM64_INSTRUCTION_LENGTH 4

/*
 * @brief Answers if the signed integer value can be placed in 9-bit field
 * @param[in] intValue : signed integer value
 * @return true if the value can be placed in 9-bit field, false otherwise
 */
inline bool constantIsImm9(int32_t intValue)
   {
   return (-256 <= intValue && intValue < 256);
   }

/*
 * @brief Answers if the unsigned integer value can be placed in 12-bit field
 * @param[in] intValue : unsigned integer value
 * @return true if the value can be placed in 12-bit field, false otherwise
 */
inline bool constantIsUnsignedImm12(uint64_t intValue)
   {
   return (intValue < 4096);
   }

/*
 * @brief Answers if the signed integer value can be placed in 28-bit field
 * @param[in] intValue : signed integer value
 * @return true if the value can be placed in 28-bit field, false otherwise
 */
inline bool constantIsSignedImm28(intptr_t intValue)
   {
   return (-0x8000000 <= intValue && intValue < 0x8000000);
   }

namespace TR
{

class ARM64ImmInstruction : public TR::Instruction
   {
   uint32_t _sourceImmediate;
   TR_ExternalRelocationTargetKind _reloKind;
   TR::SymbolReference *_symbolReference;

public:

   // Constructors without relocation types
   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] imm : immediate value
    * @param[in] cg : CodeGenerator
    */
   ARM64ImmInstruction(TR::InstOpCode::Mnemonic op, TR::Node *node, uint32_t imm, TR::CodeGenerator *cg)
      : TR::Instruction(op, node, cg), _sourceImmediate(imm), _reloKind(TR_NoRelocation), _symbolReference(NULL)
      {
      }
   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] imm : immediate value
    * @param[in] precedingInstruction : preceding instruction
    * @param[in] cg : CodeGenerator
    */
   ARM64ImmInstruction(TR::InstOpCode::Mnemonic op, TR::Node *node, uint32_t imm,
                        TR::Instruction *precedingInstruction, TR::CodeGenerator *cg)
      : TR::Instruction(op, node, precedingInstruction, cg), _sourceImmediate(imm), _reloKind(TR_NoRelocation), _symbolReference(NULL)
      {
      }

   // Constructors with relocation types
   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] imm : immediate value
    * @param[in] relocationKind : relocation kind
    * @param[in] cg : CodeGenerator
    */
   ARM64ImmInstruction(TR::InstOpCode::Mnemonic op, TR::Node *node, uint32_t imm, TR_ExternalRelocationTargetKind relocationKind,
                        TR::CodeGenerator *cg)
      : TR::Instruction(op, node, cg), _sourceImmediate(imm), _reloKind(relocationKind), _symbolReference(NULL)
      {
      setNeedsAOTRelocation(true);
      }
   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] imm : immediate value
    * @param[in] relocationKind : relocation kind
    * @param[in] precedingInstruction : preceding instruction
    * @param[in] cg : CodeGenerator
    */
   ARM64ImmInstruction(TR::InstOpCode::Mnemonic op, TR::Node *node, uint32_t imm, TR_ExternalRelocationTargetKind relocationKind,
                        TR::Instruction *precedingInstruction, TR::CodeGenerator *cg)
      : TR::Instruction(op, node, precedingInstruction, cg), _sourceImmediate(imm), _reloKind(relocationKind), _symbolReference(NULL)
      {
      setNeedsAOTRelocation(true);
      }

   // Constructors with relocation typesand associated symbol references
   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] imm : immediate value
    * @param[in] relocationKind : relocation kind
    * @param[in] sr : symbol reference
    * @param[in] cg : CodeGenerator
    */
   ARM64ImmInstruction(TR::InstOpCode::Mnemonic op, TR::Node *node, uint32_t imm, TR_ExternalRelocationTargetKind relocationKind,
                        TR::SymbolReference *sr, TR::CodeGenerator *cg)
      : TR::Instruction(op, node, cg), _sourceImmediate(imm), _reloKind(relocationKind), _symbolReference(sr)
      {
      setNeedsAOTRelocation(true);
      }
   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] imm : immediate value
    * @param[in] relocationKind : relocation kind
    * @param[in] sr : symbol reference
    * @param[in] precedingInstruction : preceding instruction
    * @param[in] cg : CodeGenerator
    */
   ARM64ImmInstruction(TR::InstOpCode::Mnemonic op, TR::Node *node, uint32_t imm, TR_ExternalRelocationTargetKind relocationKind,
                        TR::SymbolReference *sr, TR::Instruction *precedingInstruction, TR::CodeGenerator *cg)
      : TR::Instruction(op, node, precedingInstruction, cg), _sourceImmediate(imm), _reloKind(relocationKind), _symbolReference(sr)
      {
      setNeedsAOTRelocation(true);
      }

   /**
    * @brief Gets instruction kind
    * @return instruction kind
    */
   virtual Kind getKind() { return IsImm; }

   /**
    * @brief Gets source immediate
    * @return source immediate
    */
   uint32_t getSourceImmediate() {return _sourceImmediate;}
   /**
    * @brief Sets source immediate
    * @param[in] si : immediate value
    * @return source immediate
    */
   uint32_t setSourceImmediate(uint32_t si) {return (_sourceImmediate = si);}

   /**
    * @brief Gets relocation kind
    * @return relocation kind
    */
   TR_ExternalRelocationTargetKind getReloKind() { return _reloKind; }
   /**
    * @brief Sets relocation kind
    * @param[in] reloKind : relocation kind
    */
   void setReloKind(TR_ExternalRelocationTargetKind reloKind) { _reloKind = reloKind; }

   /**
    * @brief Gets symbol reference
    * @return symbol reference
    */
   TR::SymbolReference *getSymbolReference() {return _symbolReference;}
   /**
    * @brief Sets symbol reference
    * @param[in] sr : symbol reference
    * @return symbol reference
    */
   TR::SymbolReference *setSymbolReference(TR::SymbolReference *sr)
      {
      return (_symbolReference = sr);
      }

   /**
    * @brief Generates binary encoding of the instruction
    * @return instruction cursor
    */
   virtual uint8_t *generateBinaryEncoding();
   };

class ARM64ImmSymInstruction : public TR::Instruction
   {
   uintptr_t _addrImmediate;
   TR::SymbolReference *_symbolReference;
   TR::Snippet *_snippet;

   public:

   ARM64ImmSymInstruction(TR::InstOpCode::Mnemonic op,
                          TR::Node *node,
                          uintptr_t imm,
                          TR::RegisterDependencyConditions *cond,
                          TR::SymbolReference *sr,
                          TR::Snippet *s,
                          TR::CodeGenerator *cg)
      : TR::Instruction(op, node, cond, cg),
        _addrImmediate(imm), _symbolReference(sr), _snippet(s)
      {
      }

   ARM64ImmSymInstruction(TR::InstOpCode::Mnemonic op,
                          TR::Node *node,
                          uintptr_t imm,
                          TR::RegisterDependencyConditions *cond,
                          TR::SymbolReference *sr,
                          TR::Snippet *s,
                          TR::Instruction *precedingInstruction,
                          TR::CodeGenerator *cg)
      : TR::Instruction(op, node, cond, precedingInstruction, cg),
        _addrImmediate(imm), _symbolReference(sr), _snippet(s)
      {
      }

   /**
    * @brief Gets instruction kind
    * @return instruction kind
    */
   virtual Kind getKind() { return IsImmSym; }

   /**
    * @brief Gets address immediate
    * @return address immediate
    */
   uintptrj_t getAddrImmediate() { return _addrImmediate; }

   /**
    * @brief Gets symbol reference
    * @return symbol reference
    */
   TR::SymbolReference *getSymbolReference() { return _symbolReference; }
   /**
    * @brief Sets symbol reference
    * @param[in] sr : symbol reference
    * @return symbol reference
    */
   TR::SymbolReference *setSymbolReference(TR::SymbolReference *sr)
      {
      return (_symbolReference = sr);
      }

   /**
    * @brief Gets call snippet
    * @return call snippet
    */
   TR::Snippet *getCallSnippet() { return _snippet;}
   /**
    * @brief Sets call snippet
    * @param[in] s : call snippet
    * @return call snippet
    */
   TR::Snippet *setCallSnippet(TR::Snippet *s) { return (_snippet = s); }

   /**
    * @brief Sets immediate field in binary encoding
    * @param[in] instruction : instruction cursor
    * @param[in] distance : branch distance
    */
   void insertImmediateField(uint32_t *instruction, int32_t distance)
      {
      TR_ASSERT((distance & 0x3) == 0, "branch distance is not aligned");
      *instruction |= ((distance >> 2) & 0x3ffffff); // imm26
      }

   /**
    * @brief Generates binary encoding of the instruction
    * @return instruction cursor
    */
   virtual uint8_t *generateBinaryEncoding();
   };

class ARM64RegBranchInstruction : public TR::Instruction
   {
   TR::Register *_register;

   public:

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] treg : branch target
    * @param[in] cg : CodeGenerator
    */
   ARM64RegBranchInstruction(TR::InstOpCode::Mnemonic op, TR::Node *node, TR::Register *treg, TR::CodeGenerator *cg)
      : TR::Instruction(op, node, cg), _register(treg)
      {
      useRegister(treg);
      }

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] treg : branch target
    * @param[in] precedingInstruction : preceding instruction
    * @param[in] cg : CodeGenerator
    */
   ARM64RegBranchInstruction(TR::InstOpCode::Mnemonic op, TR::Node *node, TR::Register *treg,
                             TR::Instruction *precedingInstruction, TR::CodeGenerator *cg)
      : TR::Instruction(op, node, precedingInstruction, cg), _register(treg)
      {
      useRegister(treg);
      }

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] treg : branch target
    * @param[in] cond : register dependency condition
    * @param[in] cg : CodeGenerator
    */
   ARM64RegBranchInstruction(TR::InstOpCode::Mnemonic op, TR::Node *node, TR::Register *treg,
                             TR::RegisterDependencyConditions *cond, TR::CodeGenerator *cg)
      : TR::Instruction(op, node, cond, cg), _register(treg)
      {
      useRegister(treg);
      }

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] treg : branch target
    * @param[in] cond : register dependency condition
    * @param[in] precedingInstruction : preceding instruction
    * @param[in] cg : CodeGenerator
    */
   ARM64RegBranchInstruction(TR::InstOpCode::Mnemonic op, TR::Node *node, TR::Register *treg,
                             TR::RegisterDependencyConditions *cond,
                             TR::Instruction *precedingInstruction, TR::CodeGenerator *cg)
      : TR::Instruction(op, node, cond, precedingInstruction, cg), _register(treg)
      {
      useRegister(treg);
      }

   /**
    * @brief Gets instruction kind
    * @return instruction kind
    */
   virtual Kind getKind() { return IsRegBranch; }

   /**
    * @brief Gets target register
    * @return target register
    */
   TR::Register *getTargetRegister() {return _register;}
   /**
    * @brief Sets target register
    * @param[in] tr : target register
    * @return target register
    */
   TR::Register *setTargetRegister(TR::Register *tr) {return (_register = tr);}

   /**
    * @brief Sets target register in binary encoding
    * @param[in] instruction : instruction cursor
    */
   void insertTargetRegister(uint32_t *instruction)
      {
      TR::RealRegister *target = toRealRegister(_register);
      target->setRegisterFieldRN(instruction);
      }

   /**
    * @brief Generates binary encoding of the instruction
    * @return instruction cursor
    */
   virtual uint8_t *generateBinaryEncoding();
   };

class ARM64Trg1Instruction : public TR::Instruction
   {
   TR::Register *_target1Register;

   public:

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] treg : target register
    * @param[in] cg : CodeGenerator
    */
   ARM64Trg1Instruction(TR::InstOpCode::Mnemonic op, TR::Node *node, TR::Register *treg, TR::CodeGenerator *cg)
      : TR::Instruction(op, node, cg), _target1Register(treg)
      {
      useRegister(treg);
      }

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] treg : target register
    * @param[in] precedingInstruction : preceding instruction
    * @param[in] cg : CodeGenerator
    */
   ARM64Trg1Instruction(TR::InstOpCode::Mnemonic op, TR::Node *node, TR::Register *treg,
                        TR::Instruction *precedingInstruction, TR::CodeGenerator *cg)
      : TR::Instruction(op, node, precedingInstruction, cg), _target1Register(treg)
      {
      useRegister(treg);
      }

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] treg : target register
    * @param[in] cond : register dependency conditions
    * @param[in] cg : CodeGenerator
    */
   ARM64Trg1Instruction(TR::InstOpCode::Mnemonic op, TR::Node *node, TR::Register *treg,
                        TR::RegisterDependencyConditions *cond, TR::CodeGenerator *cg)
      : TR::Instruction(op, node, cond, cg), _target1Register(treg)
      {
      useRegister(treg);
      }

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] treg : target register
    * @param[in] cond : register dependency conditions
    * @param[in] precedingInstruction : preceding instruction
    * @param[in] cg : CodeGenerator
    */
   ARM64Trg1Instruction(TR::InstOpCode::Mnemonic op, TR::Node *node, TR::Register *treg,
                        TR::RegisterDependencyConditions *cond, TR::Instruction *precedingInstruction,
                        TR::CodeGenerator *cg)
      : TR::Instruction(op, node, cond, precedingInstruction, cg), _target1Register(treg)
      {
      useRegister(treg);
      }

   /**
    * @brief Gets instruction kind
    * @return instruction kind
    */
   virtual Kind getKind() { return IsTrg1; }

   /**
    * @brief Gets target register
    * @return target register
    */
   TR::Register *getTargetRegister() {return _target1Register;}
   /**
    * @brief Sets target register
    * @param[in] tr : target register
    * @return target register
    */
   TR::Register *setTargetRegister(TR::Register *tr) {return (_target1Register = tr);}

   /**
    * @brief Sets target register in binary encoding
    * @param[in] instruction : instruction cursor
    */
   void insertTargetRegister(uint32_t *instruction)
      {
      TR::RealRegister *target = toRealRegister(_target1Register);
      target->setRegisterFieldRD(instruction);
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

   /**
    * @brief Generates binary encoding of the instruction
    * @return instruction cursor
    */
   virtual uint8_t *generateBinaryEncoding();
   };

class ARM64Trg1ImmInstruction : public ARM64Trg1Instruction
   {
   uint32_t _sourceImmediate;

   public:

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] treg : target register
    * @param[in] imm : immediate value
    * @param[in] cg : CodeGenerator
    */
   ARM64Trg1ImmInstruction(TR::InstOpCode::Mnemonic op, TR::Node *node, TR::Register *treg,
                            uint32_t imm, TR::CodeGenerator *cg)
      : ARM64Trg1Instruction(op, node, treg, cg), _sourceImmediate(imm)
      {
      }

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] treg : target register
    * @param[in] imm : immediate value
    * @param[in] precedingInstruction : preceding instruction
    * @param[in] cg : CodeGenerator
    */
   ARM64Trg1ImmInstruction(TR::InstOpCode::Mnemonic op, TR::Node *node, TR::Register *treg,
                            uint32_t imm,
                            TR::Instruction *precedingInstruction, TR::CodeGenerator *cg)
      : ARM64Trg1Instruction(op, node, treg, precedingInstruction, cg), _sourceImmediate(imm)
      {
      }

   /**
    * @brief Gets instruction kind
    * @return instruction kind
    */
   virtual Kind getKind() { return IsTrg1Imm; }

   /**
    * @brief Gets source immediate
    * @return source immediate
    */
   uint32_t getSourceImmediate() {return _sourceImmediate;}
   /**
    * @brief Sets source immediate
    * @param[in] si : immediate value
    * @return source immediate
    */
   uint32_t setSourceImmediate(uint32_t si) {return (_sourceImmediate = si);}

   /**
    * @brief Sets immediate field in binary encoding
    * @param[in] instruction : instruction cursor
    */
   void insertImmediateField(uint32_t *instruction)
      {
      TR::InstOpCode::Mnemonic op = getOpCodeValue();
#ifdef TR_RISCV_ARM64_SOURCE_COMPAT
      if (op == TR::InstOpCode::movzx || op == TR::InstOpCode::movzw ||
          op == TR::InstOpCode::movnx || op == TR::InstOpCode::movnw ||
          op == TR::InstOpCode::movkx || op == TR::InstOpCode::movkw)
         {
         *instruction |= ((_sourceImmediate & 0x3ffff) << 5);
         }
      else if (op == TR::InstOpCode::adr || op == TR::InstOpCode::adrp)
         {
         *instruction |= ((_sourceImmediate & 0x7ffff) << 5) | ((_sourceImmediate & 0x3) << 29);
         }
      else
#endif
         {
         TR_ASSERT(false, "Unsupported opcode in ARM64Trg1ImmInstruction.");
         }
      }

   /**
    * @brief Generates binary encoding of the instruction
    * @return instruction cursor
    */
   virtual uint8_t *generateBinaryEncoding();
   };

class ARM64Trg1Src1Instruction : public ARM64Trg1Instruction
   {
   TR::Register *_source1Register;

   public:

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] treg : target register
    * @param[in] sreg : source register
    * @param[in] cg : CodeGenerator
    */
   ARM64Trg1Src1Instruction(TR::InstOpCode::Mnemonic op, TR::Node *node, TR::Register *treg,
                             TR::Register *sreg, TR::CodeGenerator *cg)
      : ARM64Trg1Instruction(op, node, treg, cg), _source1Register(sreg)
      {
      useRegister(sreg);
      }

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] treg : target register
    * @param[in] sreg : source register
    * @param[in] precedingInstruction : preceding instruction
    * @param[in] cg : CodeGenerator
    */
   ARM64Trg1Src1Instruction(TR::InstOpCode::Mnemonic op, TR::Node *node, TR::Register *treg,
                             TR::Register *sreg,
                             TR::Instruction *precedingInstruction, TR::CodeGenerator *cg)
      : ARM64Trg1Instruction(op, node, treg, precedingInstruction, cg), _source1Register(sreg)
      {
      useRegister(sreg);
      }

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] treg : target register
    * @param[in] sreg : source register
    * @param[in] cond : register dependency conditions
    * @param[in] cg : CodeGenerator
    */
   ARM64Trg1Src1Instruction(TR::InstOpCode::Mnemonic op, TR::Node *node, TR::Register *treg,
                             TR::Register *sreg, TR::RegisterDependencyConditions *cond, TR::CodeGenerator *cg)
      : ARM64Trg1Instruction(op, node, treg, cond, cg), _source1Register(sreg)
      {
      useRegister(sreg);
      }

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] treg : target register
    * @param[in] sreg : source register
    * @param[in] cond : register dependency conditions
    * @param[in] precedingInstruction : preceding instruction
    * @param[in] cg : CodeGenerator
    */
   ARM64Trg1Src1Instruction(TR::InstOpCode::Mnemonic op, TR::Node *node, TR::Register *treg,
                             TR::Register *sreg, TR::RegisterDependencyConditions *cond,
                             TR::Instruction *precedingInstruction, TR::CodeGenerator *cg)
      : ARM64Trg1Instruction(op, node, treg, cond, precedingInstruction, cg), _source1Register(sreg)
      {
      useRegister(sreg);
      }

   /**
    * @brief Gets instruction kind
    * @return instruction kind
    */
   virtual Kind getKind() { return IsTrg1Src1; }

   /**
    * @brief Gets source register
    * @return source register
    */
   TR::Register *getSource1Register() {return _source1Register;}
   /**
    * @brief Sets source register
    * @param[in] sr : source register
    * @return source register
    */
   TR::Register *setSource1Register(TR::Register *sr) {return (_source1Register = sr);}

   /**
    * @brief Gets i-th source register
    * @param[in] i : index
    * @return i-th source register or NULL
    */
   virtual TR::Register *getSourceRegister(uint32_t i) {if (i==0) return _source1Register; return NULL;}

   /**
    * @brief Sets source register in binary encoding
    * @param[in] instruction : instruction cursor
    */
   void insertSource1Register(uint32_t *instruction)
      {
      TR::RealRegister *source1 = toRealRegister(_source1Register);
      source1->setRegisterFieldRN(instruction);
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

   /**
    * @brief Generates binary encoding of the instruction
    * @return instruction cursor
    */
   virtual uint8_t *generateBinaryEncoding();
   };

class ARM64Trg1Src1ImmInstruction : public ARM64Trg1Src1Instruction
   {
   uint32_t _source1Immediate;

   public:

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] treg : target register
    * @param[in] sreg : source register
    * @param[in] imm : immediate value
    * @param[in] cg : CodeGenerator
    */
   ARM64Trg1Src1ImmInstruction(TR::InstOpCode::Mnemonic op, TR::Node *node, TR::Register *treg,
                                TR::Register *sreg, uint32_t imm, TR::CodeGenerator *cg)
      : ARM64Trg1Src1Instruction(op, node, treg, sreg, cg), _source1Immediate(imm)
      {
      }

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] treg : target register
    * @param[in] sreg : source register
    * @param[in] imm : immediate value
    * @param[in] precedingInstruction : preceding instruction
    * @param[in] cg : CodeGenerator
    */
   ARM64Trg1Src1ImmInstruction(TR::InstOpCode::Mnemonic op, TR::Node *node, TR::Register *treg,
                                TR::Register *sreg, uint32_t imm,
                                TR::Instruction *precedingInstruction, TR::CodeGenerator *cg)
      : ARM64Trg1Src1Instruction(op, node, treg, sreg, precedingInstruction, cg), _source1Immediate(imm)
      {
      }

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] treg : target register
    * @param[in] sreg : source register
    * @param[in] imm : immediate value
    * @param[in] cond : register dependency conditions
    * @param[in] cg : CodeGenerator
    */
   ARM64Trg1Src1ImmInstruction(TR::InstOpCode::Mnemonic op, TR::Node *node, TR::Register *treg,
                               TR::Register *sreg, uint32_t imm,
                               TR::RegisterDependencyConditions *cond, TR::CodeGenerator *cg)
      : ARM64Trg1Src1Instruction(op, node, treg, sreg, cond, cg), _source1Immediate(imm)
      {
      }

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] treg : target register
    * @param[in] sreg : source register
    * @param[in] imm : immediate value
    * @param[in] cond : register dependency conditions
    * @param[in] precedingInstruction : preceding instruction
    * @param[in] cg : CodeGenerator
    */
   ARM64Trg1Src1ImmInstruction(TR::InstOpCode::Mnemonic op, TR::Node *node, TR::Register *treg,
                               TR::Register *sreg, uint32_t imm, TR::RegisterDependencyConditions *cond,
                               TR::Instruction *precedingInstruction, TR::CodeGenerator *cg)
      : ARM64Trg1Src1Instruction(op, node, treg, sreg, cond, precedingInstruction, cg), _source1Immediate(imm)
      {
      }

   /**
    * @brief Gets instruction kind
    * @return instruction kind
    */
   virtual Kind getKind() { return IsTrg1Src1Imm; }

   /**
    * @brief Gets source immediate
    * @return source immediate
    */
   uint32_t getSourceImmediate() {return _source1Immediate;}
   /**
    * @brief Sets source immediate
    * @param[in] si : immediate value
    * @return source immediate
    */
   uint32_t setSourceImmediate(uint32_t si) {return (_source1Immediate = si);}

   /**
    * @brief Sets immediate field in binary encoding
    * @param[in] instruction : instruction cursor
    */
   void insertImmediateField(uint32_t *instruction)
      {
      *instruction |= ((_source1Immediate & 0xfff) << 10); /* imm12 */
      }

   /**
    * @brief Generates binary encoding of the instruction
    * @return instruction cursor
    */
   virtual uint8_t *generateBinaryEncoding();
   };

class ARM64Trg1Src2Instruction : public ARM64Trg1Src1Instruction
   {
   TR::Register *_source2Register;

   public:

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] treg : target register
    * @param[in] s1reg : source register 1
    * @param[in] s2reg : source register 2
    * @param[in] cg : CodeGenerator
    */
   ARM64Trg1Src2Instruction( TR::InstOpCode::Mnemonic op,
                             TR::Node *node,
                             TR::Register *treg,
                             TR::Register *s1reg,
                             TR::Register *s2reg, TR::CodeGenerator *cg)
      : ARM64Trg1Src1Instruction(op, node, treg, s1reg, cg), _source2Register(s2reg)
      {
      useRegister(s2reg);
      }

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] treg : target register
    * @param[in] s1reg : source register 1
    * @param[in] s2reg : source register 2
    * @param[in] precedingInstruction : preceding instruction
    * @param[in] cg : CodeGenerator
    */
   ARM64Trg1Src2Instruction( TR::InstOpCode::Mnemonic op,
                             TR::Node *node,
                             TR::Register *treg,
                             TR::Register *s1reg,
                             TR::Register *s2reg,
                             TR::Instruction *precedingInstruction, TR::CodeGenerator *cg)
      : ARM64Trg1Src1Instruction(op, node, treg, s1reg, precedingInstruction, cg),
                             _source2Register(s2reg)
      {
      useRegister(s2reg);
      }

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] treg : target register
    * @param[in] s1reg : source register 1
    * @param[in] s2reg : source register 2
    * @param[in] cond : register dependency conditions
    * @param[in] cg : CodeGenerator
    */
   ARM64Trg1Src2Instruction( TR::InstOpCode::Mnemonic op,
                             TR::Node *node,
                             TR::Register *treg,
                             TR::Register *s1reg,
                             TR::Register *s2reg,
                             TR::RegisterDependencyConditions *cond,
                             TR::CodeGenerator *cg)
      : ARM64Trg1Src1Instruction(op, node, treg, s1reg, cond, cg), _source2Register(s2reg)
      {
      useRegister(s2reg);
      }

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] treg : target register
    * @param[in] s1reg : source register 1
    * @param[in] s2reg : source register 2
    * @param[in] cond : register dependency conditions
    * @param[in] precedingInstruction : preceding instruction
    * @param[in] cg : CodeGenerator
    */
   ARM64Trg1Src2Instruction( TR::InstOpCode::Mnemonic op,
                             TR::Node *node,
                             TR::Register *treg,
                             TR::Register *s1reg,
                             TR::Register *s2reg,
                             TR::RegisterDependencyConditions *cond,
                             TR::Instruction *precedingInstruction, TR::CodeGenerator *cg)
      : ARM64Trg1Src1Instruction(op, node, treg, s1reg, cond, precedingInstruction, cg),
                             _source2Register(s2reg)
      {
      useRegister(s2reg);
      }

   /**
    * @brief Gets instruction kind
    * @return instruction kind
    */
   virtual Kind getKind() { return IsTrg1Src2; }

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

   /**
    * @brief Generates binary encoding of the instruction
    * @return instruction cursor
    */
   virtual uint8_t *generateBinaryEncoding();
   };



class ARM64Trg1Src3Instruction : public ARM64Trg1Src2Instruction
   {
   TR::Register *_source3Register;

   public:

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] treg : target register
    * @param[in] s1reg : source register 1
    * @param[in] s2reg : source register 2
    * @param[in] s3reg : source register 3
    * @param[in] cg : CodeGenerator
    */
   ARM64Trg1Src3Instruction( TR::InstOpCode::Mnemonic op,
                             TR::Node *node,
                             TR::Register *treg,
                             TR::Register *s1reg,
                             TR::Register *s2reg,
                             TR::Register *s3reg, TR::CodeGenerator *cg)
      : ARM64Trg1Src2Instruction(op, node, treg, s1reg, s2reg, cg), _source3Register(s3reg)
      {
      useRegister(s3reg);
      }

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] treg : target register
    * @param[in] s1reg : source register 1
    * @param[in] s2reg : source register 2
    * @param[in] s3reg : source register 3
    * @param[in] precedingInstruction : preceding instruction
    * @param[in] cg : CodeGenerator
    */
   ARM64Trg1Src3Instruction( TR::InstOpCode::Mnemonic op,
                             TR::Node *node,
                             TR::Register *treg,
                             TR::Register *s1reg,
                             TR::Register *s2reg,
                             TR::Register *s3reg,
                             TR::Instruction *precedingInstruction, TR::CodeGenerator *cg)
      : ARM64Trg1Src2Instruction(op, node, treg, s1reg, s2reg, precedingInstruction, cg),
                             _source3Register(s3reg)
      {
      useRegister(s3reg);
      }

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] treg : target register
    * @param[in] s1reg : source register 1
    * @param[in] s2reg : source register 2
    * @param[in] s3reg : source register 3
    * @param[in] cond : register dependency conditions
    * @param[in] cg : CodeGenerator
    */
   ARM64Trg1Src3Instruction( TR::InstOpCode::Mnemonic op,
                             TR::Node *node,
                             TR::Register *treg,
                             TR::Register *s1reg,
                             TR::Register *s2reg,
                             TR::Register *s3reg,
                             TR::RegisterDependencyConditions *cond,
                             TR::CodeGenerator *cg)
      : ARM64Trg1Src2Instruction(op, node, treg, s1reg, s2reg, cond, cg), _source3Register(s3reg)
      {
      useRegister(s3reg);
      }

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] treg : target register
    * @param[in] s1reg : source register 1
    * @param[in] s2reg : source register 2
    * @param[in] s3reg : source register 3
    * @param[in] cond : register dependency conditions
    * @param[in] precedingInstruction : preceding instruction
    * @param[in] cg : CodeGenerator
    */
   ARM64Trg1Src3Instruction( TR::InstOpCode::Mnemonic op,
                             TR::Node *node,
                             TR::Register *treg,
                             TR::Register *s1reg,
                             TR::Register *s2reg,
                             TR::Register *s3reg,
                             TR::RegisterDependencyConditions *cond,
                             TR::Instruction *precedingInstruction, TR::CodeGenerator *cg)
      : ARM64Trg1Src2Instruction(op, node, treg, s1reg, s2reg, cond, precedingInstruction, cg),
                             _source3Register(s3reg)
      {
      useRegister(s3reg);
      }

   /**
    * @brief Gets instruction kind
    * @return instruction kind
    */
   virtual Kind getKind() { return IsTrg1Src3; }

   /**
    * @brief Gets source register 3
    * @return source register 3
    */
   TR::Register *getSource3Register() {return _source3Register;}
   /**
    * @brief Sets source register 3
    * @param[in] sr : source register 3
    * @return source register 3
    */
   TR::Register *setSource3Register(TR::Register *sr) {return (_source3Register = sr);}

   /**
    * @brief Gets i-th source register
    * @param[in] i : index
    * @return i-th source register or NULL
    */
   virtual TR::Register *getSourceRegister(uint32_t i) {if      (i==0) return getSource1Register();
                                               else if (i==1) return getSource2Register();
                                               else if (i==2) return _source3Register; return NULL;}

   /**
    * @brief Sets source register 3 in binary encoding
    * @param[in] instruction : instruction cursor
    */
   void insertSource3Register(uint32_t *instruction)
      {
      TR::RealRegister *source3 = toRealRegister(_source3Register);
      source3->setRegisterFieldRA(instruction);
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

   /**
    * @brief Generates binary encoding of the instruction
    * @return instruction cursor
    */
   virtual uint8_t *generateBinaryEncoding();
   };

class ARM64Trg1MemInstruction : public ARM64Trg1Instruction
   {
   TR::MemoryReference *_memoryReference;

   public:

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] treg : target register
    * @param[in] mr : memory reference
    * @param[in] cg : CodeGenerator
    */
   ARM64Trg1MemInstruction(TR::InstOpCode::Mnemonic op,
                            TR::Node *node,
                            TR::Register *treg,
                            TR::MemoryReference *mr, TR::CodeGenerator *cg)
      : ARM64Trg1Instruction(op, node, treg, cg), _memoryReference(mr)
      {
      }

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] treg : target register
    * @param[in] mr : memory reference
    * @param[in] precedingInstruction : preceding instruction
    * @param[in] cg : CodeGenerator
    */
   ARM64Trg1MemInstruction(TR::InstOpCode::Mnemonic op,
                            TR::Node *node,
                            TR::Register *treg,
                            TR::MemoryReference *mr,
                            TR::Instruction *precedingInstruction, TR::CodeGenerator *cg)
      : ARM64Trg1Instruction(op, node, treg, precedingInstruction, cg), _memoryReference(mr)
      {
      mr->incRegisterTotalUseCounts(cg);
      }

   /**
    * @brief Gets instruction kind
    * @return instruction kind
    */
   virtual Kind getKind() { return IsTrg1Mem; }

   /**
    * @brief Gets memory reference
    * @return memory reference
    */
   TR::MemoryReference *getMemoryReference() {return _memoryReference;}
   /**
    * @brief Sets memory reference
    * @param[in] mr : memory reference
    * @return memory reference
    */
   TR::MemoryReference *setMemoryReference(TR::MemoryReference *mr)
      {
      return (_memoryReference = mr);
      }

   /**
    * @brief Gets base register of memory reference
    * @return base register
    */
   virtual TR::Register *getMemoryBase() {return getMemoryReference()->getBaseRegister();}
   /**
    * @brief Gets index register of memory reference
    * @return index register
    */
   virtual TR::Register *getMemoryIndex() {return NULL; }
   /**
    * @brief Gets offset of memory reference
    * @return offset
    */
   virtual int32_t getOffset() {return getMemoryReference()->getOffset();}

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

   /**
    * @brief Generates binary encoding of the instruction
    * @return instruction cursor
    */
   virtual uint8_t *generateBinaryEncoding();

   /**
    * @brief Estimates binary length
    * @param[in] currentEstimate : current estimated length
    * @return estimated binary length
    */
   virtual int32_t estimateBinaryLength(int32_t currentEstimate);
   };

class ARM64MemInstruction : public TR::Instruction
   {
   TR::MemoryReference *_memoryReference;

   public:

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] mr : memory reference
    * @param[in] cg : CodeGenerator
    */
   ARM64MemInstruction(TR::InstOpCode::Mnemonic op,
                        TR::Node *node,
                        TR::MemoryReference *mr, TR::CodeGenerator *cg)
      : TR::Instruction(op, node, cg), _memoryReference(mr)
      {
      }

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] mr : memory reference
    * @param[in] precedingInstruction : preceding instruction
    * @param[in] cg : CodeGenerator
    */
   ARM64MemInstruction(TR::InstOpCode::Mnemonic op,
                        TR::Node *node,
                        TR::MemoryReference *mr,
                        TR::Instruction *precedingInstruction, TR::CodeGenerator *cg)
      : TR::Instruction(op, node, precedingInstruction, cg), _memoryReference(mr)
      {
      }

   /**
    * @brief Gets instruction kind
    * @return instruction kind
    */
   virtual Kind getKind() { return IsMem; }

   /**
    * @brief Gets memory reference
    * @return memory reference
    */
   TR::MemoryReference *getMemoryReference() {return _memoryReference;}
   /**
    * @brief Sets memory reference
    * @param[in] mr : memory reference
    * @return memory reference
    */
   TR::MemoryReference *setMemoryReference(TR::MemoryReference *mr)
      {
      return (_memoryReference = mr);
      }

   /**
    * @brief Gets base register of memory reference
    * @return base register
    */
   virtual TR::Register *getMemoryBase() {return getMemoryReference()->getBaseRegister();}
   /**
    * @brief Gets index register of memory reference
    * @return index register
    */
   virtual TR::Register *getMemoryIndex() {return NULL;}
   /**
    * @brief Gets offset of memory reference
    * @return offset
    */
   virtual int32_t getOffset() {return getMemoryReference()->getOffset();}

   /**
    * @brief Generates binary encoding of the instruction
    * @return instruction cursor
    */
   virtual uint8_t *generateBinaryEncoding();

   /**
    * @brief Estimates binary length
    * @param[in] currentEstimate : current estimated length
    * @return estimated binary length
    */
   virtual int32_t estimateBinaryLength(int32_t currentEstimate);
   };

class ARM64MemSrc1Instruction : public ARM64MemInstruction
   {
   TR::Register *_source1Register;

   public:

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] mr : memory reference
    * @param[in] sreg : source register
    * @param[in] cg : CodeGenerator
    */
   ARM64MemSrc1Instruction(TR::InstOpCode::Mnemonic op,
                            TR::Node *node,
                            TR::MemoryReference *mr,
                            TR::Register *sreg, TR::CodeGenerator *cg)
      : ARM64MemInstruction(op, node, mr, cg), _source1Register(sreg)
      {
      useRegister(sreg);
      mr->incRegisterTotalUseCounts(cg);
      }

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] mr : memory reference
    * @param[in] sreg : source register
    * @param[in] precedingInstruction : preceding instruction
    * @param[in] cg : CodeGenerator
    */
   ARM64MemSrc1Instruction(TR::InstOpCode::Mnemonic op,
                            TR::Node *node,
                            TR::MemoryReference *mr,
                            TR::Register *sreg,
                            TR::Instruction *precedingInstruction, TR::CodeGenerator *cg)
      : ARM64MemInstruction(op, node, mr, precedingInstruction, cg), _source1Register(sreg)
      {
      }

   /**
    * @brief Gets instruction kind
    * @return instruction kind
    */
   virtual Kind getKind() { return IsMemSrc1; }

   /**
    * @brief Gets source register
    * @return source register
    */
   TR::Register *getSource1Register() {return _source1Register;}
   /**
    * @brief Sets source register
    * @param[in] sr : source register
    * @return source register
    */
   TR::Register *setSource1Register(TR::Register *sr) {return (_source1Register = sr);}

   /**
    * @brief Sets source register in binary encoding
    * @param[in] instruction : instruction cursor
    */
   void insertSource1Register(uint32_t *instruction)
      {
      TR::RealRegister *source1 = toRealRegister(_source1Register);
      source1->setRegisterFieldRT(instruction);
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

   /**
    * @brief Generates binary encoding of the instruction
    * @return instruction cursor
    */
   virtual uint8_t *generateBinaryEncoding();

   /**
    * @brief Estimates binary length
    * @param[in] currentEstimate : current estimated length
    * @return estimated binary length
    */
   virtual int32_t estimateBinaryLength(int32_t currentEstimate);
   };

class ARM64Src1Instruction : public TR::Instruction
   {
   TR::Register *_source1Register;

   public:

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] sreg : source register
    * @param[in] cg : CodeGenerator
    */
   ARM64Src1Instruction(TR::InstOpCode::Mnemonic op, TR::Node *node, TR::Register *sreg, TR::CodeGenerator *cg)
      : TR::Instruction(op, node, cg), _source1Register(sreg)
      {
      useRegister(sreg);
      }

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] sreg : source register
    * @param[in] precedingInstruction : preceding instruction
    * @param[in] cg : CodeGenerator
    */
   ARM64Src1Instruction(TR::InstOpCode::Mnemonic op, TR::Node *node, TR::Register *sreg,
                        TR::Instruction *precedingInstruction, TR::CodeGenerator *cg)
      : TR::Instruction(op, node, precedingInstruction, cg), _source1Register(sreg)
      {
      useRegister(sreg);
      }

   /**
    * @brief Gets instruction kind
    * @return instruction kind
    */
   virtual Kind getKind() { return IsSrc1; }

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
    * @brief Gets i-th source register
    * @param[in] i : index
    * @return i-th source register or NULL
    */
   virtual TR::Register *getSourceRegister(uint32_t i) {if (i==0) return _source1Register; return NULL;}

   /**
    * @brief Sets source register in binary encoding
    * @param[in] instruction : instruction cursor
    */
   void insertSource1Register(uint32_t *instruction)
      {
      TR::RealRegister *source = toRealRegister(_source1Register);
      source->setRegisterFieldRN(instruction);
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

   /**
    * @brief Generates binary encoding of the instruction
    * @return instruction cursor
    */
   virtual uint8_t *generateBinaryEncoding();
   };

class ARM64Src2Instruction : public ARM64Src1Instruction
   {
   TR::Register *_source2Register;

   public:

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] s1reg : source register 1
    * @param[in] s2reg : source register 2
    * @param[in] cg : CodeGenerator
    */
   ARM64Src2Instruction( TR::InstOpCode::Mnemonic op,
                         TR::Node *node,
                         TR::Register *s1reg,
                         TR::Register *s2reg, TR::CodeGenerator *cg)
      : ARM64Src1Instruction(op, node, s1reg, cg), _source2Register(s2reg)
      {
      useRegister(s2reg);
      }

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] s1reg : source register 1
    * @param[in] s2reg : source register 2
    * @param[in] precedingInstruction : preceding instruction
    * @param[in] cg : CodeGenerator
    */
   ARM64Src2Instruction( TR::InstOpCode::Mnemonic op,
                         TR::Node *node,
                         TR::Register *s1reg,
                         TR::Register *s2reg,
                         TR::Instruction *precedingInstruction, TR::CodeGenerator *cg)
      : ARM64Src1Instruction(op, node, s1reg, precedingInstruction, cg),
        _source2Register(s2reg)
      {
      useRegister(s2reg);
      }

   /**
    * @brief Gets instruction kind
    * @return instruction kind
    */
   virtual Kind getKind() { return IsSrc2; }

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

   /**
    * @brief Generates binary encoding of the instruction
    * @return instruction cursor
    */
   virtual uint8_t *generateBinaryEncoding();
   };

} // TR

#endif
