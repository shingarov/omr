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

/*
 * This file will be included within an enum. Only comments and enumerator
 * definitions are permitted.
 */

//		Opcode                                                         BINARY    	OPCODE    	comments
/* UNALLOCATED */
		bad,                                                    	/* 0x00000000	BAD       	invalid operation */
/*
 * RISC-V instructions
 */
#define DECLARE_INSN(mnemonic, match, mask) _ ## mnemonic,
#include <riscv-opc.h>
#undef DECLARE_INSN
/*
 * AArch64 instructions. They are defined for AArch64 source-level compatibility
 * only, they *must* die at some point.
 */
#ifdef TR_RISCV_ARM64_SOURCE_COMPAT
#define DECLARE_INSN(mnemonic) mnemonic,
#else
#define DECLARE_INSN(mnemonic) bad_ ## mnemonic,
#endif
/* Branch,exception generation and system Instruction */
	/* Compare _ Branch (immediate) */
		DECLARE_INSN(cbzw)                                                   	/* 0x34000000	CBZ       	 */
		DECLARE_INSN(cbnzw)                                                  	/* 0x35000000	CBNZ      	 */
		DECLARE_INSN(cbzx)                                                   	/* 0xB4000000	CBZ       	 */
		DECLARE_INSN(cbnzx)                                                  	/* 0xB5000000	CBNZ      	 */
	/* Test bit & branch (immediate) */
		DECLARE_INSN(tbz)                                                    	/* 0x36000000	TBZ       	 */
		DECLARE_INSN(tbnz)                                                   	/* 0x37000000	TBNZ      	 */
	/* Conditional branch (immediate) */
		DECLARE_INSN(b_cond)                                                 	/* 0x54000000	B_cond    	 */
	/* Exception generation */
		DECLARE_INSN(brkarm64)                                               	/* 0xD4200000	BRK       	AArch64 Specific BRK */
	/* System */
	/* Unconditional branch (register) */
		DECLARE_INSN(br)                                                     	/* 0xD61F0000	BR        	 */
		DECLARE_INSN(blr)                                                    	/* 0xD63F0000	BLR       	 */
		DECLARE_INSN(ret)                                                    	/* 0xD65F0000	RET       	 */
	/* Unconditional branch (immediate) */
		DECLARE_INSN(b)                                                      	/* 0x14000000	B         	 */
		DECLARE_INSN(bl)                                                     	/* 0x94000000	BL        	 */
