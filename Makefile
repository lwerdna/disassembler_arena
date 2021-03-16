CPPFLAGS =
CPPFLAGS += -std=c++11 -O0 -g
BFDFLAGS = -lbfd -liberty -lopcodes -lz

.PHONY: all

# ppcd
TARGETS_PPCD_PPC = ppcd_powerpc_32.so ppcd_powerpc_64.so ppcd_gecko.so ppcd_broadway.so

# libopcodes
TARGETS_LIBOPCODES_INTEL = libopcodes_x86.so libopcodes_x64.so
TARGETS_LIBOPCODES_ARM = libopcodes_arm_unknown.so libopcodes_arm_2.so libopcodes_arm_2a.so libopcodes_arm_3.so libopcodes_arm_3m.so libopcodes_arm_4.so libopcodes_arm_4t.so libopcodes_arm_5.so libopcodes_arm_5t.so libopcodes_arm_5te.so libopcodes_arm_xscale.so libopcodes_arm_ep9312.so libopcodes_arm_iwmmxt.so libopcodes_arm_iwmmxt2.so
TARGETS_LIBOPCODES_AARCH64 = libopcodes_aarch64.so libopcodes_aarch64_ilp32.so
TARGETS_LIBOPCODES_PPC = libopcodes_ppc.so libopcodes_ppc64.so libopcodes_ppc_403.so libopcodes_ppc_403gc.so libopcodes_ppc_405.so libopcodes_ppc_505.so libopcodes_ppc_601.so libopcodes_ppc_602.so libopcodes_ppc_603.so libopcodes_ppc_ec603e.so libopcodes_ppc_604.so libopcodes_ppc_620.so libopcodes_ppc_630.so libopcodes_ppc_750.so libopcodes_ppc_860.so libopcodes_ppc_a35.so libopcodes_ppc_rs64ii.so libopcodes_ppc_rs64iii.so libopcodes_ppc_7400.so libopcodes_ppc_e500.so libopcodes_ppc_e500mc.so libopcodes_ppc_e500mc64.so libopcodes_ppc_e5500.so libopcodes_ppc_e6500.so libopcodes_ppc_titan.so libopcodes_ppc_vle.so
TARGETS_LIBOPCODES_SH4 = libopcodes_sh4.so

# capstone
TARGETS_CAPSTONE_INTEL = capstone_x86.so capstone_x64.so
TARGETS_CAPSTONE_ARM = capstone_arm.so capstone_arm_v8.so
TARGETS_CAPSTONE_THUMB = capstone_arm_thumb.so capstone_arm_thumb_mclass.so capstone_arm_thumb_mclass_v8.so
TARGETS_CAPSTONE_AARCH64 = capstone_aarch64.so
TARGETS_CAPSTONE_PPC = capstone_ppc64.so

# llvm
TARGETS_LLVM_ARM = llvm_arm.so llvm_arm_v8_1a.so llvm_arm_v8_2a.so llvm_arm_v8_3a.so llvm_arm_v8_4a.so llvm_arm_v8_5a.so llvm_arm_v8_all.so
TARGETS_LLVM_THUMB = llvm_thumb.so
TARGETS_LLVM_AARCH64 = llvm_aarch64.so llvm_aarch64_1a.so llvm_aarch64_2a.so llvm_aarch64_3a.so llvm_aarch64_4a.so llvm_aarch64_5a.so llvm_aarch64_all.so

# binary ninja
TARGETS_BINJA_INTEL = binja_x86.so binja_x64.so
TARGETS_BINJA_ARM = binja_armv7.so
TARGETS_BINJA_THUMB = binja_thumb2.so
TARGETS_BINJA_AARCH64 = binja_aarch64.so
TARGETS_BINJA_PPC = binja_ppc.so
TARGETS_BINJA_MIPS = binja_mips32.so

TARGETS_INTEL = $(TARGETS_LIBOPCODES_INTEL) $(TARGETS_CAPSTONE_INTEL) $(TARGETS_BINJA_INTEL)
TARGETS_ARM = $(TARGETS_LIBOPCODES_ARM) $(TARGETS_CAPSTONE_ARM) $(TARGETS_LLVM_ARM) $(TARGETS_BINJA_ARM)
TARGETS_THUMB = $(TARGETS_CAPSTONE_THUMB) $(TARGETS_LLVM_THUMB) $(TARGETS_BINJA_THUMB)
TARGETS_AARCH64 = $(TARGETS_LIBOPCODES_AARCH64) $(TARGETS_CAPSTONE_AARCH64) $(TARGETS_LLVM_AARCH64) $(TARGETS_BINJA_AARCH64)
TARGETS_PPC = $(TARGETS_PPCD_PPC) $(TARGETS_LIBOPCODES_PPC) $(TARGETS_CAPSTONE_PPC) $(TARGETS_BINJA_PPC)
TARGETS_MIPS = $(TARGETS_BINJA_MIPS)

