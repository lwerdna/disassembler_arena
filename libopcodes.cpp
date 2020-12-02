/* generic disassembler using libopcodes that caches architecture/machine
	pairs */

/* */
#include <stdint.h>
#include <string.h>

/* c++ stuff */
#include <map>
using namespace std;

/* libopcodes stuff */
#define PACKAGE "bfd"
#define PACKAGE_VERSION "2.30"
#include <bfd.h> // for bfd_arch_arm, etc.
#include <dis-asm.h>

/* local stuff */
#include "utils.h"

/*****************************************************************************/
/* disassembler callbacks */
/*****************************************************************************/
int cb_fprintf(void *stream, const char *fmt, ...)
{
	va_list args;
    va_start(args, fmt);
    char *built_str = (char *)stream;
	char buf[1024];
    int rc = vsnprintf(buf, sizeof(buf)-1, fmt, args);
    va_end(args);
    strcat((char *)built_str, buf);
    return rc;
};

/* overrides struct disassemble_info .read_memory_func,
	by default it's buffer_read_memory */
int read_memory_dword_swap(bfd_vma memaddr, bfd_byte *myaddr, unsigned int length,
	struct disassemble_info *dinfo)
{
	//printf("custom_read_memory(addr=0x%lX, length=%d)\n", memaddr, length);
	//printf("source_bytes: %02X %02X %02X %02X\n", source_bytes[0], source_bytes[1], source_bytes[2], source_bytes[3]);

	/* we stuffed this here during disassembler setup */
	uint8_t *source_bytes = (uint8_t *)dinfo->private_data;

	memcpy(myaddr, source_bytes, length);
	bswap32_mem(myaddr, length);

	return 0;
}

/*****************************************************************************/
/* architecture/machine pairs to track libopcodes disassemblers */
/*****************************************************************************/
struct architecture_machine_pair
{
	enum bfd_architecture architecture;
	uint32_t machine;
};

bool operator<(const struct architecture_machine_pair& lhs, const struct architecture_machine_pair& rhs)
{
	if(lhs.architecture < rhs.architecture)
		return true;
	if(lhs.machine < rhs.machine)
		return true;
	return false;
}

map<struct architecture_machine_pair, struct disassemble_info> am2di;
map<struct architecture_machine_pair, disassembler_ftype> am2df;

/*****************************************************************************/
/* actual disassemble call */
/*****************************************************************************/
int disasm_libopcodes(
	enum bfd_architecture arch,
	uint32_t mach,
	uint64_t addr,
	uint8_t *data,
	int len,
	char *result
)
{
	struct architecture_machine_pair amp = { .architecture=arch, .machine=mach };

	//printf("bfd_arch_powerpc is: %d\n", bfd_arch_powerpc); // 25
	//printf("using arch: %d\n", arch);
	//printf("using mach: %d\n", mach);
	//printf("using addr: %d\n", addr);
	//printf("using  len: %d\n", len);
	//printf("   insword: 0x%08X\n", insword);
	//printf("      data: %02X %02X %02X %02X...\n", data[0], data[1], data[2], data[3]);

	if(am2di.find(amp) == am2di.end()) {
		/* create disassemble info, see <binutils>/include/dis-asm.h */
		struct disassemble_info dinfo = {0};

		/* defaults, see <binutils>/opcodes/dis-init.c */
		init_disassemble_info(&dinfo, NULL, cb_fprintf);
		dinfo.arch = arch;
		dinfo.mach = mach;
		dinfo.endian = BFD_ENDIAN_BIG;
		dinfo.endian_code = BFD_ENDIAN_BIG;
		dinfo.display_endian = BFD_ENDIAN_BIG;

		/* read dinfo.arch and populate further defaults, see <binutils>/opcodes/disassemble.c */
		disassemble_init_for_target(&dinfo);
		/* WORKAROUND: aarch64 is always little endian to libopcodes,
			see <binutils>/opcodes/aarch64-dis.c for hardcode set to BFD_ENDIAN_LITTLE
			need custom memory reader to get big endian from aarch64 */
		/*
		if(dinfo.arch==bfd_arch_aarch64 && dinfo.endian==BFD_ENDIAN_BIG) {
			dinfo.private_data = data;
			dinfo.read_memory_func = read_memory_dword_swap;
		}
		*/

		/* store in lookup */
		am2di[amp] = dinfo;

		/* create disassembler */
		disassembler_ftype disasm;
		disasm = disassembler(arch, TRUE, mach, NULL);
		if(!disasm) {
			printf("ERROR: disassembler() returned no function\n");
			return -1;
		}
		am2df[amp] = disasm;
	}

	disassemble_info dinfo = am2di[amp];
	disassembler_ftype disasm = am2df[amp];

	/* use the stream pointer to store buffer that fprintf() should append to */
	dinfo.stream = (void *)result;

	/* call disassembler
		will use callbacks in dinfo (like .read_memory_func, .print_addr_func, etc.)
		and the defaults are fine for this use case, see the defaults in a debugger
		or look at especially buffer_read_memory() in dis-buf.c for details */
	dinfo.octets_per_byte = 1;
	dinfo.buffer_vma = addr;
	dinfo.stop_vma = addr + len;

	/* source data */
	dinfo.buffer = data;
	dinfo.buffer_length = len;

	/* also store source data in .private_data in case custom .read_memory_func
		needs to access it */
	dinfo.private_data = data;

	result[0] = '\0';
	disasm((bfd_vma)addr, &dinfo);

	return 0;
}