/* Loads and stores */
	/* Load/store exclusive */
		DECLARE_INSN(stxrb)                                                  	/* 0x08000000	STXRB     	 */
		DECLARE_INSN(stlxrb)                                                 	/* 0x08008000	STLXRB    	 */
		DECLARE_INSN(ldxrb)                                                  	/* 0x08400000	LDXRB     	 */
		DECLARE_INSN(ldaxrb)                                                 	/* 0x08408000	LDAXRB    	 */
		DECLARE_INSN(stlrb)                                                  	/* 0x08808000	STLRB     	 */
		DECLARE_INSN(ldarb)                                                  	/* 0x08C08000	LDARB     	 */
		DECLARE_INSN(stxrh)                                                  	/* 0x48000000	STXRH     	 */
		DECLARE_INSN(stlxrh)                                                 	/* 0x48008000	STLXRH    	 */
		DECLARE_INSN(ldxrh)                                                  	/* 0x48400000	LDXRH     	 */
		DECLARE_INSN(ldaxrh)                                                 	/* 0x48408000	LDAXRH    	 */
		DECLARE_INSN(stlrh)                                                  	/* 0x48808000	STLRH     	 */
		DECLARE_INSN(ldarh)                                                  	/* 0x48C08000	LDARH     	 */
		DECLARE_INSN(stxrw)                                                  	/* 0x88000000	STXR      	 */
		DECLARE_INSN(stlxrw)                                                 	/* 0x88008000	STLXR     	 */
		DECLARE_INSN(stxpw)                                                  	/* 0x88200000	STXP      	 */
		DECLARE_INSN(stlxpw)                                                 	/* 0x88208000	STLXP     	 */
		DECLARE_INSN(ldxrw)                                                  	/* 0x88400000	LDXR      	 */
		DECLARE_INSN(ldaxrw)                                                 	/* 0x88408000	LDAXR     	 */
		DECLARE_INSN(ldxpw)                                                  	/* 0x88600000	LDXP      	 */
		DECLARE_INSN(ldaxpw)                                                 	/* 0x88608000	LDAXP     	 */
		DECLARE_INSN(stlrw)                                                  	/* 0x88808000	STLR      	 */
		DECLARE_INSN(ldarw)                                                  	/* 0x88C08000	LDAR      	 */
		DECLARE_INSN(stxrx)                                                  	/* 0xC8000000	STXR      	 */
		DECLARE_INSN(stlxrx)                                                 	/* 0xC8008000	STLXR     	 */
		DECLARE_INSN(stxpx)                                                  	/* 0xC8200000	STXP      	 */
		DECLARE_INSN(stlxpx)                                                 	/* 0xC8208000	STLXP     	 */
		DECLARE_INSN(ldxrx)                                                  	/* 0xC8400000	LDXR      	 */
		DECLARE_INSN(ldaxrx)                                                 	/* 0xC8408000	LDAXR     	 */
		DECLARE_INSN(ldxpx)                                                  	/* 0xC8600000	LDXP      	 */
		DECLARE_INSN(ldaxpx)                                                 	/* 0xC8608000	LDAXP     	 */
		DECLARE_INSN(stlrx)                                                  	/* 0xC8808000	STLR      	 */
		DECLARE_INSN(ldarx)                                                  	/* 0xC8C08000	LDAR      	 */
	/* Load register (literal) */
		DECLARE_INSN(ldrw)                                                   	/* 0x18000000	LDR       	 */
		DECLARE_INSN(vldrs)                                                  	/* 0x1C000000	LDR       	 */
		DECLARE_INSN(ldrx)                                                   	/* 0x58000000	LDR       	 */
		DECLARE_INSN(vldrd)                                                  	/* 0x5C000000	LDR       	 */
		DECLARE_INSN(ldrsw)                                                  	/* 0x98000000	LDRSW     	 */
		DECLARE_INSN(vldrq)                                                  	/* 0x9C000000	LDR       	 */
		DECLARE_INSN(prfm)                                                   	/* 0xD8000000	PRFM      	 */
	/* Load/store no-allocate pair (offset) */
		DECLARE_INSN(stnpw)                                                  	/* 0x28000000	STNP      	 */
		DECLARE_INSN(ldnpw)                                                  	/* 0x28400000	LDNP      	 */
		DECLARE_INSN(vstnps)                                                 	/* 0x2C000000	STNP      	 */
		DECLARE_INSN(vldnps)                                                 	/* 0x2C400000	LDNP      	 */
		DECLARE_INSN(vstnpd)                                                 	/* 0x6C000000	STNP      	 */
		DECLARE_INSN(vldnpd)                                                 	/* 0x6C400000	LDNP      	 */
		DECLARE_INSN(stnpx)                                                  	/* 0xA8000000	STNP      	 */
		DECLARE_INSN(ldnpx)                                                  	/* 0xA8400000	LDNP      	 */
		DECLARE_INSN(vstnpq)                                                 	/* 0xAC000000	STNP      	 */
		DECLARE_INSN(vldnpq)                                                 	/* 0xAC400000	LDNP      	 */
	/* Load/store register pair (post-indexed) */
		DECLARE_INSN(stppostw)                                               	/* 0x28800000	STP       	 */
		DECLARE_INSN(ldppostw)                                               	/* 0x28C00000	LDP       	 */
		DECLARE_INSN(vstpposts)                                              	/* 0x2C800000	STP       	 */
		DECLARE_INSN(vldpposts)                                              	/* 0x2CC00000	LDP       	 */
		DECLARE_INSN(ldpswpost)                                              	/* 0x68C00000	LDPSW     	 */
		DECLARE_INSN(vstppostd)                                              	/* 0x6C800000	STP       	 */
		DECLARE_INSN(vldppostd)                                              	/* 0x6CC00000	LDP       	 */
		DECLARE_INSN(stppostx)                                               	/* 0xA8800000	STP       	 */
		DECLARE_INSN(ldppostx)                                               	/* 0xA8C00000	LDP       	 */
		DECLARE_INSN(vstppostq)                                              	/* 0xAC800000	STP       	 */
		DECLARE_INSN(vldppostq)                                              	/* 0xACC00000	LDP       	 */
	/* Load/store register pair (offset) */
		DECLARE_INSN(stpoffw)                                                	/* 0x29000000	STP       	 */
		DECLARE_INSN(ldpoffw)                                                	/* 0x29400000	LDP       	 */
		DECLARE_INSN(vstpoffs)                                               	/* 0x2D000000	STP       	 */
		DECLARE_INSN(vldpoffs)                                               	/* 0x2D400000	LDP       	 */
		DECLARE_INSN(ldpswoff)                                               	/* 0x69400000	LDPSW     	 */
		DECLARE_INSN(vstpoffd)                                               	/* 0x6D000000	STP       	 */
		DECLARE_INSN(vldpoffd)                                               	/* 0x6D400000	LDP       	 */
		DECLARE_INSN(stpoffx)                                                	/* 0xA9000000	STP       	 */
		DECLARE_INSN(ldpoffx)                                                	/* 0xA9400000	LDP       	 */
		DECLARE_INSN(vstpoffq)                                               	/* 0xAD000000	STP       	 */
		DECLARE_INSN(vldpoffq)                                               	/* 0xAD400000	LDP       	 */
	/* Load/store register pair (pre-indexed) */
		DECLARE_INSN(stpprew)                                                	/* 0x29800000	STP       	 */
		DECLARE_INSN(ldpprew)                                                	/* 0x29C00000	LDP       	 */
		DECLARE_INSN(vstppres)                                               	/* 0x2D800000	STP       	 */
		DECLARE_INSN(vldppres)                                               	/* 0x2DC00000	LDP       	 */
		DECLARE_INSN(ldpswpre)                                               	/* 0x69C00000	LDPSW     	 */
		DECLARE_INSN(vstppred)                                               	/* 0x6D800000	STP       	 */
		DECLARE_INSN(vldppred)                                               	/* 0x6DC00000	LDP       	 */
		DECLARE_INSN(stpprex)                                                	/* 0xA9800000	STP       	 */
		DECLARE_INSN(ldpprex)                                                	/* 0xA9C00000	LDP       	 */
		DECLARE_INSN(vstppreq)                                               	/* 0xAD800000	STP       	 */
		DECLARE_INSN(vldppreq)                                               	/* 0xADC00000	LDP       	 */
	/* Load/store register (unscaled immediate) */
		DECLARE_INSN(sturb)                                                  	/* 0x38000000	STURB     	 */
		DECLARE_INSN(ldurb)                                                  	/* 0x38400000	LDURB     	 */
		DECLARE_INSN(ldursbx)                                                	/* 0x38800000	LDURSB    	 */
		DECLARE_INSN(ldursbw)                                                	/* 0x38C00000	LDURSB    	 */
		DECLARE_INSN(vsturb)                                                 	/* 0x3C000000	STUR      	 */
		DECLARE_INSN(vldurb)                                                 	/* 0x3C400000	LDUR      	 */
		DECLARE_INSN(vsturq)                                                 	/* 0x3C800000	STUR      	 */
		DECLARE_INSN(vldurq)                                                 	/* 0x3CC00000	LDUR      	 */
		DECLARE_INSN(sturh)                                                  	/* 0x78000000	STURH     	 */
		DECLARE_INSN(ldurh)                                                  	/* 0x78400000	LDURH     	 */
		DECLARE_INSN(ldurshx)                                                	/* 0x78800000	LDURSH    	 */
		DECLARE_INSN(ldurshw)                                                	/* 0x78C00000	LDURSH    	 */
		DECLARE_INSN(vsturh)                                                 	/* 0x7C000000	STUR      	 */
		DECLARE_INSN(vldurh)                                                 	/* 0x7C400000	LDUR      	 */
		DECLARE_INSN(sturw)                                                  	/* 0xB8000000	STUR      	 */
		DECLARE_INSN(ldurw)                                                  	/* 0xB8400000	LDUR      	 */
		DECLARE_INSN(ldursw)                                                 	/* 0xB8800000	LDURSW    	 */
		DECLARE_INSN(vsturs)                                                 	/* 0xBC000000	STUR      	 */
		DECLARE_INSN(vldurs)                                                 	/* 0xBC400000	LDUR      	 */
		DECLARE_INSN(sturx)                                                  	/* 0xF8000000	STUR      	 */
		DECLARE_INSN(ldurx)                                                  	/* 0xF8400000	LDUR      	 */
		DECLARE_INSN(prfum)                                                  	/* 0xF8800000	PRFUM     	 */
		DECLARE_INSN(vsturd)                                                 	/* 0xFC000000	STUR      	 */
		DECLARE_INSN(vldurd)                                                 	/* 0xFC400000	LDUR      	 */
	/* Load/store register (immediate post-indexed) */
		DECLARE_INSN(strbpost)                                               	/* 0x38000400	STRB      	 */
		DECLARE_INSN(ldrbpost)                                               	/* 0x38400400	LDRB      	 */
		DECLARE_INSN(ldrsbpostx)                                             	/* 0x38800400	LDRSB     	 */
		DECLARE_INSN(ldrsbpostw)                                             	/* 0x38C00400	LDRSB     	 */
		DECLARE_INSN(vstrpostb)                                              	/* 0x3C000400	STR       	 */
		DECLARE_INSN(vldrpostb)                                              	/* 0x3C400400	LDR       	 */
		DECLARE_INSN(vstrpostq)                                              	/* 0x3C800400	STR       	 */
		DECLARE_INSN(vldrpostq)                                              	/* 0x3CC00400	LDR       	 */
		DECLARE_INSN(strhpost)                                               	/* 0x78000400	STRH      	 */
		DECLARE_INSN(ldrhpost)                                               	/* 0x78400400	LDRH      	 */
		DECLARE_INSN(ldrshpostx)                                             	/* 0x78800400	LDRSH     	 */
		DECLARE_INSN(ldrshpostw)                                             	/* 0x78C00400	LDRSH     	 */
		DECLARE_INSN(vstrposth)                                              	/* 0x7C000400	STR       	 */
		DECLARE_INSN(vldrposth)                                              	/* 0x7C400400	LDR       	 */
		DECLARE_INSN(strpostw)                                               	/* 0xB8000400	STR       	 */
		DECLARE_INSN(ldrpostw)                                               	/* 0xB8400400	LDR       	 */
		DECLARE_INSN(ldrswpost)                                              	/* 0xB8800400	LDRSW     	 */
		DECLARE_INSN(vstrposts)                                              	/* 0xBC000400	STR       	 */
		DECLARE_INSN(vldrposts)                                              	/* 0xBC400400	LDR       	 */
		DECLARE_INSN(strpostx)                                               	/* 0xF8000400	STR       	 */
		DECLARE_INSN(ldrpostx)                                               	/* 0xF8400400	LDR       	 */
		DECLARE_INSN(vstrpostd)                                              	/* 0xFC000400	STR       	 */
		DECLARE_INSN(vldrpostd)                                              	/* 0xFC400400	LDR       	 */
	/* Load/store register (unprivileged) */
		DECLARE_INSN(sttrb)                                                  	/* 0x38000800	STTRB     	 */
		DECLARE_INSN(ldtrb)                                                  	/* 0x38400800	LDTRB     	 */
		DECLARE_INSN(ldtrsbx)                                                	/* 0x38800800	LDTRSB    	 */
		DECLARE_INSN(ldtrsbw)                                                	/* 0x38C00800	LDTRSB    	 */
		DECLARE_INSN(sttrh)                                                  	/* 0x78000800	STTRH     	 */
		DECLARE_INSN(ldtrh)                                                  	/* 0x78400800	LDTRH     	 */
		DECLARE_INSN(ldtrshx)                                                	/* 0x78800800	LDTRSH    	 */
		DECLARE_INSN(ldtrshw)                                                	/* 0x78C00800	LDTRSH    	 */
		DECLARE_INSN(sttrw)                                                  	/* 0xB8000800	STTR      	 */
		DECLARE_INSN(ldtrw)                                                  	/* 0xB8400800	LDTR      	 */
		DECLARE_INSN(ldtrsw)                                                 	/* 0xB8800800	LDTRSW    	 */
		DECLARE_INSN(sttrx)                                                  	/* 0xF8000800	STTR      	 */
		DECLARE_INSN(ldtrx)                                                  	/* 0xF8400800	LDTR      	 */
	/* Load/store register (immediate pre-indexed) */
		DECLARE_INSN(strbpre)                                                	/* 0x38000C00	STRB      	 */
		DECLARE_INSN(ldrbpre)                                                	/* 0x38400C00	LDRB      	 */
		DECLARE_INSN(ldrsbprex)                                              	/* 0x38800C00	LDRSB     	 */
		DECLARE_INSN(ldrsbprew)                                              	/* 0x38C00C00	LDRSB     	 */
		DECLARE_INSN(vstrpreb)                                               	/* 0x3C000C00	STR       	 */
		DECLARE_INSN(vldrpreb)                                               	/* 0x3C400C00	LDR       	 */
		DECLARE_INSN(vstrpreq)                                               	/* 0x3C800C00	STR       	 */
		DECLARE_INSN(vldrpreq)                                               	/* 0x3CC00C00	LDR       	 */
		DECLARE_INSN(strhpre)                                                	/* 0x78000C00	STRH      	 */
		DECLARE_INSN(ldrhpre)                                                	/* 0x78400C00	LDRH      	 */
		DECLARE_INSN(ldrshprex)                                              	/* 0x78800C00	LDRSH     	 */
		DECLARE_INSN(ldrshprew)                                              	/* 0x78C00C00	LDRSH     	 */
		DECLARE_INSN(vstrpreh)                                               	/* 0x7C000C00	STR       	 */
		DECLARE_INSN(vldrpreh)                                               	/* 0x7C400C00	LDR       	 */
		DECLARE_INSN(strprew)                                                	/* 0xB8000C00	STR       	 */
		DECLARE_INSN(ldrprew)                                                	/* 0xB8400C00	LDR       	 */
		DECLARE_INSN(ldrswpre)                                               	/* 0xB8800C00	LDRSW     	 */
		DECLARE_INSN(vstrpres)                                               	/* 0xBC000C00	STR       	 */
		DECLARE_INSN(vldrpres)                                               	/* 0xBC400C00	LDR       	 */
		DECLARE_INSN(strprex)                                                	/* 0xF8000C00	STR       	 */
		DECLARE_INSN(ldrprex)                                                	/* 0xF8400C00	LDR       	 */
		DECLARE_INSN(vstrpred)                                               	/* 0xFC000C00	STR       	 */
		DECLARE_INSN(vldrpred)                                               	/* 0xFC400C00	LDR       	 */
	/* Load/store register (register offset) */
		DECLARE_INSN(strboff)                                                	/* 0x38200800	STRB      	 */
		DECLARE_INSN(ldrboff)                                                	/* 0x38600800	LDRB      	 */
		DECLARE_INSN(ldrsboffx)                                              	/* 0x38A00800	LDRSB     	 */
		DECLARE_INSN(ldrsboffw)                                              	/* 0x38E00800	LDRSB     	 */
		DECLARE_INSN(vstroffb)                                               	/* 0x3C200800	STR       	 */
		DECLARE_INSN(vldroffb)                                               	/* 0x3C600800	LDR       	 */
		DECLARE_INSN(vstroffq)                                               	/* 0x3CA00800	STR       	 */
		DECLARE_INSN(vldroffq)                                               	/* 0x3CE00800	LDR       	 */
		DECLARE_INSN(strhoff)                                                	/* 0x78200800	STRH      	 */
		DECLARE_INSN(ldrhoff)                                                	/* 0x78600800	LDRH      	 */
		DECLARE_INSN(ldrshoffx)                                              	/* 0x78A00800	LDRSH     	 */
		DECLARE_INSN(ldrshoffw)                                              	/* 0x78E00800	LDRSH     	 */
		DECLARE_INSN(vstroffh)                                               	/* 0x7C200800	STR       	 */
		DECLARE_INSN(vldroffh)                                               	/* 0x7C600800	LDR       	 */
		DECLARE_INSN(stroffw)                                                	/* 0xB8200800	STR       	 */
		DECLARE_INSN(ldroffw)                                                	/* 0xB8600800	LDR       	 */
		DECLARE_INSN(ldrswoff)                                               	/* 0xB8A00800	LDRSW     	 */
		DECLARE_INSN(vstroffs)                                               	/* 0xBC200800	STR       	 */
		DECLARE_INSN(vldroffs)                                               	/* 0xBC600800	LDR       	 */
		DECLARE_INSN(stroffx)                                                	/* 0xF8200800	STR       	 */
		DECLARE_INSN(ldroffx)                                                	/* 0xF8600800	LDR       	 */
		DECLARE_INSN(vstroffd)                                               	/* 0xFC200800	STR       	 */
		DECLARE_INSN(vldroffd)                                               	/* 0xFC600800	LDR       	 */
		DECLARE_INSN(prfmoff)                                                	/* 0xF8A00800	PRFM      	 */
	/* Load/store register (unsigned immediate) */
		DECLARE_INSN(strbimm)                                                	/* 0x39000000	STRB      	 */
		DECLARE_INSN(ldrbimm)                                                	/* 0x39400000	LDRB      	 */
		DECLARE_INSN(ldrsbimmx)                                              	/* 0x39800000	LDRSB     	 */
		DECLARE_INSN(ldrsbimmw)                                              	/* 0x39C00000	LDRSB     	 */
		DECLARE_INSN(vstrimmb)                                               	/* 0x3D000000	STR       	 */
		DECLARE_INSN(vldrimmb)                                               	/* 0x3D400000	LDR       	 */
		DECLARE_INSN(vstrimmq)                                               	/* 0x3D800000	STR       	 */
		DECLARE_INSN(vldrimmq)                                               	/* 0x3DC00000	LDR       	 */
		DECLARE_INSN(strhimm)                                                	/* 0x79000000	STRH      	 */
		DECLARE_INSN(ldrhimm)                                                	/* 0x79400000	LDRH      	 */
		DECLARE_INSN(ldrshimmx)                                              	/* 0x79800000	LDRSH     	 */
		DECLARE_INSN(ldrshimmw)                                              	/* 0x79C00000	LDRSH     	 */
		DECLARE_INSN(vstrimmh)                                               	/* 0x7D000000	STR       	 */
		DECLARE_INSN(vldrimmh)                                               	/* 0x7D400000	LDR       	 */
		DECLARE_INSN(strimmw)                                                	/* 0xB9000000	STR       	 */
		DECLARE_INSN(ldrimmw)                                                	/* 0xB9400000	LDR       	 */
		DECLARE_INSN(ldrswimm)                                               	/* 0xB9800000	LDRSW     	 */
		DECLARE_INSN(vstrimms)                                               	/* 0xBD000000	STR       	 */
		DECLARE_INSN(vldrimms)                                               	/* 0xBD400000	LDR       	 */
		DECLARE_INSN(strimmx)                                                	/* 0xF9000000	STR       	 */
		DECLARE_INSN(ldrimmx)                                                	/* 0xF9400000	LDR       	 */
		DECLARE_INSN(vstrimmd)                                               	/* 0xFD000000	STR       	 */
		DECLARE_INSN(vldrimmd)                                               	/* 0xFD400000	LDR       	 */
		DECLARE_INSN(prfmimm)                                                	/* 0xF9800000	PRFM      	 */