all: $(TARGETS_INTEL) $(TARGETS_ARM) $(TARGETS_AARCH64) $(TARGETS_PPC) $(TARGETS_MIPS) call_so

intel: $(TARGETS_INTEL) call_so

arm: $(TARGETS_ARM) call_so

thumb: $(TARGETS_THUMB) call_so

aarch64: $(TARGETS_AARCH64) call_so

ppc: $(TARGETS_PPC) call_so

mips: $(TARGETS_MIPS) call_so

sh4: $(TARGETS_LIBOPCODES_SH4) call_so

clean:
	rm -rf *.so *.o *.dSYM

#------------------------------------------------------------------------------
# capstone
#------------------------------------------------------------------------------

# x86/x64

capstone_x86.so: capstone.cpp
	g++ $(CPPFLAGS) -DX86 -lcapstone -shared capstone.cpp -o capstone_x86.so

capstone_x64.so: capstone.cpp
	g++ $(CPPFLAGS) -DX64 -lcapstone -shared capstone.cpp -o capstone_x64.so

# arm

capstone_arm.so: capstone.cpp
	g++ $(CPPFLAGS) -DARM -lcapstone -shared capstone.cpp -o capstone_arm.so

capstone_arm_v8.so: capstone.cpp
	g++ $(CPPFLAGS) -DARM_V8 -lcapstone -shared capstone.cpp -o capstone_arm_v8.so

capstone_arm_thumb.so: capstone.cpp
	g++ $(CPPFLAGS) -DARM_THUMB -lcapstone -shared capstone.cpp -o capstone_arm_thumb.so

capstone_arm_thumb_mclass.so: capstone.cpp
	g++ $(CPPFLAGS) -DARM_THUMB_MCLASS -lcapstone -shared capstone.cpp -o capstone_arm_thumb_mclass.so

capstone_arm_thumb_mclass_v8.so: capstone.cpp
	g++ $(CPPFLAGS) -DARM_THUMB_MCLASS_V8 -lcapstone -shared capstone.cpp -o capstone_arm_thumb_mclass_v8.so

capstone_aarch64.so: capstone.cpp
	g++ $(CPPFLAGS) -DAARCH64 -lcapstone -shared capstone.cpp -o capstone_aarch64.so

# powerpc

capstone_ppc64.so: capstone.cpp
	g++ $(CPPFLAGS) -DPOWERPC_64 -lcapstone -shared capstone.cpp -o capstone_ppc64.so

#------------------------------------------------------------------------------
# ppcd
#------------------------------------------------------------------------------

ppcd_powerpc_32.so: ppcd_main.cpp ppcd.cpp ppcd.h
	g++ $(CPPFLAGS) -DPOWERPC_32 -shared ppcd.cpp ppcd_main.cpp -o ppcd_powerpc_32.so

ppcd_powerpc_64.so: ppcd_main.cpp ppcd.cpp ppcd.h
	g++ $(CPPFLAGS) -DPOWERPC_64 -shared ppcd.cpp ppcd_main.cpp -o ppcd_powerpc_64.so

ppcd_gecko.so: ppcd_main.cpp ppcd.cpp ppcd.h
	g++ $(CPPFLAGS) -DGECKO -shared ppcd.cpp ppcd_main.cpp -o ppcd_gecko.so

ppcd_broadway.so: ppcd_main.cpp ppcd.cpp ppcd.h
	g++ $(CPPFLAGS) -DBROADWAY -shared ppcd.cpp ppcd_main.cpp -o ppcd_broadway.so

#------------------------------------------------------------------------------
# libbfd/libopcodes
#------------------------------------------------------------------------------

# x86/x64

libopcodes_x86.so: libopcodes.cpp utils.o
	g++ $(CPPFLAGS) $(BFDFLAGS) utils.o -DX86 -shared -o libopcodes_x86.so libopcodes.cpp

libopcodes_x64.so: libopcodes.cpp utils.o
	g++ $(CPPFLAGS) $(BFDFLAGS) utils.o -DX64 -shared -o libopcodes_x64.so libopcodes.cpp

# powerpc

