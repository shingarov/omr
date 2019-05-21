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

#include <riscv.h>
#include "codegen/RVInstruction.hpp"
#include "codegen/CodeGenerator.hpp"
#include "codegen/GenerateInstructions.hpp"
#include "codegen/Linkage.hpp"
#include "codegen/RegisterDependency.hpp"
#include "codegen/TreeEvaluator.hpp"
#include "il/Node.hpp"
#include "il/Node_inlines.hpp"
#include "infra/Bit.hpp"

static TR::Register *addOrSubInteger(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Node *firstChild = node->getFirstChild();
   TR::Register *src1Reg = cg->evaluate(firstChild);
   TR::Node *secondChild = node->getSecondChild();
   TR::Register *trgReg = cg->allocateRegister();
   bool isAdd = node->getOpCode().isAdd();

   if (secondChild->getOpCode().isLoadConst() && secondChild->getRegister() == NULL)
      {
      int32_t value = secondChild->getInt();
      if (VALID_ITYPE_IMM(value))
         {
         generateITYPE(TR::InstOpCode::_addi, node, trgReg, src1Reg, isAdd ? value : - value, cg);
         }
      else
         {
         TR::Register *tmpReg = cg->allocateRegister();
         loadConstant32(cg, node, value, tmpReg);
         generateRTYPE(isAdd ? TR::InstOpCode::_add : TR::InstOpCode::_sub, node, trgReg, src1Reg, tmpReg, cg);
         cg->stopUsingRegister(tmpReg);
         }
      }
   else
      {
      TR::Register *src2Reg = cg->evaluate(secondChild);
      generateRTYPE(isAdd ? TR::InstOpCode::_add : TR::InstOpCode::_sub, node, trgReg, src1Reg, src2Reg, cg);
      }

   node->setRegister(trgReg);
   firstChild->decReferenceCount();
   secondChild->decReferenceCount();
   return trgReg;
   }

TR::Register *
OMR::ARM64::TreeEvaluator::iaddEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return addOrSubInteger(node, cg);
   }

TR::Register *
OMR::ARM64::TreeEvaluator::laddEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::laddEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::isubEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return addOrSubInteger(node, cg);
   }

TR::Register *
OMR::ARM64::TreeEvaluator::lsubEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::lsubEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

// Multiply a register by a 32-bit constant
static void mulConstant32(TR::Node *node, TR::Register *treg, TR::Register *sreg, int32_t value, TR::CodeGenerator *cg)
   {
   if (value == 0)
      {
      loadConstant32(cg, node, 0, treg);
      }
   else if (value == 1)
      {
      generateITYPE(TR::InstOpCode::_addi, node, treg, sreg, 0, cg);
      }
   else if (value == -1)
      {
      TR::Register *zero = cg->machine()->getRealRegister(TR::RealRegister::zero);
      generateRTYPE(TR::InstOpCode::_subw, node, treg, zero, sreg, cg);
      }
   else
      {
      TR::Register *tmpReg = cg->allocateRegister();
      loadConstant32(cg, node, value, tmpReg);
      generateRTYPE(TR::InstOpCode::_mulw, node, treg, sreg, tmpReg, cg);
      cg->stopUsingRegister(tmpReg);
      }
   }

TR::Register *
OMR::ARM64::TreeEvaluator::imulEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Node *firstChild = node->getFirstChild();
   TR::Register *src1Reg = cg->evaluate(firstChild);
   TR::Node *secondChild = node->getSecondChild();
   TR::Register *trgReg;

   if (secondChild->getOpCode().isLoadConst() && secondChild->getRegister() == NULL)
      {
      int32_t value = secondChild->getInt();
      if (value > 0 && cg->convertMultiplyToShift(node))
         {
         // The multiply has been converted to a shift.
         trgReg = cg->evaluate(node);
         return trgReg;
         }
      else
         {
         trgReg = cg->allocateRegister();
         mulConstant32(node, trgReg, src1Reg, value, cg);
         }
      }
   else
      {
      TR::Register *src2Reg = cg->evaluate(secondChild);
      trgReg = cg->allocateRegister();
      generateRTYPE(TR::InstOpCode::_mul, node, trgReg, src1Reg, src2Reg, cg);
      }
   firstChild->decReferenceCount();
   secondChild->decReferenceCount();
   node->setRegister(trgReg);
   return trgReg;
   }