/* Data processing - Immediate */
	/* PC-rel. addressing */
		DECLARE_INSN(adr)                                                    	/* 0x10000000	ADR       	 */
		DECLARE_INSN(adrp)                                                   	/* 0x90000000	ADRP      	 */
	/* Add/subtract (immediate) */
		DECLARE_INSN(addimmw)                                                	/* 0x11000000	ADD       	 */
		DECLARE_INSN(addsimmw)                                               	/* 0x31000000	ADDS      	 */
		DECLARE_INSN(subimmw)                                                	/* 0x51000000	SUB       	 */
		DECLARE_INSN(subsimmw)                                               	/* 0x71000000	SUBS      	 */
		DECLARE_INSN(addimmx)                                                	/* 0x91000000	ADD       	 */
		DECLARE_INSN(addsimmx)                                               	/* 0xB1000000	ADDS      	 */
		DECLARE_INSN(subimmx)                                                	/* 0xD1000000	SUB       	 */
		DECLARE_INSN(subsimmx)                                               	/* 0xF1000000	SUBS      	 */
	/* Logical (immediate) */
		DECLARE_INSN(andimmw)                                                	/* 0x12000000	AND       	 */
		DECLARE_INSN(orrimmw)                                                	/* 0x32000000	ORR       	 */
		DECLARE_INSN(eorimmw)                                                	/* 0x52000000	EOR       	 */
		DECLARE_INSN(andsimmw)                                               	/* 0x72000000	ANDS      	 */
		DECLARE_INSN(andimmx)                                                	/* 0x92000000	AND       	 */
		DECLARE_INSN(orrimmx)                                                	/* 0xB2000000	ORR       	 */
		DECLARE_INSN(eorimmx)                                                	/* 0xD2000000	EOR       	 */
		DECLARE_INSN(andsimmx)                                               	/* 0xF2000000	ANDS      	 */
	/* Move wide (immediate) */
		DECLARE_INSN(movnw)                                                  	/* 0x12800000	MOVN      	 */
		DECLARE_INSN(movzw)                                                  	/* 0x52800000	MOVZ      	 */
		DECLARE_INSN(movkw)                                                  	/* 0x72800000	MOVK      	 */
		DECLARE_INSN(movnx)                                                  	/* 0x92800000	MOVN      	 */
		DECLARE_INSN(movzx)                                                  	/* 0xD2800000	MOVZ      	 */
		DECLARE_INSN(movkx)                                                  	/* 0xF2800000	MOVK      	 */
	/* Bitfield */
		DECLARE_INSN(sbfmw)                                                  	/* 0x13000000	SBFM      	 */
		DECLARE_INSN(bfmw)                                                   	/* 0x33000000	BFM       	 */
		DECLARE_INSN(ubfmw)                                                  	/* 0x53000000	UBFM      	 */
		DECLARE_INSN(sbfmx)                                                  	/* 0x93400000	SBFM      	 */
		DECLARE_INSN(bfmx)                                                   	/* 0xB3400000	BFM       	 */
		DECLARE_INSN(ubfmx)                                                  	/* 0xD3400000	UBFM      	 */
	/* Extract */
		DECLARE_INSN(extrw)                                                  	/* 0x13800000	EXTR      	 */
		DECLARE_INSN(extrx)                                                  	/* 0x93C08000	EXTR      	 */