libopcodes_ppc.so: libopcodes.cpp utils.o
	g++ $(CPPFLAGS) $(BFDFLAGS) utils.o -DPPC -shared -o libopcodes_ppc.so libopcodes.cpp

libopcodes_ppc64.so: libopcodes.cpp utils.o
	g++ $(CPPFLAGS) $(BFDFLAGS) utils.o -DPPC64 -shared -o libopcodes_ppc64.so libopcodes.cpp

libopcodes_ppc_403.so: libopcodes.cpp utils.o
	g++ $(CPPFLAGS) $(BFDFLAGS) utils.o -DPPC_403 -shared -o libopcodes_ppc_403.so libopcodes.cpp

libopcodes_ppc_403gc.so: libopcodes.cpp utils.o
	g++ $(CPPFLAGS) $(BFDFLAGS) utils.o -DPPC_403GC -shared -o libopcodes_ppc_403gc.so libopcodes.cpp

libopcodes_ppc_405.so: libopcodes.cpp utils.o
	g++ $(CPPFLAGS) $(BFDFLAGS) utils.o -DPPC_405 -shared -o libopcodes_ppc_405.so libopcodes.cpp

libopcodes_ppc_505.so: libopcodes.cpp utils.o
	g++ $(CPPFLAGS) $(BFDFLAGS) utils.o -DPPC_505 -shared -o libopcodes_ppc_505.so libopcodes.cpp

libopcodes_ppc_601.so: libopcodes.cpp utils.o
	g++ $(CPPFLAGS) $(BFDFLAGS) utils.o -DPPC_601 -shared -o libopcodes_ppc_601.so libopcodes.cpp

libopcodes_ppc_602.so: libopcodes.cpp utils.o
	g++ $(CPPFLAGS) $(BFDFLAGS) utils.o -DPPC_602 -shared -o libopcodes_ppc_602.so libopcodes.cpp

libopcodes_ppc_603.so: libopcodes.cpp utils.o
	g++ $(CPPFLAGS) $(BFDFLAGS) utils.o -DPPC_603 -shared -o libopcodes_ppc_603.so libopcodes.cpp

libopcodes_ppc_ec603e.so: libopcodes.cpp utils.o
	g++ $(CPPFLAGS) $(BFDFLAGS) utils.o -DPPC_EC603E -shared -o libopcodes_ppc_ec603e.so libopcodes.cpp

libopcodes_ppc_604.so: libopcodes.cpp utils.o
	g++ $(CPPFLAGS) $(BFDFLAGS) utils.o -DPPC_604 -shared -o libopcodes_ppc_604.so libopcodes.cpp

libopcodes_ppc_620.so: libopcodes.cpp utils.o
	g++ $(CPPFLAGS) $(BFDFLAGS) utils.o -DPPC_620 -shared -o libopcodes_ppc_620.so libopcodes.cpp

libopcodes_ppc_630.so: libopcodes.cpp utils.o
	g++ $(CPPFLAGS) $(BFDFLAGS) utils.o -DPPC_630 -shared -o libopcodes_ppc_630.so libopcodes.cpp

libopcodes_ppc_750.so: libopcodes.cpp utils.o
	g++ $(CPPFLAGS) $(BFDFLAGS) utils.o -DPPC_750 -shared -o libopcodes_ppc_750.so libopcodes.cpp

libopcodes_ppc_860.so: libopcodes.cpp utils.o
	g++ $(CPPFLAGS) $(BFDFLAGS) utils.o -DPPC_860 -shared -o libopcodes_ppc_860.so libopcodes.cpp

libopcodes_ppc_a35.so: libopcodes.cpp utils.o
	g++ $(CPPFLAGS) $(BFDFLAGS) utils.o -DPPC_A35 -shared -o libopcodes_ppc_a35.so libopcodes.cpp

libopcodes_ppc_rs64ii.so: libopcodes.cpp utils.o
	g++ $(CPPFLAGS) $(BFDFLAGS) utils.o -DPPC_RS64II -shared -o libopcodes_ppc_rs64ii.so libopcodes.cpp

libopcodes_ppc_rs64iii.so: libopcodes.cpp utils.o
	g++ $(CPPFLAGS) $(BFDFLAGS) utils.o -DPPC_RS64III -shared -o libopcodes_ppc_rs64iii.so libopcodes.cpp

libopcodes_ppc_7400.so: libopcodes.cpp utils.o
	g++ $(CPPFLAGS) $(BFDFLAGS) utils.o -DPPC_7400 -shared -o libopcodes_ppc_7400.so libopcodes.cpp

