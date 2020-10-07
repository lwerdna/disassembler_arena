/* c++ includes */
#include <map>
#include <string>
#include <vector>
using namespace std;

#define ERR_RETURN(MSG) { printf("ERROR: " MSG "\n"); return -1; }

/* llvm includes */
#include <llvm-c/Target.h>
#include <llvm-c/Disassembler.h>

#include <llvm/MC/MCAsmBackend.h>
#include <llvm/MC/MCAsmInfo.h>
#include <llvm/MC/MCContext.h>
#include <llvm/MC/MCDisassembler/MCDisassembler.h>
#include <llvm/MC/MCCodeEmitter.h>
#include <llvm/MC/MCInstPrinter.h>
#include <llvm/MC/MCInstrInfo.h>
#include <llvm/MC/MCObjectFileInfo.h>
#include <llvm/MC/MCRegisterInfo.h>
#include <llvm/MC/MCSectionMachO.h>
#include <llvm/MC/MCSubtargetInfo.h>
#include <llvm/MC/MCParser/MCTargetAsmParser.h>
#include <llvm/MC/MCTargetOptionsCommandFlags.inc>
#include <llvm/MC/MCParser/AsmLexer.h>

#include <llvm/Support/CommandLine.h>
#include <llvm/Support/Compression.h>
#include <llvm/Support/FileUtilities.h>
#include <llvm/Support/FormattedStream.h>
#include <llvm/Support/Host.h> // for getDefaultTargetTriple();
#include <llvm/Support/ManagedStatic.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/PrettyStackTrace.h>
#include <llvm/Support/Signals.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/TargetRegistry.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/ToolOutputFile.h>

#include "utils.h"

/* a dummy function for the disasm context, else aarch64 crashes on
	FF 43 00 D1

	see llvm-c/Disassembler.h
*/
const char *
symbol_lookup_cb(void *DisInfo, uint64_t ReferenceValue, uint64_t *ReferenceType,
	uint64_t ReferencePC, const char **ReferenceName)
{
	const char *strRefType = "unknown";

	switch(*ReferenceType) {
		/* No input reference type or no output reference type. */
		case LLVMDisassembler_ReferenceType_InOut_None:
			strRefType = "none"; break;
		/* The input reference is from a branch instruction. */
		case LLVMDisassembler_ReferenceType_In_Branch:
			strRefType = "branch"; break;
		/* The input reference is from a PC relative load instruction. */
		case LLVMDisassembler_ReferenceType_In_PCrel_Load:
			strRefType = "pcref_load"; break;

		/* The input reference is from an ARM64::ADRP instruction. */
		case LLVMDisassembler_ReferenceType_In_ARM64_ADRP:
			strRefType = "arm64_adrp"; break;
		/* The input reference is from an ARM64::ADDXri instruction. */
		case LLVMDisassembler_ReferenceType_In_ARM64_ADDXri:
			strRefType = "arm64_addxri"; break;
		/* The input reference is from an ARM64::LDRXui instruction. */
		case LLVMDisassembler_ReferenceType_In_ARM64_LDRXui:
			strRefType = "arm64_ldrxui"; break;
		/* The input reference is from an ARM64::LDRXl instruction. */
		case LLVMDisassembler_ReferenceType_In_ARM64_LDRXl:
			strRefType = "arm64_ldrxl"; break;
		/* The input reference is from an ARM64::ADR instruction. */
		case LLVMDisassembler_ReferenceType_In_ARM64_ADR:
			strRefType = "arm64_adr"; break;
	}

	//printf("%s(refval:%llx reftype:%s)\n", __func__, ReferenceValue,
	//	strRefType);

	*ReferenceType = LLVMDisassembler_ReferenceType_InOut_None;
	return NULL;
}

/* this uses the LLVM C++ API */
extern "C" int disassemble(uint32_t addr, uint8_t *data, int len, char *result)
{
	int rc = -1;
	LLVMDisasmContextRef context;
	const char *triplet = "aarch64-none-elf";
	bool initialized = false;
	size_t instr_len;

	string TripleName, ArchName;
	const char *FeaturesStr;
	#if defined(AARCH64_ARMV8)
	TripleName = ArchName = "aarch64";
	FeaturesStr = "";
	#elif defined(AARCH64_ARMV8_1A)
	TripleName = ArchName = "aarch64";
	FeaturesStr = "+v8.1a";
	#elif defined(AARCH64_ARMV8_2A)
	TripleName = ArchName = "aarch64";
	FeaturesStr = "+v8.2a";
	#elif defined(AARCH64_ARMV8_3A)
	TripleName = ArchName = "aarch64";
	FeaturesStr = "+v8.3a";
	#elif defined(AARCH64_ARMV8_4A)
	TripleName = ArchName = "aarch64";
	FeaturesStr = "+v8.4a";
	#elif defined(AARCH64_ARMV8_5A)
	TripleName = ArchName = "aarch64";
	FeaturesStr = "+v8.5a";
	#elif defined(AARCH64_ARMV8_ALL)
	TripleName = ArchName = "aarch64";
	FeaturesStr =
		"+v8.5a,+bti,+ccdp,+crc,+crypto,+dotprod,+fp-armv8,+fp16fml,"
		"+fullfp16,+mte,+neon,+predres,+rand,+ras,+rcpc,+sb,"
		"+sha3,+sm4,+spe,+specrestrict,+ssbs,+tme";
	#endif

	if(!initialized) {
		LLVMInitializeAllTargetInfos();
		LLVMInitializeAllTargetMCs();
		//LLVMInitializeAllAsmParsers();
		LLVMInitializeAllDisassemblers();
		initialized = true;
	}

	/*
	printf("disassemble_options(0x%X, %02X %02X %02X %02X, %d, \"%s\")\n",
		addr, data[0], data[1], data[2], data[3], len, options);
	*/

	context = LLVMCreateDisasmCPUFeatures (
		triplet, /* triple */
		"", /* CPU */
		FeaturesStr, /* Features */
		NULL, /* void *DisInfo */
		0, /* TagType */
		NULL, /* LLVMOpInfoCallback GetOpInfo */
		symbol_lookup_cb /* LLVMSymbolLookupCallback SymbolLookUp */
	);

	if(context == NULL) {
		printf("ERROR: LLVMCreateDisasm(\"%s\", \"%s\", ...)\n", triplet, FeaturesStr);
		goto cleanup;
	}

	instr_len = LLVMDisasmInstruction(
		context, /* disasm context */
		data, /* source data */
		len, /* length of source data */
		addr, /* address */
		result, /* output buf */
		1024 /* size of output buf */
	);

	if(instr_len <= 0) {
		//printf("LLVMDisasmInstruction() returned %zu\n", instr_len);
		goto cleanup;
	}

	rc = 0;
	cleanup:
	if(context) {
		LLVMDisasmDispose(context);
		context = NULL;
	}
	return rc;
}

