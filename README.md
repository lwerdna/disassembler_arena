This project aims to compare the output of many disassemblers.

It compiles disassemblers into shared objects that you can call easily. The provided Makefile has targets like `intel`, `arm`, `thumb`, `aarch64`, `ppc`, `mips`, `sh4`, `nanomips`:

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
...
```

See `README_LIBOPCODES.md` and `README_QEMU.md` for specific instructions on incorporating those projects into the arena.
