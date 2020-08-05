/* LLVM services: disassembler/assemble

	anything starting with "llvm_svcs" is a public API
	anything else is internal use
*/

/* c++ includes */
#include <map>
#include <string>
#include <vector>
using namespace std;

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

/*****************************************************************************/
/* MISCELLANY */
/*****************************************************************************/

void
llvm_svcs_init(void)
{
	llvm::InitializeAllTargetInfos();
	llvm::InitializeAllTargetMCs();
	llvm::InitializeAllAsmParsers();
	llvm::InitializeAllDisassemblers();
}

/* references:
	http://clang.llvm.org/docs/CrossCompilation.html
	<llvm_source>/include/llvm/ADT/Triple.h
	https://wiki.debian.org/Multiarch/Tuples
	https://www.linux-mips.org/wiki/MIPS_ABI_History
*/
void
llvm_svcs_triplet_decompose(const char *triplet, string &arch, string &subarch,
	string &vendor, string &os, string &environ, string &objFormat)
{
	//spec = llvm::sys::getDefaultTargetTriple();
	//std::string machSpec = "x86_64-apple-darwin14.5.0";
	//std::string machSpec = "x86_64-apple-darwin";
	//std::string machSpec = "x86_64-thumb-linux-gnu";
	//std::string machSpec = "x86_64-unknown-linux-gnu";
	Triple trip(triplet);

	/* FIRST component is <arch><subarch>
		eg: x86,arm,thumb,mips,... */
	arch = trip.getArchName();

	/* sub architectures, see llvm/ADT/Triple.h
		eg: v5,v6m,v7a,v7m,... */
	switch(trip.getSubArch()) {
		case llvm::Triple::NoSubArch: subarch=""; break;
   		case llvm::Triple::ARMSubArch_v8_5a: subarch="v8_5a"; break;
   		case llvm::Triple::ARMSubArch_v8_4a: subarch="v8_4a"; break;
   		case llvm::Triple::ARMSubArch_v8_3a: subarch="v8_3a"; break;
   		case llvm::Triple::ARMSubArch_v8_2a: subarch="v8_2a"; break;
		case llvm::Triple::ARMSubArch_v8_1a: subarch="v8_1a"; break;
		case llvm::Triple::ARMSubArch_v8_1m_mainline: subarch="v8_1m_mainline"; break;
		case llvm::Triple::ARMSubArch_v8: subarch="v8"; break;
		case llvm::Triple::ARMSubArch_v8r: subarch="v8r"; break;
		case llvm::Triple::ARMSubArch_v8m_baseline: subarch="v8m_baseline"; break;
		case llvm::Triple::ARMSubArch_v8m_mainline: subarch="v8m_mainline"; break;
		case llvm::Triple::ARMSubArch_v7: subarch="v7"; break;
		case llvm::Triple::ARMSubArch_v7em: subarch="v7em"; break;
		case llvm::Triple::ARMSubArch_v7m: subarch="v7m"; break;
		case llvm::Triple::ARMSubArch_v7s: subarch="v7s"; break;
		case llvm::Triple::ARMSubArch_v7ve: subarch="v7ve"; break;
		case llvm::Triple::ARMSubArch_v7k: subarch="v7k"; break;
		case llvm::Triple::ARMSubArch_v6: subarch="v6"; break;
		case llvm::Triple::ARMSubArch_v6m: subarch="v6m"; break;
		case llvm::Triple::ARMSubArch_v6k: subarch="v6k"; break;
		case llvm::Triple::ARMSubArch_v6t2: subarch="v6t2"; break;
		case llvm::Triple::ARMSubArch_v5: subarch="v5"; break;
		case llvm::Triple::ARMSubArch_v5te: subarch="v5te"; break;
		case llvm::Triple::ARMSubArch_v4t: subarch="v4t"; break;
		case llvm::Triple::KalimbaSubArch_v3: subarch="v3"; break;
		case llvm::Triple::KalimbaSubArch_v4: subarch="v4"; break;
		case llvm::Triple::KalimbaSubArch_v5: subarch="v5"; break;
		case llvm::Triple::MipsSubArch_r6: subarch="r6"; break;
		case llvm::Triple::PPCSubArch_spe: subarch="spe"; break;
	}

	/* SECOND component is <vendor>
		eg: pc,apple,nvidia,ibm,... */
	vendor = trip.getVendorName();

	/* THIRD component is <sys> or <os>
		eg: none,linux,win32,darwin,cuda,... */
	os = trip.getOSName();

	/* FOURTH component is <environment> or <abi>
		eg: eabi,gnu,android.macho,elf,... */
	environ = trip.getEnvironmentName();

	/* this is not part of the triplet, just trivia about what object format
		will be used to contain the code */
	Triple::ObjectFormatType oft = trip.getObjectFormat();

	switch(oft) {
		case Triple::Wasm: objFormat = "wasm"; break;
		case Triple::XCOFF: objFormat = "xcoff"; break;
		case Triple::COFF: objFormat = "coff"; break;
		case Triple::ELF: objFormat = "elf"; break;
		case Triple::MachO: objFormat = "MachO"; break;
		case Triple::UnknownObjectFormat: objFormat = "unknown"; break;
	}
}

