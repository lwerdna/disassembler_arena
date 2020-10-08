#include <stdint.h>

#include "binaryninjacore.h"
#include "binaryninjaapi.h"

using namespace BinaryNinja;

/******************************************************************************
 MAIN
******************************************************************************/

#if defined(X86)
const char *archmode = "x86";
#elif defined(X64)
const char *archmode = "x86_64";
#elif defined(ARMV7)
const char *archmode = "armv7eb";
#elif defined(THUMB2)
const char *archmode = "thumb2eb";
#elif defined(PPC)
const char *archmode = "ppc";
#elif defined(AARCH64)
const char *archmode = "aarch64";
#elif defined(MIPS32)
const char *archmode = "mips32";
#endif

extern "C" int disassemble(uint32_t addr_, uint8_t *data, int len, char *result)
{
	int rc = -1;

	static bool init = false;
	static BNArchitecture *arch;
	if(!init) {
		BNSetBundledPluginDirectory(PATH_BUNDLED_PLUGINS);
		BNInitCorePlugins();
		arch = BNGetArchitectureByName(archmode);
		if(!arch) return -1;
		init = true;
	}

	/* actually disassemble now */
	BNInstructionTextToken *ttResult = NULL;
	size_t ttCount, nBytesDisasm=len;
	BNGetInstructionText(arch, (const uint8_t *)data, 0, &nBytesDisasm,
	  &ttResult, &ttCount);

	if(ttCount <= 0)
		return -1;

	strcpy(result, "");
	for(int i=0; i<ttCount; ++i)
		strcat(result, ttResult[i].text);

	if(ttResult)
		BNFreeInstructionText(ttResult, ttCount);

	return 0;
}