TR::Register *
OMR::ARM64::TreeEvaluator::imulhEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
   TR::Node *firstChild = node->getFirstChild();
   TR::Register *src1Reg = cg->evaluate(firstChild);
   TR::Node *secondChild = node->getSecondChild();
   TR::Register *src2Reg = cg->evaluate(secondChild);
   TR::Register *trgReg = cg->allocateRegister();

   generateRTYPE(TR::InstOpCode::_mul,  node, trgReg, src1Reg, src2Reg, cg);
   generateITYPE(TR::InstOpCode::_srai, node, trgReg, trgReg,  32, cg);

   firstChild->decReferenceCount();
   secondChild->decReferenceCount();
   node->setRegister(trgReg);
   return trgReg;

	}

TR::Register *
OMR::ARM64::TreeEvaluator::lmulEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::lmulEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

static TR::Register *idivHelper(TR::Node *node, bool is64bit, TR::CodeGenerator *cg)
   {
   // TODO: Add checks for special cases

   TR::Node *firstChild = node->getFirstChild();
   TR::Register *src1Reg = cg->evaluate(firstChild);
   TR::Node *secondChild = node->getSecondChild();
   TR::Register *src2Reg = cg->evaluate(secondChild);
   TR::Register *trgReg = cg->allocateRegister();

   generateRTYPE(is64bit ? TR::InstOpCode::_div : TR::InstOpCode::_divw, node, trgReg, src1Reg, src2Reg, cg);

   firstChild->decReferenceCount();
   secondChild->decReferenceCount();
   node->setRegister(trgReg);
   return trgReg;
   }

static TR::Register *iremHelper(TR::Node *node, bool is64bit, TR::CodeGenerator *cg)
   {
   // TODO: Add checks for special cases

   TR::Node *firstChild = node->getFirstChild();
   TR::Register *src1Reg = cg->evaluate(firstChild);
   TR::Node *secondChild = node->getSecondChild();
   TR::Register *src2Reg = cg->evaluate(secondChild);
   TR::Register *tmpReg = cg->allocateRegister();
   TR::Register *trgReg = cg->allocateRegister();

   generateRTYPE(is64bit ? TR::InstOpCode::_rem : TR::InstOpCode::_remw, node, trgReg, src1Reg, src2Reg, cg);

   cg->stopUsingRegister(tmpReg);
   firstChild->decReferenceCount();
   secondChild->decReferenceCount();
   node->setRegister(trgReg);
   return trgReg;
   }

TR::Register *
OMR::ARM64::TreeEvaluator::idivEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return idivHelper(node, false, cg);
   }

TR::Register *
OMR::ARM64::TreeEvaluator::iremEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return iremHelper(node, false, cg);
   }

TR::Register *
OMR::ARM64::TreeEvaluator::ldivEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return idivHelper(node, true, cg);
   }

TR::Register *
OMR::ARM64::TreeEvaluator::lremEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return iremHelper(node, true, cg);
   }

static TR::Register *shiftHelper(TR::Node *node, TR::InstOpCode::Mnemonic op, TR::CodeGenerator *cg)
   {
   TR::Node *firstChild = node->getFirstChild();
   TR::Node *secondChild = node->getSecondChild();
   TR::ILOpCodes secondOp = secondChild->getOpCodeValue();
   TR::Register *srcReg = cg->evaluate(firstChild);
   TR::Register *trgReg = cg->allocateRegister();

   if (secondOp == TR::iconst || secondOp == TR::iuconst)
      {
      int32_t value = secondChild->getInt();
      switch (op)
         {
         case TR::InstOpCode::_sllw:
            op = TR::InstOpCode::_slliw;
            break;
         case TR::InstOpCode::_srlw:
            op = TR::InstOpCode::_srliw;
            break;
         case TR::InstOpCode::_sraw:
            op = TR::InstOpCode::_sraiw;
            break;
         case TR::InstOpCode::_sll:
            op = TR::InstOpCode::_slli;
            break;
         case TR::InstOpCode::_srl:
            op = TR::InstOpCode::_srli;
            break;
         case TR::InstOpCode::_sra:
            op = TR::InstOpCode::_srai;
            break;
         default:
            TR_ASSERT(false, "Unsupported shift type.");
         }
      generateITYPE(op, node, trgReg, srcReg, value, cg);
      }
   else
      {
      TR::Register *shiftAmountReg = cg->evaluate(secondChild);
      generateRTYPE(op, node, trgReg, srcReg, shiftAmountReg, cg);
      }

   node->setRegister(trgReg);
   firstChild->decReferenceCount();
   secondChild->decReferenceCount();
   return trgReg;
   }

TR::Register *
OMR::ARM64::TreeEvaluator::ishlEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return shiftHelper(node, TR::InstOpCode::_sllw, cg);
   }

