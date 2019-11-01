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

/*****************************************************************************/
/* disassembler string making callback */
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
	uint32_t addr,
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

	if(am2di.find(amp) == am2di.end()) {
		/* create disassemble info */
		struct disassemble_info dinfo = {0};
		init_disassemble_info(&dinfo, NULL, cb_fprintf);
		dinfo.flavour = bfd_target_unknown_flavour;
		dinfo.arch = arch;
		dinfo.mach = mach;
		dinfo.endian = BFD_ENDIAN_BIG;
		disassemble_init_for_target(&dinfo); // reads dinfo.arch and populate extra stuff
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

	/* use the stream pointer as our private data
		(the buffer that fprintf() should append to) */
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

	result[0] = '\0';
	disasm((bfd_vma)addr, &dinfo);

	return 0;
}
