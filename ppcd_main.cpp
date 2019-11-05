#include <string.h>
#include <stdint.h>

#include "ppcd.h"

static uint32_t bswap32(uint32_t data)
{
    return ((data >> 24)             ) |
           ((data >>  8) & 0x0000ff00) |
           ((data <<  8) & 0x00ff0000) |
           ((data << 24)             );
}

extern "C" int disassemble(uint32_t addr, uint8_t *data, int len, char *result)
{
	PPCD_CB disa;

	disa.pc = addr;
	disa.instr = bswap32(*(uint32_t *)data);
	PPCDisasm(&disa);



	strcpy(result, disa.mnemonic);
	if(disa.operands[0]) {
		strcat(result, " ");
		strcat(result, disa.operands);
	}

	if(disa.iclass == PPC_DISA_ILLEGAL)
		return 0;
	else
		return 4;
}
