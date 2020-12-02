/* generic disassembler using capstone */
#include <stdint.h>
#include <string.h>

/* capstone stuff /usr/local/include/capstone */
#include <capstone/capstone.h>
#include <capstone/ppc.h>

/* see test_mc.py from the capstone suite */

#ifdef ARM
cs_arch arch = (cs_arch)CS_ARCH_ARM;
cs_mode mode = (cs_mode)(CS_MODE_ARM);
#endif

	#ifdef ARM_V8
	cs_arch arch = (cs_arch)CS_ARCH_ARM;
	cs_mode mode = (cs_mode)(CS_MODE_V8);
	#endif

#ifdef ARM_THUMB
cs_arch arch = (cs_arch)CS_ARCH_ARM;
cs_mode mode = (cs_mode)(CS_MODE_THUMB);
#endif

	#ifdef ARM_THUMB_MCLASS
	cs_arch arch = (cs_arch)CS_ARCH_ARM;
	cs_mode mode = (cs_mode)(CS_MODE_THUMB | CS_MODE_MCLASS);
	#endif

		#ifdef ARM_THUMB_MCLASS_V8
		cs_arch arch = (cs_arch)CS_ARCH_ARM;
		cs_mode mode = (cs_mode)(CS_MODE_THUMB | CS_MODE_MCLASS | CS_MODE_V8);
		#endif

#ifdef AARCH64
cs_arch arch = (cs_arch)CS_ARCH_ARM64;
cs_mode mode = (cs_mode)(CS_MODE_LITTLE_ENDIAN);
#endif

#ifdef POWERPC_64
cs_mode mode = (cs_mode)(CS_MODE_64|CS_MODE_BIG_ENDIAN);
cs_arch arch = (cs_arch)CS_ARCH_PPC;
#endif

#ifdef X86
cs_mode mode = (cs_mode)(CS_MODE_32);
cs_arch arch = (cs_arch)CS_ARCH_X86;
#endif

#ifdef X64
cs_mode mode = (cs_mode)(CS_MODE_64);
cs_arch arch = (cs_arch)CS_ARCH_X86;
#endif

extern "C" int disassemble(uint64_t addr_, uint8_t *data, int len, char *result)
{
	int rc = -1;

	/* one-time capstone init stuff */
	static bool init = false;
	static csh handle = 0;
	static cs_insn *insn = NULL;
	if (!init) {
		if(cs_open(arch, mode, &handle) != CS_ERR_OK) {
			printf("ERROR: cs_open()\n");
			exit(-1);
		}
		insn = cs_malloc(handle);
		init = true;
	}

	/* actually disassemble */
	uint64_t addr = addr_;
	size_t size = 4;
	const uint8_t *pinsword = data;

	size_t count = cs_disasm_iter(handle, &pinsword, &size, &addr, insn);
	if(count != 1) {
		if(cs_errno(handle) == CS_ERR_OK) {
			if(result)
				strcpy(result, "undef");
		}
		else {
			//printf("ERROR: cs_disasm_iter()\n");
			if(result)
				strcpy(result, "error");
			while(0);
		}
	}
	else {
		if(result) {
			strcpy(result, insn->mnemonic);
			if(insn->op_str[0]) {
				strcat(result, " ");
				strcat(result, insn->op_str);
			}
		}

		rc = 0;
	}

	return rc;
}