/* Data Processing - register */
	/* Logical (shifted register) */
		DECLARE_INSN(andw)                                                   	/* 0x0A000000	AND       	 */
		DECLARE_INSN(bicw)                                                   	/* 0x0A200000	BIC       	 */
		DECLARE_INSN(orrw)                                                   	/* 0x2A000000	ORR       	 */
		DECLARE_INSN(ornw)                                                   	/* 0x2A200000	ORN       	 */
		DECLARE_INSN(eorw)                                                   	/* 0x4A000000	EOR       	 */
		DECLARE_INSN(eonw)                                                   	/* 0x4A200000	EON       	 */
		DECLARE_INSN(andsw)                                                  	/* 0x6A000000	ANDS      	 */
		DECLARE_INSN(bicsw)                                                  	/* 0x6A200000	BICS      	 */
		DECLARE_INSN(andx)                                                   	/* 0x8A000000	AND       	 */
		DECLARE_INSN(bicx)                                                   	/* 0x8A200000	BIC       	 */
		DECLARE_INSN(orrx)                                                   	/* 0xAA000000	ORR       	 */
		DECLARE_INSN(ornx)                                                   	/* 0xAA200000	ORN       	 */
		DECLARE_INSN(eorx)                                                   	/* 0xCA000000	EOR       	 */
		DECLARE_INSN(eonx)                                                   	/* 0xCA200000	EON       	 */
		DECLARE_INSN(andsx)                                                  	/* 0xEA000000	ANDS      	 */
		DECLARE_INSN(bicsx)                                                  	/* 0xEA200000	BICS      	 */
	/* Add/subtract (shifted register) */
		DECLARE_INSN(addw)                                                   	/* 0x0B000000	ADD       	 */
		DECLARE_INSN(addsw)                                                  	/* 0x2B000000	ADDS      	 */
		DECLARE_INSN(subw)                                                   	/* 0x4B000000	SUB       	 */
		DECLARE_INSN(subsw)                                                  	/* 0x6B000000	SUBS      	 */
		DECLARE_INSN(addx)                                                   	/* 0x8B000000	ADD       	 */
		DECLARE_INSN(addsx)                                                  	/* 0xAB000000	ADDS      	 */
		DECLARE_INSN(subx)                                                   	/* 0xCB000000	SUB       	 */
		DECLARE_INSN(subsx)                                                  	/* 0xEB000000	SUBS      	 */
	/* Add/subtract (extended register) */
		DECLARE_INSN(addextw)                                                	/* 0x0B200000	ADD       	 */
		DECLARE_INSN(addsextw)                                               	/* 0x2B200000	ADDS      	 */
		DECLARE_INSN(subextw)                                                	/* 0x4B200000	SUB       	 */
		DECLARE_INSN(subsextw)                                               	/* 0x6B200000	SUBS      	 */
		DECLARE_INSN(addextx)                                                	/* 0x8B200000	ADD       	 */
		DECLARE_INSN(addsextx)                                               	/* 0xAB200000	ADDS      	 */
		DECLARE_INSN(subextx)                                                	/* 0xCB200000	SUB       	 */
		DECLARE_INSN(subsextx)                                               	/* 0xEB200000	SUBS      	 */
	/* Add/subtract (with carry) */
		DECLARE_INSN(adcw)                                                   	/* 0x1A000000	ADC       	 */
		DECLARE_INSN(adcsw)                                                  	/* 0x3A000000	ADCS      	 */
		DECLARE_INSN(sbcw)                                                   	/* 0x5A000000	SBC       	 */
		DECLARE_INSN(sbcsw)                                                  	/* 0x7A000000	SBCS      	 */
		DECLARE_INSN(adcx)                                                   	/* 0x9A000000	ADC       	 */
		DECLARE_INSN(adcsx)                                                  	/* 0xBA000000	ADCS      	 */
		DECLARE_INSN(sbcx)                                                   	/* 0xDA000000	SBC       	 */
		DECLARE_INSN(sbcsx)                                                  	/* 0xFA000000	SBCS      	 */
	/* Conditional compare (register) */
		DECLARE_INSN(ccmnw)                                                  	/* 0x3A400000	CCMN      	 */
		DECLARE_INSN(ccmnx)                                                  	/* 0xBA400000	CCMN      	 */
		DECLARE_INSN(ccmpw)                                                  	/* 0x7A400000	CCMP      	 */
		DECLARE_INSN(ccmpx)                                                  	/* 0xFA400000	CCMP      	 */
	/* Conditional compare (immediate) */
		DECLARE_INSN(ccmnimmw)                                               	/* 0x3A400800	CCMN      	 */
		DECLARE_INSN(ccmnimmx)                                               	/* 0xBA400800	CCMN      	 */
		DECLARE_INSN(ccmpimmw)                                               	/* 0x7A400800	CCMP      	 */
		DECLARE_INSN(ccmpimmx)                                               	/* 0xFA400800	CCMP      	 */
	/* Conditional select */
		DECLARE_INSN(cselw)                                                  	/* 0x1A800000	CSEL      	 */
		DECLARE_INSN(csincw)                                                 	/* 0x1A800400	CSINC     	 */
		DECLARE_INSN(csinvw)                                                 	/* 0x5A800000	CSINV     	 */
		DECLARE_INSN(csnegw)                                                 	/* 0x5A800400	CSNEG     	 */
		DECLARE_INSN(cselx)                                                  	/* 0x9A800000	CSEL      	 */
		DECLARE_INSN(csincx)                                                 	/* 0x9A800400	CSINC     	 */
		DECLARE_INSN(csinvx)                                                 	/* 0xDA800000	CSINV     	 */
		DECLARE_INSN(csnegx)                                                 	/* 0xDA800400	CSNEG     	 */
	/* Data-processing (3 source) */
		DECLARE_INSN(maddw)                                                  	/* 0x1B000000	MADD      	 */
		DECLARE_INSN(maddx)                                                  	/* 0x9B000000	MADD      	 */
		DECLARE_INSN(smaddl)                                                 	/* 0x9B200000	SMADDL    	 */
		DECLARE_INSN(umaddl)                                                 	/* 0x9BA00000	UMADDL    	 */
		DECLARE_INSN(msubw)                                                  	/* 0x1B008000	MSUB      	 */
		DECLARE_INSN(msubx)                                                  	/* 0x9B008000	MSUB      	 */
		DECLARE_INSN(smsubl)                                                 	/* 0x9B208000	SMSUBL    	 */
		DECLARE_INSN(umsubl)                                                 	/* 0x9BA08000	UMSUBL    	 */
		DECLARE_INSN(smulh)                                                  	/* 0x9B400000	SMULH     	 */
		DECLARE_INSN(umulh)                                                  	/* 0x9BC00000	UMULH     	 */
	/* Data-processing (2 source) */
		DECLARE_INSN(crc32x)                                                 	/* 0x9AC04C00	CRC32X    	 */
		DECLARE_INSN(crc32cx)                                                	/* 0x9AC05C00	CRC32CX   	 */
		DECLARE_INSN(crc32b)                                                 	/* 0x1AC04000	CRC32B    	 */
		DECLARE_INSN(crc32cb)                                                	/* 0x1AC05000	CRC32CB   	 */
		DECLARE_INSN(crc32h)                                                 	/* 0x1AC04400	CRC32H    	 */
		DECLARE_INSN(crc32ch)                                                	/* 0x1AC05400	CRC32CH   	 */
		DECLARE_INSN(crc32w)                                                 	/* 0x1AC04800	CRC32W    	 */
		DECLARE_INSN(crc32cw)                                                	/* 0x1AC05800	CRC32CW   	 */
		DECLARE_INSN(udivw)                                                  	/* 0x1AC00800	UDIV      	 */
		DECLARE_INSN(udivx)                                                  	/* 0x9AC00800	UDIV      	 */
		DECLARE_INSN(sdivw)                                                  	/* 0x1AC00C00	SDIV      	 */
		DECLARE_INSN(sdivx)                                                  	/* 0x9AC00C00	SDIV      	 */
		DECLARE_INSN(lslvw)                                                  	/* 0x1AC02000	LSLV      	 */
		DECLARE_INSN(lslvx)                                                  	/* 0x9AC02000	LSLV      	 */
		DECLARE_INSN(lsrvw)                                                  	/* 0x1AC02400	LSRV      	 */
		DECLARE_INSN(lsrvx)                                                  	/* 0x9AC02400	LSRV      	 */
		DECLARE_INSN(asrvw)                                                  	/* 0x1AC02800	ASRV      	 */
		DECLARE_INSN(asrvx)                                                  	/* 0x9AC02800	ASRV      	 */
		DECLARE_INSN(rorvw)                                                  	/* 0x1AC02C00	RORV      	 */
		DECLARE_INSN(rorvx)                                                  	/* 0x9AC02C00	RORV      	 */
	/* Data-processing (1 source) */
		DECLARE_INSN(rbitw)                                                  	/* 0x5AC00000	RBIT      	 */
		DECLARE_INSN(rbitx)                                                  	/* 0xDAC00000	RBIT      	 */
		DECLARE_INSN(clzw)                                                   	/* 0x5AC01000	CLZ       	 */
		DECLARE_INSN(clzx)                                                   	/* 0xDAC01000	CLZ       	 */
		DECLARE_INSN(clsw)                                                   	/* 0x5AC01400	CLS       	 */
		DECLARE_INSN(clsx)                                                   	/* 0xDAC01400	CLS       	 */
		DECLARE_INSN(revw)                                                   	/* 0x5AC00800	REV       	 */
		DECLARE_INSN(revx)                                                   	/* 0xDAC00C00	REV       	 */
		DECLARE_INSN(rev16w)                                                 	/* 0xDAC00400	REV16     	 */
		DECLARE_INSN(rev16x)                                                 	/* 0x5AC00400	REV16     	 */
		DECLARE_INSN(rev32)                                                  	/* 0xDAC00800	REV32     	 */
