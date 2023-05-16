This project aims to compare the output of many disassemblers.

It compiles disassemblers into shared objects that you can call easily. The provided Makefile has targets like `intel`, `arm`, `thumb`, `aarch64`, `ppc`, `mips`, `sh4`:

```
make aarch64
```

Each shared object exports a simple function:

```
extern "C" int disassemble(uint32_t addr, uint8_t *data, int len, char *result)
```

Which is easy to resolve dynamically. See `./call_so.c` and `./call_so.py` for examples.

The utility `all.py` will scan the current directory for shared objects and use `call_so` to invoke their `disassemble()` functions with the supplied address and bytes:


```
$ ./all.py 0 aa bb cc dd
capstone_aarch64.so              AABBCCDD: orn x29, x6, x27, asr #51     DDCCBBAA: undef
capstone_ppc64.so                AABBCCDD: lha r21, -0x3323(r27)         DDCCBBAA: stfdu f14, -0x4456(r12)
capstone_x64.so                  AABBCCDD: stosb byte ptr [rdi], al      DDCCBBAA: fxch st(0), st(4)
capstone_x86.so                  AABBCCDD: stosb byte ptr es:[edi], al   DDCCBBAA: fxch st(0), st(4)
libopcodes_aarch64.so            AABBCCDD: orn x29, x6, x27, asr #51     DDCCBBAA: .inst 0xddccbbaa ; undefined
libopcodes_aarch64_ilp32.so      AABBCCDD: orn x29, x6, x27, asr #51     DDCCBBAA: .inst 0xddccbbaa ; undefined
libopcodes_ppc.so                AABBCCDD: lha     r21,-13091(r27)       DDCCBBAA: stfdu   f14,-17494(r12)
libopcodes_ppc64.so              AABBCCDD: lha     r21,-13091(r27)       DDCCBBAA: stfdu   f14,-17494(r12)
libopcodes_ppc_403.so            AABBCCDD: lha     r21,-13091(r27)       DDCCBBAA: stfdu   f14,-17494(r12)
libopcodes_ppc_403gc.so          AABBCCDD: lha     r21,-13091(r27)       DDCCBBAA: stfdu   f14,-17494(r12)
libopcodes_ppc_405.so            AABBCCDD: lha     r21,-13091(r27)       DDCCBBAA: stfdu   f14,-17494(r12)
libopcodes_ppc_505.so            AABBCCDD: lha     r21,-13091(r27)       DDCCBBAA: stfdu   f14,-17494(r12)
libopcodes_ppc_601.so            AABBCCDD: lha     r21,-13091(r27)       DDCCBBAA: stfdu   f14,-17494(r12)
libopcodes_ppc_602.so            AABBCCDD: lha     r21,-13091(r27)       DDCCBBAA: stfdu   f14,-17494(r12)
libopcodes_ppc_603.so            AABBCCDD: lha     r21,-13091(r27)       DDCCBBAA: stfdu   f14,-17494(r12)
libopcodes_ppc_604.so            AABBCCDD: lha     r21,-13091(r27)       DDCCBBAA: stfdu   f14,-17494(r12)
libopcodes_ppc_620.so            AABBCCDD: lha     r21,-13091(r27)       DDCCBBAA: stfdu   f14,-17494(r12)
libopcodes_ppc_630.so            AABBCCDD: lha     r21,-13091(r27)       DDCCBBAA: stfdu   f14,-17494(r12)
libopcodes_ppc_7400.so           AABBCCDD: lha     r21,-13091(r27)       DDCCBBAA: stfdu   f14,-17494(r12)
libopcodes_ppc_750.so            AABBCCDD: lha     r21,-13091(r27)       DDCCBBAA: stfdu   f14,-17494(r12)
libopcodes_ppc_860.so            AABBCCDD: lha     r21,-13091(r27)       DDCCBBAA: stfdu   f14,-17494(r12)
libopcodes_ppc_a35.so            AABBCCDD: lha     r21,-13091(r27)       DDCCBBAA: stfdu   f14,-17494(r12)
libopcodes_ppc_e500.so           AABBCCDD: lha     r21,-13091(r27)       DDCCBBAA: stfdu   f14,-17494(r12)
libopcodes_ppc_e500mc.so         AABBCCDD: lha     r21,-13091(r27)       DDCCBBAA: stfdu   f14,-17494(r12)
libopcodes_ppc_e500mc64.so       AABBCCDD: lha     r21,-13091(r27)       DDCCBBAA: stfdu   f14,-17494(r12)
libopcodes_ppc_e5500.so          AABBCCDD: lha     r21,-13091(r27)       DDCCBBAA: stfdu   f14,-17494(r12)
libopcodes_ppc_e6500.so          AABBCCDD: lha     r21,-13091(r27)       DDCCBBAA: stfdu   f14,-17494(r12)
libopcodes_ppc_ec603e.so         AABBCCDD: lha     r21,-13091(r27)       DDCCBBAA: stfdu   f14,-17494(r12)
libopcodes_ppc_rs64ii.so         AABBCCDD: lha     r21,-13091(r27)       DDCCBBAA: stfdu   f14,-17494(r12)
libopcodes_ppc_rs64iii.so        AABBCCDD: lha     r21,-13091(r27)       DDCCBBAA: stfdu   f14,-17494(r12)
libopcodes_ppc_titan.so          AABBCCDD: lha     r21,-13091(r27)       DDCCBBAA: stfdu   f14,-17494(r12)
libopcodes_ppc_vle.so            AABBCCDD: lha     r21,-13091(r27)       DDCCBBAA: stfdu   f14,-17494(r12)
libopcodes_x64.so                AABBCCDD: stos   BYTE PTR es:[rdi],al   DDCCBBAA: (bad)
libopcodes_x86.so                AABBCCDD: stos   BYTE PTR es:[edi],al   DDCCBBAA: (bad)
ppcd_broadway.so                 AABBCCDD: lha r21, -0x3323 (r27)        DDCCBBAA: stfdu fr14, -0x4456 (r12)
ppcd_gecko.so                    AABBCCDD: lha r21, -0x3323 (r27)        DDCCBBAA: stfdu fr14, -0x4456 (r12)
ppcd_powerpc_32.so               AABBCCDD: lha r21, -0x3323 (r27)        DDCCBBAA: stfdu fr14, -0x4456 (r12)
ppcd_powerpc_64.so               AABBCCDD: lha r21, -0x3323 (r27)        DDCCBBAA: stfdu fr14, -0x4456 (r12)
```

## qemu nanomips instructions

```
get qemu: https://download.qemu.org/qemu-8.0.0.tar.xz
./configure
edit qemu-8.0.0/disas/nanomips.c and remove "static" from nanomips_dis()
make
```

