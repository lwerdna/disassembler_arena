// given a shared object, address, and bytes
// resolve and call that object's disassemble function
//
// EXAMPLE:
//  $ ./call_so llvm_aarch64_all.so DEADBEEF 1f 20 03 d5
//  nop
//
// COMPILE:
//   gcc call_so.c -o call_so

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <dlfcn.h>

typedef int (* PFUNC_DISASSEMBLE)(uint64_t, uint8_t *, int, char *);

int main(int ac, char **av)
{
	int rc = -1;
	char distxt[128] = {'\0'};
	unsigned char data[4] = {0xc0, 0x62, 0xb9, 0xf3};
	//unsigned char data[4] = {0xf3, 0xb9, 0x62, 0xc0};


	char *pathso = av[1];
	//printf("loading %s\n", pathso);

	void *handle = dlopen(pathso, RTLD_LAZY);
	if(!handle) {
		printf("ERROR: dlopen()");
		return -1;
	}

	//printf("resolving disassemble()\n");
	PFUNC_DISASSEMBLE disassemble = dlsym(handle, "disassemble");
	if(!disassemble) {
		printf("ERROR: dlsym()");
		return -1;
	}

	uint64_t addr = strtol(av[2], 0, 16);
	int len = ac-3;
	for(int i=0; i<len; ++i) {
		data[i] = strtol(av[i+3], 0, 16) & 0xFF;
	}

	if(0) {
		printf("%s sending address 0x%llX: ", __FILE__, addr);
		for(int i=0; i<len; ++i)
			printf("%02X ", data[i]);
		printf("\n");
	}

	rc = disassemble(addr, data, len, distxt);
	if(rc >= 0)
		printf("%s", distxt);
	else
		printf("ERROR: %d", rc);

	return rc;
}