/* VFP instructions */
	/* Floating-Point Conversion */
		DECLARE_INSN(fmov_stow)                                              	/* 0x1E260000	FMOV      	 */
		DECLARE_INSN(fmov_wtos)                                              	/* 0x1E270000	FMOV      	 */
		DECLARE_INSN(fmov_dtox)                                              	/* 0x9E660000	FMOV      	 */
		DECLARE_INSN(fmov_xtod)                                              	/* 0x9E670000	FMOV      	 */
		DECLARE_INSN(fcvt_stod)                                              	/* 0x1E22C000	FCVT      	 */
		DECLARE_INSN(fcvt_dtos)                                              	/* 0x1E624000	FCVT      	 */
		DECLARE_INSN(fcvtzs_stow)                                            	/* 0x1E380000	FCVTZS    	 */
		DECLARE_INSN(fcvtzs_dtow)                                            	/* 0x1E780000	FCVTZS    	 */
		DECLARE_INSN(fcvtzs_stox)                                            	/* 0x9E380000	FCVTZS    	 */
		DECLARE_INSN(fcvtzs_dtox)                                            	/* 0x9E780000	FCVTZS    	 */
		DECLARE_INSN(scvtf_wtos)                                             	/* 0x1E220000	SCVTF     	 */
		DECLARE_INSN(scvtf_wtod)                                             	/* 0x1E620000	SCVTF     	 */
		DECLARE_INSN(scvtf_xtos)                                             	/* 0x9E220000	SCVTF     	 */
		DECLARE_INSN(scvtf_xtod)                                             	/* 0x9E620000	SCVTF     	 */
	/* Floating-Point Immediate */
		DECLARE_INSN(fmovimms)                                               	/* 0x1E201000	FMOV      	 */
		DECLARE_INSN(fmovimmd)                                               	/* 0x1E601000	FMOV      	 */
	/* Floating-Point Compare */
		DECLARE_INSN(fcmps)                                                  	/* 0x1E202000	FCMP      	 */
		DECLARE_INSN(fcmps_zero)                                             	/* 0x1E202008	FCMP      	 */
		DECLARE_INSN(fcmpd)                                                  	/* 0x1E602000	FCMP      	 */
		DECLARE_INSN(fcmpd_zero)                                             	/* 0x1E602008	FCMP      	 */
	/* Floating-Point Data-processing (1 source) */
		DECLARE_INSN(fmovs)                                                  	/* 0x1E204000	FMOV      	 */
		DECLARE_INSN(fmovd)                                                  	/* 0x1E604000	FMOV      	 */
		DECLARE_INSN(fabss)                                                  	/* 0x1E20C000	FABS      	 */
		DECLARE_INSN(fabsd)                                                  	/* 0x1E60C000	FABS      	 */
		DECLARE_INSN(fnegs)                                                  	/* 0x1E214000	FNEG      	 */
		DECLARE_INSN(fnegd)                                                  	/* 0x1E614000	FNEG      	 */
	/* Floating-Point Data-processing (2 source) */
		DECLARE_INSN(fadds)                                                  	/* 0x1E202800	FADD      	 */
		DECLARE_INSN(faddd)                                                  	/* 0x1E602800	FADD      	 */
		DECLARE_INSN(fsubs)                                                  	/* 0x1E203800	FSUB      	 */
		DECLARE_INSN(fsubd)                                                  	/* 0x1E603800	FSUB      	 */
		DECLARE_INSN(fmuls)                                                  	/* 0x1E200800	FMUL      	 */
		DECLARE_INSN(fmuld)                                                  	/* 0x1E600800	FMUL      	 */
		DECLARE_INSN(fdivs)                                                  	/* 0x1E201800	FDIV      	 */
		DECLARE_INSN(fdivd)                                                  	/* 0x1E601800	FDIV      	 */
		DECLARE_INSN(fmaxs)                                                  	/* 0x1E204800	FMAX      	 */
		DECLARE_INSN(fmaxd)                                                  	/* 0x1E604800	FMAX      	 */
		DECLARE_INSN(fmins)                                                  	/* 0x1E205800	FMIN      	 */
		DECLARE_INSN(fmind)                                                  	/* 0x1E605800	FMIN      	 */
#undef DECLARE_INSN
/* Internal OpCodes */
		proc,  // Entry to the method
		fence, // Fence
		retn,  // Return
		dd,    // Define word
		label, // Destination of a jump
		ARM64LastOp = label,
		ARM64NumOpCodes = ARM64LastOp+1,