libopcodes_ppc_e500.so: libopcodes.cpp utils.o
	g++ $(CPPFLAGS) $(BFDFLAGS) utils.o -DPPC_E500 -shared -o libopcodes_ppc_e500.so libopcodes.cpp

libopcodes_ppc_e500mc.so: libopcodes.cpp utils.o
	g++ $(CPPFLAGS) $(BFDFLAGS) utils.o -DPPC_E500MC -shared -o libopcodes_ppc_e500mc.so libopcodes.cpp

libopcodes_ppc_e500mc64.so: libopcodes.cpp utils.o
	g++ $(CPPFLAGS) $(BFDFLAGS) utils.o -DPPC_E500MC64 -shared -o libopcodes_ppc_e500mc64.so libopcodes.cpp

libopcodes_ppc_e5500.so: libopcodes.cpp utils.o
	g++ $(CPPFLAGS) $(BFDFLAGS) utils.o -DPPC_E5500 -shared -o libopcodes_ppc_e5500.so libopcodes.cpp

libopcodes_ppc_e6500.so: libopcodes.cpp utils.o
	g++ $(CPPFLAGS) $(BFDFLAGS) utils.o -DPPC_E6500 -shared -o libopcodes_ppc_e6500.so libopcodes.cpp

libopcodes_ppc_titan.so: libopcodes.cpp utils.o
	g++ $(CPPFLAGS) $(BFDFLAGS) utils.o -DPPC_TITAN -shared -o libopcodes_ppc_titan.so libopcodes.cpp

libopcodes_ppc_vle.so: libopcodes.cpp utils.o
	g++ $(CPPFLAGS) $(BFDFLAGS) utils.o -DPPC_VLE -shared -o libopcodes_ppc_vle.so libopcodes.cpp

# arm

libopcodes_arm_unknown.so: libopcodes.cpp utils.o
	g++ $(CPPFLAGS) $(BFDFLAGS) utils.o -DARM_UNKNOWN -shared -o libopcodes_arm_unknown.so libopcodes.cpp

libopcodes_arm_2.so: libopcodes.cpp utils.o
	g++ $(CPPFLAGS) $(BFDFLAGS) utils.o -DARM_2 -shared -o libopcodes_arm_2.so libopcodes.cpp

libopcodes_arm_2a.so: libopcodes.cpp utils.o
	g++ $(CPPFLAGS) $(BFDFLAGS) utils.o -DARM_2a -shared -o libopcodes_arm_2a.so libopcodes.cpp

libopcodes_arm_3.so: libopcodes.cpp utils.o
	g++ $(CPPFLAGS) $(BFDFLAGS) utils.o -DARM_3 -shared -o libopcodes_arm_3.so libopcodes.cpp

libopcodes_arm_3m.so: libopcodes.cpp utils.o
	g++ $(CPPFLAGS) $(BFDFLAGS) utils.o -DARM_3M -shared -o libopcodes_arm_3m.so libopcodes.cpp

libopcodes_arm_4.so: libopcodes.cpp utils.o
	g++ $(CPPFLAGS) $(BFDFLAGS) utils.o -DARM_4 -shared -o libopcodes_arm_4.so libopcodes.cpp

libopcodes_arm_4t.so: libopcodes.cpp utils.o
	g++ $(CPPFLAGS) $(BFDFLAGS) utils.o -DARM_4T -shared -o libopcodes_arm_4t.so libopcodes.cpp

libopcodes_arm_5.so: libopcodes.cpp utils.o
	g++ $(CPPFLAGS) $(BFDFLAGS) utils.o -DARM_5 -shared -o libopcodes_arm_5.so libopcodes.cpp

libopcodes_arm_5t.so: libopcodes.cpp utils.o
	g++ $(CPPFLAGS) $(BFDFLAGS) utils.o -DARM_5T -shared -o libopcodes_arm_5t.so libopcodes.cpp

libopcodes_arm_5te.so: libopcodes.cpp utils.o
	g++ $(CPPFLAGS) $(BFDFLAGS) utils.o -DARM_5TE -shared -o libopcodes_arm_5te.so libopcodes.cpp

libopcodes_arm_xscale.so: libopcodes.cpp utils.o
	g++ $(CPPFLAGS) $(BFDFLAGS) utils.o -DARM_XSCALE -shared -o libopcodes_arm_xscale.so libopcodes.cpp

