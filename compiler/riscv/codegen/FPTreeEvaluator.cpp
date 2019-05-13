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

#include "codegen/CodeGenerator.hpp"
#include "codegen/RVInstruction.hpp"
#include "codegen/GenerateInstructions.hpp"
#include "codegen/Linkage.hpp"
#include "codegen/RegisterDependency.hpp"
#include "codegen/TreeEvaluator.hpp"
#include "il/Node.hpp"
#include "il/Node_inlines.hpp"

static void fpBitsMovHelper(TR::Node *node, TR::InstOpCode::Mnemonic op, TR::Register *trgReg, TR::CodeGenerator *cg)
   {
   TR::Node *child = node->getFirstChild();
   TR::Register *srcReg = cg->evaluate(child);
   TR::RealRegister *zero = cg->machine()->getRealRegister(TR::RealRegister::zero);

   generateRTYPE(op, node, trgReg, srcReg, zero, cg);

   cg->decReferenceCount(child);
   }

TR::Register *
OMR::ARM64::TreeEvaluator::ibits2fEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Register *trgReg = cg->allocateSinglePrecisionRegister();

   fpBitsMovHelper(node, TR::InstOpCode::_fmv_s_x, trgReg, cg);

   node->setRegister(trgReg);
   return trgReg;
   }

TR::Register *
OMR::ARM64::TreeEvaluator::fbits2iEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Register *trgReg = cg->allocateRegister();

   fpBitsMovHelper(node, TR::InstOpCode::_fmv_x_s, trgReg, cg);

   node->setRegister(trgReg);
   return trgReg;
   }

TR::Register *
OMR::ARM64::TreeEvaluator::lbits2dEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Register *trgReg = cg->allocateRegister(TR_FPR);

   fpBitsMovHelper(node, TR::InstOpCode::_fmv_d_x, trgReg, cg);

   node->setRegister(trgReg);
   return trgReg;
   }

TR::Register *
OMR::ARM64::TreeEvaluator::dbits2lEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Register *trgReg = cg->allocateRegister();

   fpBitsMovHelper(node, TR::InstOpCode::_fmv_x_d, trgReg, cg);

   node->setRegister(trgReg);
   return trgReg;
   }

TR::Register *
OMR::ARM64::TreeEvaluator::fconstEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Register *trgReg = cg->allocateSinglePrecisionRegister();
   TR::Register *tmpReg = cg->allocateRegister();
   TR::Register *zero = cg->machine()->getRealRegister(TR::RealRegister::zero);


   union {
      float f;
      int32_t i;
   } fvalue;

   fvalue.f = node->getFloat();
   loadConstant32(cg, node, fvalue.i, tmpReg);
   generateRTYPE(TR::InstOpCode::_fmv_s_x, node, trgReg, tmpReg, zero, cg);
   cg->stopUsingRegister(tmpReg);

   node->setRegister(trgReg);
   return trgReg;
   }

TR::Register *
OMR::ARM64::TreeEvaluator::dconstEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Register *trgReg = cg->allocateRegister(TR_FPR);
   TR::Register *tmpReg = cg->allocateRegister();
   TR::Register *zero = cg->machine()->getRealRegister(TR::RealRegister::zero);

   union {
      double d;
      int64_t l;
   } dvalue;

   dvalue.d = node->getDouble();
   loadConstant64(cg, node, dvalue.l, tmpReg);
   generateRTYPE(TR::InstOpCode::_fmv_d_x, node, trgReg, tmpReg, zero, cg);
   cg->stopUsingRegister(tmpReg);

   node->setRegister(trgReg);
   return trgReg;
   }

// also handles floadi
TR::Register *
OMR::ARM64::TreeEvaluator::floadEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return commonLoadEvaluator(node, TR::InstOpCode::_flw, 4, cg);
   }

// also handles dloadi
TR::Register *
OMR::ARM64::TreeEvaluator::dloadEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return commonLoadEvaluator(node, TR::InstOpCode::_fld, 8, cg);
   }