TR::Register *
OMR::ARM64::TreeEvaluator::ishrEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return shiftHelper(node, TR::InstOpCode::_sraw, cg);
   }

TR::Register *
OMR::ARM64::TreeEvaluator::iushrEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return shiftHelper(node, TR::InstOpCode::_srlw, cg);
   }

// also handles lrol
TR::Register *
OMR::ARM64::TreeEvaluator::irolEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Node *firstChild = node->getFirstChild();
   TR::Node *secondChild = node->getSecondChild();
   TR::Register *trgReg = cg->gprClobberEvaluate(firstChild);
   TR::Register *tmpReg = cg->allocateRegister();
   bool is64bit = node->getDataType().isInt64();
   TR::InstOpCode::Mnemonic op;

   /*
    * RISC-V Base Integer ISA lacks a native rotate instruction, so here we need
    * to use two shifts and or to implement a rotate, like (for 64bit):
    *
    *   sll tmp, src, shift
    *   srl dst, src, 64 - shift,
    *   or  dst, dst, tmp
    *
    * For 32bit, we must use .w instructions plus sign-extend the result with
    *
    *   sext.w dst  // aka addi.w dst, dst, 0
    *
    * TODO: in future, we may check for presence of B extension and use 'rot'
    * instruction (if it finds its way to B extensions)
    */
   if (secondChild->getOpCode().isLoadConst())
      {
      int32_t value = secondChild->getInt();
      uint32_t shift = is64bit ? (value & 0x3F) : (value & 0x1F);

      if (shift != 0)
         {
         if (is64bit)
            {
            generateITYPE(TR::InstOpCode::_slli, node, tmpReg, trgReg, shift, cg);
            generateITYPE(TR::InstOpCode::_srli, node, trgReg, trgReg, 64-shift, cg);
            generateRTYPE(TR::InstOpCode::_or,   node, trgReg, trgReg, tmpReg, cg);
            }
         else
            {
            generateITYPE(TR::InstOpCode::_slliw, node, tmpReg, trgReg, shift, cg);
            generateITYPE(TR::InstOpCode::_srliw, node, trgReg, trgReg, 32-shift, cg);
            generateRTYPE(TR::InstOpCode::_or,    node, trgReg, trgReg, tmpReg, cg);
            generateITYPE(TR::InstOpCode::_addiw, node, trgReg, trgReg, 0, cg);
            }
         }
      }
   else
      {
      TR::Register *shift1Reg = cg->evaluate(secondChild);
      TR::Register *shift2Reg = cg->allocateRegister();
      TR::Register *zero = cg->machine()->getRealRegister(TR::RealRegister::zero);

      generateRTYPE(TR::InstOpCode::_subw, node, shift2Reg, zero, shift1Reg, cg);
      if (is64bit)
         {
         generateRTYPE(TR::InstOpCode::_sll,   node, tmpReg, trgReg, shift1Reg, cg);
         generateRTYPE(TR::InstOpCode::_srl,   node, trgReg, trgReg, shift2Reg, cg);
         generateRTYPE(TR::InstOpCode::_or,    node, trgReg, trgReg, tmpReg, cg);
         }
      else
         {
         generateRTYPE(TR::InstOpCode::_sllw,  node, tmpReg, trgReg, shift1Reg, cg);
         generateRTYPE(TR::InstOpCode::_srlw,  node, trgReg, trgReg, shift2Reg, cg);
         generateRTYPE(TR::InstOpCode::_or,    node, trgReg, trgReg, tmpReg, cg);
         generateITYPE(TR::InstOpCode::_addiw, node, trgReg, trgReg, 0, cg);
         }
      cg->stopUsingRegister(shift2Reg);
      }
   cg->stopUsingRegister(tmpReg);

   node->setRegister(trgReg);
   firstChild->decReferenceCount();
   secondChild->decReferenceCount();
   return trgReg;
   }

TR::Register *
OMR::ARM64::TreeEvaluator::lshlEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return shiftHelper(node, TR::InstOpCode::_sll, cg);
   }

TR::Register *
OMR::ARM64::TreeEvaluator::lshrEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return shiftHelper(node, TR::InstOpCode::_sra, cg);
   }

TR::Register *
OMR::ARM64::TreeEvaluator::lushrEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return shiftHelper(node, TR::InstOpCode::_srl, cg);
   }

TR::Register *
OMR::ARM64::TreeEvaluator::landEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::landEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::lorEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::lorEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::lxorEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::lxorEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::iandEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::iandEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::iorEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::iorEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::ixorEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::ixorEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}