/*****************************************************************************/
/* DISASSEMBLE related functions */
/*****************************************************************************/

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

extern "C" int test_triplet(char *triplet)
{
	int rc = -1;
	static LLVMDisasmContextRef context = NULL;

	static bool initialized = false;
	if(!initialized) {
		LLVMInitializeAllTargetInfos();
		LLVMInitializeAllTargetMCs();
		LLVMInitializeAllAsmParsers();
		LLVMInitializeAllDisassemblers();
		initialized = true;
	}

	//printf("testing triplet: %s\n", triplet);

	context = LLVMCreateDisasm(triplet, NULL, 0, NULL, symbol_lookup_cb);
	if(context) {
		LLVMDisasmDispose(context);
		return 0;
	}

	return -1;
}

extern "C" int disassemble(uint32_t addr, uint8_t *data, int len, char *result)
{
	int rc = -1;
	static LLVMDisasmContextRef context = NULL;

	const char *triplet;
	const char *features_str;

	/* do NOT seek arch features by changing the arch part of triplet (eg: aarch64 -> aarch64-v8)
		instead, use aarch64, and specify features */

	/* aarch64 features as of 10.0.1:
		bti, ccdp, crc, crypto, dotprod, fp16fml, fparmv8, fullfp16,
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
	triplet = "aarch64-none-elf";
	features_str = "";
	#elif defined(AARCH64_ARMV8_1A)
	triplet = "aarch64-none-elf";
	features_str = "+v8.1a";
	#elif defined(AARCH64_ARMV8_2A)
	triplet = "aarch64-none-elf";
	features_str = "+v8.2a";
	#elif defined(AARCH64_ARMV8_3A)
	triplet = "aarch64-none-elf";
	features_str = "+v8.3a";
	#elif defined(AARCH64_ARMV8_4A)
	triplet = "aarch64-none-elf";
	features_str = "+v8.4a";
	#elif defined(AARCH64_ARMV8_5A)
	triplet = "aarch64-none-elf";
	features_str = "+v8.5a";
	#elif defined(AARCH64_ARMV8_ALL)
	triplet = "aarch64-none-elf";
	features_str =
		"+v8.5a,+bti,+ccdp,+crc,+crypto,+dotprod,+fp16fml,"
		"+fullfp16,+mte,+neon,+predres,+rand,+ras,+rcpc,+sb,"
		"+sha3,+sm4,+spe,+specrestrict,+ssbs,+tme";
	#endif

	if(context == NULL) {
		LLVMInitializeAllTargetInfos();
		LLVMInitializeAllTargetMCs();
		LLVMInitializeAllAsmParsers();
		LLVMInitializeAllDisassemblers();

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
	bswap32_mem(data, len);
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

	bswap32_mem(data, len);

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