extern "C" int disassemble(uint64_t addr, uint8_t *data, int len, char *result)
{
	enum bfd_architecture arch;
	int machine;

	#if defined(X86)
	arch = bfd_arch_i386;
	machine = bfd_mach_i386_i386_intel_syntax;
	#elif defined(X64)
	arch = bfd_arch_i386;
	machine = bfd_mach_x86_64_intel_syntax;


	#elif defined(ARM_UNKNOWN)
	arch = bfd_arch_arm;
	machine = bfd_mach_arm_unknown;
	#elif defined(ARM_2)
	arch = bfd_arch_arm;
	machine = bfd_mach_arm_2;
	#elif defined(ARM_2a)
	arch = bfd_arch_arm;
	machine = bfd_mach_arm_2a;
	#elif defined(ARM_3)
	arch = bfd_arch_arm;
	machine = bfd_mach_arm_3;
	#elif defined(ARM_3M)
	arch = bfd_arch_arm;
	machine = bfd_mach_arm_3M;
	#elif defined(ARM_4)
	arch = bfd_arch_arm;
	machine = bfd_mach_arm_4;
	#elif defined(ARM_4T)
	arch = bfd_arch_arm;
	machine = bfd_mach_arm_4T;
	#elif defined(ARM_5)
	arch = bfd_arch_arm;
	machine = bfd_mach_arm_5;
	#elif defined(ARM_5T)
	arch = bfd_arch_arm;
	machine = bfd_mach_arm_5T;
	#elif defined(ARM_5TE)
	arch = bfd_arch_arm;
	machine = bfd_mach_arm_5TE;
	#elif defined(ARM_XSCALE)
	arch = bfd_arch_arm;
	machine = bfd_mach_arm_XScale;
	#elif defined(ARM_EP9312)
	arch = bfd_arch_arm;
	machine = bfd_mach_arm_ep9312;
	#elif defined(ARM_IWMMXT)
	arch = bfd_arch_arm;
	machine = bfd_mach_arm_iWMMXt;
	#elif defined(ARM_IWMMXT2)
	arch = bfd_arch_arm;
	machine = bfd_mach_arm_iWMMXt2;


	#elif defined(AARCH64)
	arch = bfd_arch_aarch64;
	machine = bfd_mach_aarch64;
	#elif defined(AARCH64_ILP32)
	arch = bfd_arch_aarch64;
	machine = bfd_mach_aarch64_ilp32;


	#elif defined(PPC)
	arch = bfd_arch_powerpc;
	machine = bfd_mach_ppc;
	#elif defined(PPC64)
	arch = bfd_arch_powerpc;
	machine = bfd_mach_ppc64;
	#elif defined(PPC_403)
	arch = bfd_arch_powerpc;
	machine = bfd_mach_ppc_403;
	#elif defined(PPC_403GC)
	arch = bfd_arch_powerpc;
	machine = bfd_mach_ppc_403gc;
	#elif defined(PPC_405)
	arch = bfd_arch_powerpc;
	machine = bfd_mach_ppc_405;
	#elif defined(PPC_505)
	arch = bfd_arch_powerpc;
	machine = bfd_mach_ppc_505;
	#elif defined(PPC_601)
	arch = bfd_arch_powerpc;
	machine = bfd_mach_ppc_601;
	#elif defined(PPC_602)
	arch = bfd_arch_powerpc;
	machine = bfd_mach_ppc_602;
	#elif defined(PPC_603)
	arch = bfd_arch_powerpc;
	machine = bfd_mach_ppc_603;
	#elif defined(PPC_EC603E)
	arch = bfd_arch_powerpc;
	machine = bfd_mach_ppc_ec603e;
	#elif defined(PPC_604)
	arch = bfd_arch_powerpc;
	machine = bfd_mach_ppc_604;
	#elif defined(PPC_620)
	arch = bfd_arch_powerpc;
	machine = bfd_mach_ppc_620;
	#elif defined(PPC_630)
	arch = bfd_arch_powerpc;
	machine = bfd_mach_ppc_630;
	#elif defined(PPC_750)
	arch = bfd_arch_powerpc;
	machine = bfd_mach_ppc_750;
	#elif defined(PPC_860)
	arch = bfd_arch_powerpc;
	machine = bfd_mach_ppc_860;
	#elif defined(PPC_A35)
	arch = bfd_arch_powerpc;
	machine = bfd_mach_ppc_a35;
	#elif defined(PPC_RS64II)
	arch = bfd_arch_powerpc;
	machine = bfd_mach_ppc_rs64ii;
	#elif defined(PPC_RS64III)
	arch = bfd_arch_powerpc;
	machine = bfd_mach_ppc_rs64iii;
	#elif defined(PPC_7400)
	arch = bfd_arch_powerpc;
	machine = bfd_mach_ppc_7400;
	#elif defined(PPC_E500)
	arch = bfd_arch_powerpc;
	machine = bfd_mach_ppc_e500;
	#elif defined(PPC_E500MC)
	arch = bfd_arch_powerpc;
	machine = bfd_mach_ppc_e500mc;
	#elif defined(PPC_E500MC64)
	arch = bfd_arch_powerpc;
	machine = bfd_mach_ppc_e500mc64;
	#elif defined(PPC_E5500)
	arch = bfd_arch_powerpc;
	machine = bfd_mach_ppc_e5500;
	#elif defined(PPC_E6500)
	arch = bfd_arch_powerpc;
	machine = bfd_mach_ppc_e6500;
	#elif defined(PPC_TITAN)
	arch = bfd_arch_powerpc;
	machine = bfd_mach_ppc_titan;
	#elif defined(PPC_VLE)
	arch = bfd_arch_powerpc;
	machine = bfd_mach_ppc_vle;
	#endif

	return disasm_libopcodes(arch, machine, addr, data, len, result);
}
