/* generic disassembler using capstone */
#include <stdint.h>
#include <string.h>

/* capstone stuff /usr/local/include/capstone */
#include <capstone/capstone.h>
#include <capstone/ppc.h>

#ifdef AARCH64
cs_mode mode = (cs_mode)CS_MODE_LITTLE_ENDIAN; 
cs_arch arch = (cs_arch)CS_ARCH_ARM64;
#endif

#ifdef POWERPC_64
cs_mode mode = (cs_mode)(CS_MODE_64|CS_MODE_BIG_ENDIAN); 
cs_arch arch = (cs_arch)CS_ARCH_PPC;
#endif

extern "C" int disassemble(uint32_t addr_, uint8_t *data, int len, char *result)
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
