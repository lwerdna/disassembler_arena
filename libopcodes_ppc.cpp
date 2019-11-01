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
	int machine;

	#if defined(PPC)
	machine = bfd_mach_ppc;
	#elif defined(PPC64)
	machine = bfd_mach_ppc64;
	#elif defined(PPC_403)
	machine = bfd_mach_ppc_403;
	#elif defined(PPC_403GC)
	machine = bfd_mach_ppc_403gc;
	#elif defined(PPC_405)
	machine = bfd_mach_ppc_405;
	#elif defined(PPC_505)
	machine = bfd_mach_ppc_505;
	#elif defined(PPC_601)
	machine = bfd_mach_ppc_601;
	#elif defined(PPC_602)
	machine = bfd_mach_ppc_602;
	#elif defined(PPC_603)
	machine = bfd_mach_ppc_603;
	#elif defined(PPC_EC603E)
	machine = bfd_mach_ppc_ec603e;
	#elif defined(PPC_604)
	machine = bfd_mach_ppc_604;
	#elif defined(PPC_620)
	machine = bfd_mach_ppc_620;
	#elif defined(PPC_630)
	machine = bfd_mach_ppc_630;
	#elif defined(PPC_750)
	machine = bfd_mach_ppc_750;
	#elif defined(PPC_860)
	machine = bfd_mach_ppc_860;
	#elif defined(PPC_A35)
	machine = bfd_mach_ppc_a35;
	#elif defined(PPC_RS64II)
	machine = bfd_mach_ppc_rs64ii;
	#elif defined(PPC_RS64III)
	machine = bfd_mach_ppc_rs64iii;
	#elif defined(PPC_7400)
	machine = bfd_mach_ppc_7400;
	#elif defined(PPC_E500)
	machine = bfd_mach_ppc_e500;
	#elif defined(PPC_E500MC)
	machine = bfd_mach_ppc_e500mc;
	#elif defined(PPC_E500MC64)
	machine = bfd_mach_ppc_e500mc64;
	#elif defined(PPC_E5500)
	machine = bfd_mach_ppc_e5500;
	#elif defined(PPC_E6500)
	machine = bfd_mach_ppc_e6500;
	#elif defined(PPC_TITAN)
	machine = bfd_mach_ppc_titan;
	#elif defined(PPC_VLE)
	machine = bfd_mach_ppc_vle;
	#endif

	return disasm_libopcodes(bfd_arch_powerpc, machine, addr, data, len, result);
}