// also handles fstorei
TR::Register *
OMR::ARM64::TreeEvaluator::fstoreEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return commonStoreEvaluator(node, TR::InstOpCode::_fsw, 4, cg);
   }

// also handles dstorei
TR::Register *
OMR::ARM64::TreeEvaluator::dstoreEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return commonStoreEvaluator(node, TR::InstOpCode::_fsd, 8, cg);
   }

TR::Register *
OMR::ARM64::TreeEvaluator::freturnEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return genericReturnEvaluator(node, cg->getProperties().getFloatReturnRegister(), TR_FPR, TR_FloatReturn, cg);
   }

TR::Register *
OMR::ARM64::TreeEvaluator::dreturnEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return genericReturnEvaluator(node, cg->getProperties().getDoubleReturnRegister(), TR_FPR, TR_DoubleReturn, cg);
   }

static TR::Register *
commonFpEvaluator(TR::Node *node, TR::InstOpCode::Mnemonic op, bool isDouble, TR::CodeGenerator *cg)
   {
   TR::Node *firstChild = node->getFirstChild();
   TR::Node *secondChild = node->getSecondChild();
   TR::Register *src1Reg = cg->evaluate(firstChild);
   TR::Register *src2Reg = cg->evaluate(secondChild);
   TR::Register *trgReg;

   trgReg = isDouble ? cg->allocateRegister(TR_FPR) : cg->allocateSinglePrecisionRegister();
   generateRTYPE(op, node, trgReg, src1Reg, src2Reg, cg);
   cg->decReferenceCount(firstChild);
   cg->decReferenceCount(secondChild);
   node->setRegister(trgReg);
   return trgReg;
   }

static TR::Register *
singlePrecisionEvaluator(TR::Node *node, TR::InstOpCode::Mnemonic op, TR::CodeGenerator *cg)
   {
   return commonFpEvaluator(node, op, false, cg);
   }

static TR::Register *
doublePrecisionEvaluator(TR::Node *node, TR::InstOpCode::Mnemonic op, TR::CodeGenerator *cg)
   {
   return commonFpEvaluator(node, op, true, cg);
   }

TR::Register *
OMR::ARM64::TreeEvaluator::faddEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return singlePrecisionEvaluator(node, TR::InstOpCode::_fadd_s, cg);
   }

TR::Register *
OMR::ARM64::TreeEvaluator::daddEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return doublePrecisionEvaluator(node, TR::InstOpCode::_fadd_d, cg);
   }

TR::Register *
OMR::ARM64::TreeEvaluator::fsubEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return singlePrecisionEvaluator(node, TR::InstOpCode::_fsub_s, cg);
   }

TR::Register *
OMR::ARM64::TreeEvaluator::dsubEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return doublePrecisionEvaluator(node, TR::InstOpCode::_fsub_d, cg);
   }

TR::Register *
OMR::ARM64::TreeEvaluator::fmulEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return singlePrecisionEvaluator(node, TR::InstOpCode::_fmul_s, cg);
   }

TR::Register *
OMR::ARM64::TreeEvaluator::dmulEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return doublePrecisionEvaluator(node, TR::InstOpCode::_fmul_d, cg);
   }

TR::Register *
OMR::ARM64::TreeEvaluator::fdivEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return singlePrecisionEvaluator(node, TR::InstOpCode::_fdiv_s, cg);
   }

TR::Register *
OMR::ARM64::TreeEvaluator::ddivEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return doublePrecisionEvaluator(node, TR::InstOpCode::_fdiv_d, cg);
   }

TR::Register *
OMR::ARM64::TreeEvaluator::fremEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::fremEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::dremEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::dremEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

static TR::Register *
commonFpUnaryEvaluator(TR::Node *node, TR::InstOpCode::Mnemonic op, bool isDouble, TR::CodeGenerator *cg)
   {
   TR::Node *firstChild = node->getFirstChild();
   TR::Register *srcReg = cg->evaluate(firstChild);
   TR::Register *trgReg;

   if (node->getReferenceCount() > 1)
      {
      trgReg = isDouble ? cg->allocateRegister(TR_FPR) : cg->allocateSinglePrecisionRegister();
      }
   else
      {
      trgReg = srcReg;
      }
   generateRTYPE(op, node, trgReg, srcReg, srcReg, cg);
   cg->decReferenceCount(firstChild);
   node->setRegister(trgReg);
   return trgReg;
   }

