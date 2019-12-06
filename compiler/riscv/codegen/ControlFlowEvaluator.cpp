/*******************************************************************************
 * Copyright (c) 2018, 2018 IBM Corp. and others
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

#include "codegen/ARM64Instruction.hpp"
#include "codegen/RVInstruction.hpp"
#include "codegen/CodeGenerator.hpp"
#include "codegen/GenerateInstructions.hpp"
#include "codegen/Linkage.hpp"
#include "codegen/RegisterDependency.hpp"
#include "codegen/TreeEvaluator.hpp"
#include "il/Node.hpp"
#include "il/Node_inlines.hpp"

TR::Register *
genericReturnEvaluator(TR::Node *node, TR::RealRegister::RegNum rnum, TR_RegisterKinds rk, TR_ReturnInfo i,  TR::CodeGenerator *cg)
   {
   TR::Node *firstChild = node->getFirstChild();
   TR::Register *returnRegister = cg->evaluate(firstChild);

   TR::RegisterDependencyConditions *deps = new (cg->trHeapMemory()) TR::RegisterDependencyConditions(1, 0, cg->trMemory());
   deps->addPreCondition(returnRegister, rnum);
   generateAdminInstruction(cg, TR::InstOpCode::retn, node, deps);

   cg->comp()->setReturnInfo(i);
   cg->decReferenceCount(firstChild);

   return NULL;
   }

// also handles iureturn
TR::Register *
OMR::ARM64::TreeEvaluator::ireturnEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return genericReturnEvaluator(node, cg->getProperties().getIntegerReturnRegister(), TR_GPR, TR_IntReturn, cg);
   }

// also handles lureturn, areturn
TR::Register *
OMR::ARM64::TreeEvaluator::lreturnEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return genericReturnEvaluator(node, cg->getProperties().getLongReturnRegister(), TR_GPR, TR_LongReturn, cg);
   }

// void return
TR::Register *
OMR::ARM64::TreeEvaluator::returnEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   generateAdminInstruction(cg, TR::InstOpCode::retn, node);
   cg->comp()->setReturnInfo(TR_VoidReturn);
   return NULL;
   }

TR::Register *OMR::ARM64::TreeEvaluator::gotoEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::LabelSymbol *gotoLabel = node->getBranchDestination()->getNode()->getLabel();
   TR::RealRegister *zero = cg->machine()->getRealRegister(TR::RealRegister::zero);
   if (node->getNumChildren() > 0)
      {
      TR::Node *child = node->getFirstChild();
      cg->evaluate(child);
      generateJTYPE(TR::InstOpCode::_jal, node, zero, gotoLabel,
            generateRegisterDependencyConditions(cg, child, 0), cg);
      cg->decReferenceCount(child);
      }
   else
      {
      generateJTYPE(TR::InstOpCode::_jal, node, zero, gotoLabel, cg);
      }
   return NULL;
   }

static TR::Instruction *ificmpHelper(TR::InstOpCode::Mnemonic op, TR::Node *node, bool reverse, TR::CodeGenerator *cg)
   {
   TR::Node *firstChild = node->getFirstChild();
   TR::Node *secondChild = node->getSecondChild();
   TR::Node *thirdChild = NULL;

   TR::Register *src1Reg = cg->evaluate(firstChild);
   TR::Register *src2Reg = cg->evaluate(secondChild);

   TR::LabelSymbol *dstLabel = node->getBranchDestination()->getNode()->getLabel();
   TR::Instruction *result;
   if (node->getNumChildren() == 3)
      {
      TR_ASSERT(false, "Not yet Implemented");
#if 0
      thirdChild = node->getChild(2);
      TR_ASSERT(thirdChild->getOpCodeValue() == TR::GlRegDeps, "The third child of a compare must be a TR::GlRegDeps");

      cg->evaluate(thirdChild);

      TR::RegisterDependencyConditions *deps = generateRegisterDependencyConditions(cg, thirdChild, 0);
      result = generateConditionalBranchInstruction(cg, TR::InstOpCode::b_cond, node, dstLabel, cc, deps);
#endif
      }
   else
      {
      if (reverse)
         result = generateBTYPE(op, node, dstLabel, src2Reg, src1Reg, cg);
      else
         result = generateBTYPE(op, node, dstLabel, src1Reg, src2Reg, cg);
      }

   firstChild->decReferenceCount();
   secondChild->decReferenceCount();
   if (thirdChild)
      {
      thirdChild->decReferenceCount();
      }
   return result;
   }

// also handles ifiucmpeq
TR::Register *
OMR::ARM64::TreeEvaluator::ificmpeqEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   ificmpHelper(TR::InstOpCode::_beq, node, false, cg);
   return NULL;
   }

// also handles ifiucmpne
TR::Register *
OMR::ARM64::TreeEvaluator::ificmpneEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   ificmpHelper(TR::InstOpCode::_bne, node, false, cg);
   return NULL;
   }

TR::Register *
OMR::ARM64::TreeEvaluator::ificmpltEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   ificmpHelper(TR::InstOpCode::_blt, node, false, cg);
   return NULL;
   }

TR::Register *
OMR::ARM64::TreeEvaluator::ificmpgeEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   ificmpHelper(TR::InstOpCode::_bge, node, false, cg);
   return NULL;
   }

TR::Register *
OMR::ARM64::TreeEvaluator::ificmpgtEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   ificmpHelper(TR::InstOpCode::_blt, node, true, cg);
   return NULL;
   }

TR::Register *
OMR::ARM64::TreeEvaluator::ificmpleEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   ificmpHelper(TR::InstOpCode::_bge, node, true, cg);
   return NULL;
   }

TR::Register *
OMR::ARM64::TreeEvaluator::ifiucmpltEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   ificmpHelper(TR::InstOpCode::_bltu, node, false, cg);
   return NULL;
   }

TR::Register *
OMR::ARM64::TreeEvaluator::ifiucmpgeEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   ificmpHelper(TR::InstOpCode::_bgeu, node, false, cg);
   return NULL;
   }

TR::Register *
OMR::ARM64::TreeEvaluator::ifiucmpgtEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   ificmpHelper(TR::InstOpCode::_bltu, node, true, cg);
   return NULL;
   }

TR::Register *
OMR::ARM64::TreeEvaluator::ifiucmpleEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   ificmpHelper(TR::InstOpCode::_bgeu, node, true, cg);
   return NULL;
   }

// also handles iflucmpeq, ifacmpeq
TR::Register *
OMR::ARM64::TreeEvaluator::iflcmpeqEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   ificmpHelper(TR::InstOpCode::_beq, node, false, cg);
   return NULL;
   }

// also handles iflucmpne, ifacmpne
TR::Register *
OMR::ARM64::TreeEvaluator::iflcmpneEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   ificmpHelper(TR::InstOpCode::_bne, node, false, cg);
   return NULL;
   }

TR::Register *
OMR::ARM64::TreeEvaluator::iflcmpltEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   ificmpHelper(TR::InstOpCode::_blt, node, false, cg);
   return NULL;
   }

TR::Register *
OMR::ARM64::TreeEvaluator::iflcmpgeEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   ificmpHelper(TR::InstOpCode::_bge, node, false, cg);
   return NULL;
   }

// also handles ifacmplt
TR::Register *
OMR::ARM64::TreeEvaluator::iflucmpltEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   ificmpHelper(TR::InstOpCode::_bltu, node, false, cg);
   return NULL;
   }

// also handles ifacmpge
TR::Register *
OMR::ARM64::TreeEvaluator::iflucmpgeEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   ificmpHelper(TR::InstOpCode::_bgeu, node, false, cg);
   return NULL;
   }

TR::Register *
OMR::ARM64::TreeEvaluator::iflcmpgtEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   ificmpHelper(TR::InstOpCode::_blt, node, true, cg);
   return NULL;
   }

TR::Register *
OMR::ARM64::TreeEvaluator::iflcmpleEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   ificmpHelper(TR::InstOpCode::_bge, node, true, cg);
   return NULL;
   }

// also handles ifacmpgt
TR::Register *
OMR::ARM64::TreeEvaluator::iflucmpgtEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   ificmpHelper(TR::InstOpCode::_bltu, node, true, cg);
   return NULL;
   }

// also handles ifacmple
TR::Register *
OMR::ARM64::TreeEvaluator::iflucmpleEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   ificmpHelper(TR::InstOpCode::_bgeu, node, true, cg);
   return NULL;
   }

static TR::Register *icmpHelper(TR::InstOpCode::Mnemonic op1, TR::InstOpCode::Mnemonic op2, uint32_t imm2, TR::Node *node, bool reverse, TR::CodeGenerator *cg)
   {
   TR::Register *trgReg = cg->allocateRegister();
   TR::Node *firstChild = node->getFirstChild();
   TR::Node *secondChild = node->getSecondChild();
   TR::Register *src1Reg = cg->evaluate(firstChild);
   TR::Register *src2Reg = cg->evaluate(secondChild);
   TR::Instruction *result = nullptr;

   if (reverse)
      result = generateRTYPE(op1, node, trgReg, src2Reg, src1Reg, cg);
   else
      result = generateRTYPE(op1, node, trgReg, src1Reg, src2Reg, cg);

   if (op2 != TR::InstOpCode::bad)
      {
      result = generateITYPE(op2, node, trgReg, trgReg, imm2, cg, result);
      }

   node->setRegister(trgReg);
   firstChild->decReferenceCount();
   secondChild->decReferenceCount();
   return trgReg;
   }

TR::Register *
OMR::ARM64::TreeEvaluator::icmpeqEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return icmpHelper(TR::InstOpCode::_sub, TR::InstOpCode::_sltiu, 1, node, false, cg);
   }

TR::Register *
OMR::ARM64::TreeEvaluator::icmpneEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   /*
    * We cannot use icmpHelper() because that one generates
    * RTYPE followed by ITYPE instruction. Here we need to
    * generate
    *
    *     sub result, left, right
    *     sltu result, zero, result
    *
    * i.e., RTYPE followed by RTYPE
    */
   TR::Register *trgReg = cg->allocateRegister();
   TR::Node *firstChild = node->getFirstChild();
   TR::Node *secondChild = node->getSecondChild();
   TR::Register *src1Reg = cg->evaluate(firstChild);
   TR::Register *src2Reg = cg->evaluate(secondChild);
   TR::Register *zero = cg->machine()->getRealRegister(TR::RealRegister::zero);
   TR::Instruction *result = nullptr;

   result = generateRTYPE(TR::InstOpCode::_sub, node, trgReg, src1Reg, src2Reg, cg);
   result = generateRTYPE(TR::InstOpCode::_sltu, node, trgReg, zero, trgReg, cg);


   node->setRegister(trgReg);
   firstChild->decReferenceCount();
   secondChild->decReferenceCount();
   return trgReg;

   }

