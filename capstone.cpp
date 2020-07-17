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

extern "C" int disassemble(uint32_t addr, uint8_t *data, int len, char *result)
{
	int rc = -1;

	csh handle = 0; /* capstone handle is typedef'd size_t */
	cs_insn *insn; /* detailed instruction information 
					cs_disasm() will allocate array of cs_insn here */
	size_t nInstr; /* number of instructions disassembled
					(number of cs_insn allocated) */
	
	/* initialize capstone handle */
	if(cs_open(arch, mode, &handle) != CS_ERR_OK) {
		printf("ERROR: cs_open()\n");
		goto cleanup;
	}

	nInstr = cs_disasm(handle, data, len, addr, 1, &insn);

	if(nInstr != 1) {
		//printf("ERROR: cs_disasm()\n");
		goto cleanup;
	}

	/* capstone struct -> string */
	strcpy(result, insn->mnemonic);
	if(insn->op_str[0]) {
		strcat(result, " ");
		strcat(result, insn->op_str);
	}

	cs_free(insn, nInstr);
		
	rc = 0;
	cleanup:
	if(handle)
		cs_close(&handle);
	return rc;
}