/* this uses just the LLVM C API */
extern "C" int disassemble_c(uint32_t addr, uint8_t *data, int len, char *result)
{
	int rc = -1;
	static LLVMDisasmContextRef context = NULL;

	const char *triplet;
	const char *features_str;

	/* do NOT seek arch features by changing the arch part of triplet (eg: aarch64 -> aarch64-v8)
		instead, use aarch64, and specify features */

	/* aarch64 features as of 10.0.1:
		bti, ccdp, crc, crypto, dotprod, fp16fml, fp-armv8, fullfp16,
		mte, neon, predres, rand, ras, rcpc, sb, sha3, sm4, spe,
		specrestrict, ssbs, tme, v8.1a, v8.2a, v8.3a, v8.4a, v8.5a

		llvm
		feature
		string   ARM      pcode              when
		-------  ---      -----              -----
		+ras     RAS      HaveRASExt()       ???
		+mte     MemTag   HaveMTEExt()       v8.5
		+bti     ???      HaveBTIExt()       ???
		+dotprod DotProd  HaveDOTPExt()      8.2
		+crypto  sha2     HaveSHA512Ext()    8.2
		+sm3     SM3      HaveSM3Ext()       8.2
		+sm4     SM4      HaveSM4Ext()       8.2
		???      I8MM     HaveInt8MatMulExt  8.2

		https://github.com/llvm-mirror/llvm/blob/master/tools/llvm-mc/llvm-mc.cpp
		https://github.com/llvm-mirror/llvm/tree/master/test/MC/Disassembler/AArch64
	*/
	#if defined(AARCH64_ARMV8)
	triplet = "aarch64";
	features_str = "";
	#elif defined(AARCH64_ARMV8_1A)
	triplet = "aarch64";
	features_str = "+v8.1a";
	#elif defined(AARCH64_ARMV8_2A)
	triplet = "aarch64";
	features_str = "+v8.2a";
	#elif defined(AARCH64_ARMV8_3A)
	triplet = "aarch64";
	features_str = "+v8.3a";
	#elif defined(AARCH64_ARMV8_4A)
	triplet = "aarch64";
	features_str = "+v8.4a";
	#elif defined(AARCH64_ARMV8_5A)
	triplet = "aarch64";
	features_str = "+v8.5a";
	#elif defined(AARCH64_ARMV8_ALL)
	triplet = "aarch64";
	features_str =
		"+v8.5a,+bti,+ccdp,+crc,+crypto,+dotprod,+fp-armv8,+fp16fml,"
		"+fullfp16,+mte,+neon,+predres,+rand,+ras,+rcpc,+sb,"
		"+sha3,+sm4,+spe,+specrestrict,+ssbs,+tme";
	#endif

	if(context == NULL) {
		LLVMInitializeAllTargetInfos();
		LLVMInitializeAllTargetMCs();
		LLVMInitializeAllAsmParsers();
		LLVMInitializeAllDisassemblers();

		//printf("features_str: %s\n", features_str);
		context = LLVMCreateDisasmCPUFeatures (
			triplet, /* triple */
			"", /* CPU */
			features_str, /* Features */
			NULL, /* void *DisInfo */
			0, /* TagType */
			NULL, /* LLVMOpInfoCallback GetOpInfo */
			symbol_lookup_cb /* LLVMSymbolLookupCallback SymbolLookUp */
		);

		if(context == NULL) {
			printf("ERROR: LLVMCreateDisasm(\"%s\", \"%s\", ...)\n", triplet, features_str);
			goto cleanup;
		}
	}

	/* llvm treats incoming data as little endian, and providing "aarch64_be" has no effect
		so, falsely swap it to force into our big-endian convention */
	//printf("before: %02X %02X %02X %02X\n", data[0], data[1], data[2], data[3]);
	//bswap32_mem(data, len);
	//printf("after: %02X %02X %02X %02X\n", data[0], data[1], data[2], data[3]);

	size_t instr_len;
	instr_len = LLVMDisasmInstruction(
		context, /* disasm context */
		data, /* source data */
		len, /* length of source data */
		addr, /* address */
		result, /* output buf */
		1024 /* size of output buf */
	);

	//bswap32_mem(data, len);

	if(instr_len <= 0) {
		//printf("LLVMDisasmInstruction() returned %zu\n", instr_len);
		goto cleanup;
	}

	rc = 0;
	cleanup:
	/* keep context for faster repeated calls */
	//if(context) {
	//	LLVMDisasmDispose(context);
	//	context = NULL;
	//}
	return rc;
}

