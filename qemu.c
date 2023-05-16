#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <setjmp.h> // sigjmp_buf

//-----------------------------------------------------------------------------
// rips from QEMU
//-----------------------------------------------------------------------------
typedef uint64_t img_address;

typedef int (*fprintf_function)(FILE *f, const char *fmt, ...);

typedef struct Dis_info {
	img_address m_pc;
	fprintf_function fprintf_func;
	FILE *stream;
	sigjmp_buf buf;
} Dis_info;

bool nanomips_dis(const uint16_t *data, char **buf, Dis_info *info);

//-----------------------------------------------------------------------------
// main
//-----------------------------------------------------------------------------

int disassemble(uint64_t addr, uint8_t *data, int len, char *result)
{
	char *distxt;

	Dis_info dinfo;
	memset(&dinfo, 0, sizeof(dinfo));
	dinfo.m_pc = addr;
	dinfo.fprintf_func = fprintf;
	dinfo.stream = stdout;

	nanomips_dis((uint16_t *)data, &distxt, &dinfo);

	strcpy(result, distxt);

	return 0;
}