libopcodes_arm_ep9312.so: libopcodes.cpp utils.o
	g++ $(CPPFLAGS) $(BFDFLAGS) utils.o -DARM_EP9312 -shared -o libopcodes_arm_ep9312.so libopcodes.cpp

libopcodes_arm_iwmmxt.so: libopcodes.cpp utils.o
	g++ $(CPPFLAGS) $(BFDFLAGS) utils.o -DARM_IWMMXT -shared -o libopcodes_arm_iwmmxt.so libopcodes.cpp

libopcodes_arm_iwmmxt2.so: libopcodes.cpp utils.o
	g++ $(CPPFLAGS) $(BFDFLAGS) utils.o -DARM_IWMMXT2 -shared -o libopcodes_arm_iwmmxt2.so libopcodes.cpp

# aarch64

libopcodes_aarch64.so: libopcodes.cpp utils.o
	g++ $(CPPFLAGS) $(BFDFLAGS) utils.o -DAARCH64 -shared -o libopcodes_aarch64.so libopcodes.cpp

libopcodes_aarch64_ilp32.so: libopcodes.cpp utils.o
	g++ $(CPPFLAGS) $(BFDFLAGS) utils.o -DAARCH64_ILP32 -shared -o libopcodes_aarch64_ilp32.so libopcodes.cpp

# sh4

libopcodes_sh4.so: libopcodes.cpp utils.o
	g++ $(CPPFLAGS) $(BFDFLAGS) utils.o -DSH4 -shared -o libopcodes_sh4.so libopcodes.cpp

#------------------------------------------------------------------------------
# LLVM
#------------------------------------------------------------------------------

PATH_LLVM =

LLVM_COMPILE_FLAGS = $(shell llvm-config --cxxflags)
LLVM_LINK_FLAGS = $(shell llvm-config --ldflags) $(shell llvm-config --libs) $(shell llvm-config --system-libs)

# arm

llvm_arm.so: llvm.cpp utils.o
	g++ $(CPPFLAGS) $(LLVM_COMPILE_FLAGS) $(LLVM_LINK_FLAGS) utils.o -DARM_NAKED -shared -o llvm_arm.so llvm.cpp

llvm_arm_v8_1a.so: llvm.cpp utils.o
	g++ $(CPPFLAGS) $(LLVM_COMPILE_FLAGS) $(LLVM_LINK_FLAGS) utils.o -DARM_V8_1A -shared -o llvm_arm_v8_1a.so llvm.cpp

llvm_arm_v8_2a.so: llvm.cpp utils.o
	g++ $(CPPFLAGS) $(LLVM_COMPILE_FLAGS) $(LLVM_LINK_FLAGS) utils.o -DARM_V8_2A -shared -o llvm_arm_v8_2a.so llvm.cpp

llvm_arm_v8_3a.so: llvm.cpp utils.o
	g++ $(CPPFLAGS) $(LLVM_COMPILE_FLAGS) $(LLVM_LINK_FLAGS) utils.o -DARM_V8_3A -shared -o llvm_arm_v8_3a.so llvm.cpp

llvm_arm_v8_4a.so: llvm.cpp utils.o
	g++ $(CPPFLAGS) $(LLVM_COMPILE_FLAGS) $(LLVM_LINK_FLAGS) utils.o -DARM_V8_4A -shared -o llvm_arm_v8_4a.so llvm.cpp

llvm_arm_v8_5a.so: llvm.cpp utils.o
	g++ $(CPPFLAGS) $(LLVM_COMPILE_FLAGS) $(LLVM_LINK_FLAGS) utils.o -DARM_V8_5A -shared -o llvm_arm_v8_5a.so llvm.cpp

llvm_arm_v8_all.so: llvm.cpp utils.o
	g++ $(CPPFLAGS) $(LLVM_COMPILE_FLAGS) $(LLVM_LINK_FLAGS) utils.o -DARM_V8_ALL -shared -o llvm_arm_v8_all.so llvm.cpp

# thumb

llvm_thumb.so: llvm.cpp utils.o
	g++ $(CPPFLAGS) $(LLVM_COMPILE_FLAGS) $(LLVM_LINK_FLAGS) utils.o -DARM_THUMB -shared -o llvm_thumb.so llvm.cpp

# aarch64

llvm_aarch64.so: llvm.cpp utils.o
	g++ $(CPPFLAGS) $(LLVM_COMPILE_FLAGS) $(LLVM_LINK_FLAGS) utils.o -DAARCH64_ARMV8 -shared -o llvm_aarch64.so llvm.cpp