TR::Register *
OMR::ARM64::TreeEvaluator::icmpltEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return icmpHelper(TR::InstOpCode::_slt, TR::InstOpCode::bad, 0, node, false, cg);
   }

TR::Register *
OMR::ARM64::TreeEvaluator::icmpleEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return icmpHelper(TR::InstOpCode::_slt, TR::InstOpCode::_sltiu, 1, node, true, cg);
   }

TR::Register *
OMR::ARM64::TreeEvaluator::icmpgeEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return icmpHelper(TR::InstOpCode::_slt, TR::InstOpCode::_xori, 1, node, false, cg);
   }

TR::Register *
OMR::ARM64::TreeEvaluator::icmpgtEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return icmpHelper(TR::InstOpCode::_slt, TR::InstOpCode::bad, 0, node, true, cg);
   }

TR::Register *
OMR::ARM64::TreeEvaluator::iucmpltEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return icmpHelper(TR::InstOpCode::_sltu, TR::InstOpCode::bad, 0, node, false, cg);
   }

TR::Register *
OMR::ARM64::TreeEvaluator::iucmpleEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return icmpHelper(TR::InstOpCode::_sltu, TR::InstOpCode::_xori, 1, node, true, cg);
   }

TR::Register *
OMR::ARM64::TreeEvaluator::iucmpgeEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return icmpHelper(TR::InstOpCode::_sltu, TR::InstOpCode::_xori, 1, node, false, cg);
   }

