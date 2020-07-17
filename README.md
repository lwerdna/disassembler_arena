Compile a collection of shared objects, one for each 2-tuple (architecture, disassembler).

Each exports a simple function:

```
extern "C" int disassemble(uint32_t addr, uint8_t *data, int len, char *result)
```

Which is easily callable with python. Actually, python all.py is provided as an example. It looks in the current directory for all shared objects and call their disassemble() function with the supplied bytes:


```
$ ./all.py 90 90 90 90
90 90 90 90
capstone_aarch64.so              90 90 90 90: adrp x16, #0xffffffff21210000
capstone_ppc64.so                90 90 90 90: stw r4, -0x6f70(r16)
libopcodes_aarch64.so            90 90 90 90: adrp	x16, 0xffffffff21210000
libopcodes_ppc.so                90 90 90 90: stw     r4,-28528(r16)
libopcodes_ppc64.so              90 90 90 90: stw     r4,-28528(r16)
libopcodes_ppc_403.so            90 90 90 90: stw     r4,-28528(r16)
libopcodes_ppc_403gc.so          90 90 90 90: stw     r4,-28528(r16)
libopcodes_ppc_405.so            90 90 90 90: stw     r4,-28528(r16)
libopcodes_ppc_505.so            90 90 90 90: stw     r4,-28528(r16)
libopcodes_ppc_601.so            90 90 90 90: stw     r4,-28528(r16)
libopcodes_ppc_602.so            90 90 90 90: stw     r4,-28528(r16)
libopcodes_ppc_603.so            90 90 90 90: stw     r4,-28528(r16)
libopcodes_ppc_604.so            90 90 90 90: stw     r4,-28528(r16)
libopcodes_ppc_620.so            90 90 90 90: stw     r4,-28528(r16)
libopcodes_ppc_630.so            90 90 90 90: stw     r4,-28528(r16)
libopcodes_ppc_7400.so           90 90 90 90: stw     r4,-28528(r16)
libopcodes_ppc_750.so            90 90 90 90: stw     r4,-28528(r16)
libopcodes_ppc_860.so            90 90 90 90: stw     r4,-28528(r16)
libopcodes_ppc_a35.so            90 90 90 90: st      r4,-28528(r16)
libopcodes_ppc_e500.so           90 90 90 90: stw     r4,-28528(r16)
libopcodes_ppc_e500mc.so         90 90 90 90: stw     r4,-28528(r16)
libopcodes_ppc_e500mc64.so       90 90 90 90: stw     r4,-28528(r16)
libopcodes_ppc_e5500.so          90 90 90 90: stw     r4,-28528(r16)
libopcodes_ppc_e6500.so          90 90 90 90: stw     r4,-28528(r16)
libopcodes_ppc_ec603e.so         90 90 90 90: stw     r4,-28528(r16)
libopcodes_ppc_rs64ii.so         90 90 90 90: st      r4,-28528(r16)
libopcodes_ppc_rs64iii.so        90 90 90 90: st      r4,-28528(r16)
libopcodes_ppc_titan.so          90 90 90 90: stw     r4,-28528(r16)
libopcodes_ppc_vle.so            90 90 90 90: stw     r4,-28528(r16)
ppcd_broadway.so                 90 90 90 90: stw r4, -0x6F70 (r16)
ppcd_gecko.so                    90 90 90 90: stw r4, -0x6F70 (r16)
ppcd_powerpc_32.so               90 90 90 90: stw r4, -0x6F70 (r16)
ppcd_powerpc_64.so               90 90 90 90: stw r4, -0x6F70 (r16)
```