llvm_aarch64_1a.so: llvm.cpp utils.o
	g++ $(CPPFLAGS) $(LLVM_COMPILE_FLAGS) $(LLVM_LINK_FLAGS) utils.o -DAARCH64_ARMV8_1A -shared -o llvm_aarch64_1a.so llvm.cpp

llvm_aarch64_2a.so: llvm.cpp utils.o
	g++ $(CPPFLAGS) $(LLVM_COMPILE_FLAGS) $(LLVM_LINK_FLAGS) utils.o -DAARCH64_ARMV8_2A -shared -o llvm_aarch64_2a.so llvm.cpp

llvm_aarch64_3a.so: llvm.cpp utils.o
	g++ $(CPPFLAGS) $(LLVM_COMPILE_FLAGS) $(LLVM_LINK_FLAGS) utils.o -DAARCH64_ARMV8_3A -shared -o llvm_aarch64_3a.so llvm.cpp

llvm_aarch64_4a.so: llvm.cpp utils.o
	g++ $(CPPFLAGS) $(LLVM_COMPILE_FLAGS) $(LLVM_LINK_FLAGS) utils.o -DAARCH64_ARMV8_4A -shared -o llvm_aarch64_4a.so llvm.cpp

llvm_aarch64_5a.so: llvm.cpp utils.o
	g++ $(CPPFLAGS) $(LLVM_COMPILE_FLAGS) $(LLVM_LINK_FLAGS) utils.o -DAARCH64_ARMV8_5A -shared -o llvm_aarch64_5a.so llvm.cpp

llvm_aarch64_all.so: llvm.cpp utils.o
	g++ $(CPPFLAGS) $(LLVM_COMPILE_FLAGS) $(LLVM_LINK_FLAGS) utils.o -DAARCH64_ARMV8_ALL -shared -o llvm_aarch64_all.so llvm.cpp

#------------------------------------------------------------------------------
# BINARY NINJA
#------------------------------------------------------------------------------

BINJA_API ?= /path/to/api # from https://github.com/Vector35/binaryninja-api
BINJA_APP ?= /path/to/app # eg: /Applications/Binary\ Ninja.app
BINJA_COMPILE_FLAGS = -I$(BINJA_API) -DPATH_BUNDLED_PLUGINS='"$(BINJA_APP)/Contents/MacOS/plugins"'
BINJA_LINK_FLAGS = -L$(BINJA_APP)/Contents/MacOS -Wl,-rpath,$(BINJA_APP)/Contents/MacOS -lbinaryninjacore

binja_x86.so: binaryninja.cpp
	g++ -DX86 $(CPPFLAGS) $(BINJA_COMPILE_FLAGS) binaryninja.cpp $(BINJA_LINK_FLAGS) -shared -o binja_x86.so

binja_x64.so: binaryninja.cpp
	g++ -DX64 $(CPPFLAGS) $(BINJA_COMPILE_FLAGS) binaryninja.cpp $(BINJA_LINK_FLAGS) -shared -o binja_x64.so

binja_armv7.so: binaryninja.cpp
	g++ -DARMV7 $(CPPFLAGS) $(BINJA_COMPILE_FLAGS) binaryninja.cpp $(BINJA_LINK_FLAGS) -shared -o binja_armv7.so

binja_thumb2.so: binaryninja.cpp
	g++ -DTHUMB2 $(CPPFLAGS) $(BINJA_COMPILE_FLAGS) binaryninja.cpp $(BINJA_LINK_FLAGS) -shared -o binja_thumb2.so

binja_ppc.so: binaryninja.cpp
	g++ -DPPC $(CPPFLAGS) $(BINJA_COMPILE_FLAGS) binaryninja.cpp $(BINJA_LINK_FLAGS) -shared -o binja_ppc.so

binja_aarch64.so: binaryninja.cpp
	g++ -DAARCH64 $(CPPFLAGS) $(BINJA_COMPILE_FLAGS) binaryninja.cpp $(BINJA_LINK_FLAGS) -shared -o binja_aarch64.so

binja_mips32.so: binaryninja.cpp
	g++ -DMIPS32 $(CPPFLAGS) $(BINJA_COMPILE_FLAGS) binaryninja.cpp $(BINJA_LINK_FLAGS) -shared -o binja_mips32.so

#------------------------------------------------------------------------------
# misc
#------------------------------------------------------------------------------
utils.o: utils.cpp
	g++ -c utils.cpp -o utils.o

call_so: call_so.c
	gcc call_so.c -o call_so