TR::Register *
OMR::ARM64::TreeEvaluator::iucmpgtEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return icmpHelper(TR::InstOpCode::_sltu, TR::InstOpCode::bad, 0, node, true, cg);
   }

// also handles lucmpeq
TR::Register *
OMR::ARM64::TreeEvaluator::lcmpeqEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR_ASSERT(TR::Compiler->target.is64Bit(), "RV32 not yet supported");
   return icmpeqEvaluator(node, cg);
   }

// also handles lucmpne
TR::Register *
OMR::ARM64::TreeEvaluator::lcmpneEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR_ASSERT(TR::Compiler->target.is64Bit(), "RV32 not yet supported");
   return icmpneEvaluator(node, cg);
   }

TR::Register *
OMR::ARM64::TreeEvaluator::lcmpltEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR_ASSERT(TR::Compiler->target.is64Bit(), "RV32 not yet supported");
   return icmpltEvaluator(node, cg);
   }

TR::Register *
OMR::ARM64::TreeEvaluator::lcmpgeEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR_ASSERT(TR::Compiler->target.is64Bit(), "RV32 not yet supported");
   return icmpgeEvaluator(node, cg);
   }

TR::Register *
OMR::ARM64::TreeEvaluator::lcmpgtEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR_ASSERT(TR::Compiler->target.is64Bit(), "RV32 not yet supported");
   return icmpgtEvaluator(node, cg);
   }