static TR::Register *
singlePrecisionUnaryEvaluator(TR::Node *node, TR::InstOpCode::Mnemonic op, TR::CodeGenerator *cg)
   {
   return commonFpUnaryEvaluator(node, op, false, cg);
   }

static TR::Register *
doublePrecisionUnaryEvaluator(TR::Node *node, TR::InstOpCode::Mnemonic op, TR::CodeGenerator *cg)
   {
   return commonFpUnaryEvaluator(node, op, true, cg);
   }

TR::Register *
OMR::ARM64::TreeEvaluator::fabsEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return singlePrecisionUnaryEvaluator(node, TR::InstOpCode::_fsgnjx_s, cg);
   }

TR::Register *
OMR::ARM64::TreeEvaluator::dabsEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return doublePrecisionUnaryEvaluator(node, TR::InstOpCode::_fsgnjx_d, cg);
   }

TR::Register *
OMR::ARM64::TreeEvaluator::fnegEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return singlePrecisionUnaryEvaluator(node, TR::InstOpCode::_fsgnjn_s, cg);
   }

TR::Register *
OMR::ARM64::TreeEvaluator::dnegEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return doublePrecisionUnaryEvaluator(node, TR::InstOpCode::_fsgnjn_d, cg);
   }

TR::Register *
OMR::ARM64::TreeEvaluator::i2fEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::i2fEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::i2dEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::i2dEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::l2fEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::l2fEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::l2dEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::l2dEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::f2dEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::f2dEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::f2iEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::f2iEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::d2iEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::d2iEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::d2cEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::d2cEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::d2sEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::d2sEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::d2bEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::d2bEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::f2lEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::f2lEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::d2lEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::d2lEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::d2fEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::d2fEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::ifdcmpeqEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::ifdcmpeqEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::ifdcmpneEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::ifdcmpneEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::ifdcmpltEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::ifdcmpltEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::ifdcmpgeEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::ifdcmpgeEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::ifdcmpgtEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::ifdcmpgtEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::ifdcmpleEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::ifdcmpleEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::dcmpeqEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::dcmpeqEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::dcmpneEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::dcmpneEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::dcmpltEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::dcmpltEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::dcmplEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::dcmplEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

// also handles dRegLoad
TR::Register *
OMR::ARM64::TreeEvaluator::fRegLoadEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Register *globalReg = node->getRegister();

   if (globalReg == NULL)
      {
      if (node->getOpCodeValue() == TR::fRegLoad)
         globalReg = cg->allocateSinglePrecisionRegister();
      else
         globalReg = cg->allocateRegister(TR_FPR);
      node->setRegister(globalReg);
      }
   return globalReg;
   }

TR::Register *
OMR::ARM64::TreeEvaluator::fmaxEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::fmaxEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::fminEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::fminEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::b2dEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::b2dEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::s2dEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::s2dEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::su2dEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::su2dEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::ifdcmpequEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::ifdcmpequEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::ifdcmpltuEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::ifdcmpltuEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::ifdcmpgeuEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::ifdcmpgeuEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::ifdcmpgtuEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::ifdcmpgtuEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::ifdcmpleuEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::ifdcmpleuEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::dcmpequEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::dcmpequEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::dcmpltuEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::dcmpltuEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::dcmpgeuEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::dcmpgeuEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::dcmpgtuEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::dcmpgtuEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::dcmpleuEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::dcmpleuEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}

TR::Register *
OMR::ARM64::TreeEvaluator::dcmpgEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
	// TODO:ARM64: Enable TR::TreeEvaluator::dcmpgEvaluator in compiler/aarch64/codegen/TreeEvaluatorTable.hpp when Implemented.
	return OMR::ARM64::TreeEvaluator::unImpOpEvaluator(node, cg);
	}
