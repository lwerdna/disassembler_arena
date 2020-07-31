#include <stdint.h>

uint32_t bswap32(uint32_t x)
{
	uint8_t a = (x>>24)&0xFF;
	uint8_t b = (x>>16)&0xFF;
	uint8_t c = (x>>8)&0xFF;
	uint8_t d = x&0xFF;
	return (d<<24)|(c<<16)|(b<<8)|a;
}

void bswap32_mem(uint8_t *p, int len)
{
	for(int i=0; i<len; i+=4)
		*(uint32_t *)(p+i) = bswap32(*(uint32_t *)(p+i));
}