TR::Register *
OMR::ARM64::TreeEvaluator::lcmpleEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR_ASSERT(TR::Compiler->target.is64Bit(), "RV32 not yet supported");
   return icmpleEvaluator(node, cg);
   }

TR::Register *
OMR::ARM64::TreeEvaluator::lucmpltEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR_ASSERT(TR::Compiler->target.is64Bit(), "RV32 not yet supported");
   return iucmpltEvaluator(node, cg);
   }

TR::Register *
OMR::ARM64::TreeEvaluator::lucmpgeEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR_ASSERT(TR::Compiler->target.is64Bit(), "RV32 not yet supported");
   return iucmpgeEvaluator(node, cg);
   }

TR::Register *
OMR::ARM64::TreeEvaluator::lucmpgtEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR_ASSERT(TR::Compiler->target.is64Bit(), "RV32 not yet supported");
   return iucmpgtEvaluator(node, cg);
   }

TR::Register *
OMR::ARM64::TreeEvaluator::lucmpleEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR_ASSERT(TR::Compiler->target.is64Bit(), "RV32 not yet supported");
   return iucmpleEvaluator(node, cg);
   }

TR::Register *
OMR::ARM64::TreeEvaluator::lcmpEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Node *firstChild = node->getFirstChild();
   TR::Node *secondChild = node->getSecondChild();
   TR::Register *src1Reg = cg->evaluate(firstChild);
   TR::Register *src2Reg = cg->evaluate(secondChild);
   TR::Register *trgReg = cg->allocateRegister();
   TR::Register *tmpReg = cg->allocateRegister();

   /* Here we set up registers such that
    *
    *  - trgReg = 1  iff  src1Reg < src2Reg
    *  - tmpReg = 1  iff  src1Reg > src2Reg
    *
    */
   generateRTYPE(TR::InstOpCode::_slt, node, trgReg, src1Reg, src2Reg, cg);
   generateRTYPE(TR::InstOpCode::_slt, node, tmpReg, src2Reg, src1Reg, cg);

   /*
    * There are three outcomes possible:
    *
    *   (i) trgReg = 0, tmpReg = 1  => lcmp =>  1
    *  (ii) trgReg = 1, tmpReg = 0  => lcmp => -1
    * (iii) trgReg = 0, tmpReg = 0  => lcmp =>  0
    */
   generateRTYPE(TR::InstOpCode::_sub, node, trgReg, tmpReg, trgReg, cg);

   cg->stopUsingRegister(tmpReg);

   node->setRegister(trgReg);
   firstChild->decReferenceCount();
   secondChild->decReferenceCount();
   return trgReg;
   }

TR::Register *
OMR::ARM64::TreeEvaluator::acmpeqEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::acmpeqEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::lookupEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::lookupEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::tableEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::tableEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::NULLCHKEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::NULLCHKEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::ZEROCHKEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::ZEROCHKEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::ResolveAndNULLCHKEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::ResolveAndNULLCHKEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::DIVCHKEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::DIVCHKEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::BNDCHKEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::BNDCHKEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::ArrayCopyBNDCHKEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::ArrayCopyBNDCHKEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::ArrayStoreCHKEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::ArrayStoreCHKEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::ArrayCHKEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::ArrayCHKEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}
