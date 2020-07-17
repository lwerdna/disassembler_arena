#include <string.h>
#include <stdint.h>

/* libopcodes stuff */
#define PACKAGE "bfd"
#define PACKAGE_VERSION "2.30"
#include <bfd.h> // for bfd_arch_arm, etc.
#include <dis-asm.h>

int disasm_libopcodes(enum bfd_architecture arch, uint32_t mach, uint32_t addr, uint8_t *data, int len, char *result);

extern "C" int disassemble(uint32_t addr, uint8_t *data, int len, char *result)
{
	return disasm_libopcodes(bfd_arch_aarch64, bfd_mach_aarch64, addr, data, len, result);
}
