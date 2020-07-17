CPPFLAGS = 
CPPFLAGS += -I/usr/libopcodesal/include -std=c++11 -O0 -g
BFDFLAGS = -lbfd -liberty -lopcodes -lz 

.PHONY: all

all: ppcd_powerpc_32.so ppcd_powerpc_64.so ppcd_gecko.so ppcd_broadway.so libopcodes.o libopcodes_ppc.so libopcodes_ppc64.so libopcodes_ppc_403.so libopcodes_ppc_403gc.so libopcodes_ppc_405.so libopcodes_ppc_505.so libopcodes_ppc_601.so libopcodes_ppc_602.so libopcodes_ppc_603.so libopcodes_ppc_ec603e.so libopcodes_ppc_604.so libopcodes_ppc_620.so libopcodes_ppc_630.so libopcodes_ppc_750.so libopcodes_ppc_860.so libopcodes_ppc_a35.so libopcodes_ppc_rs64ii.so libopcodes_ppc_rs64iii.so libopcodes_ppc_7400.so libopcodes_ppc_e500.so libopcodes_ppc_e500mc.so libopcodes_ppc_e500mc64.so libopcodes_ppc_e5500.so libopcodes_ppc_e6500.so libopcodes_ppc_titan.so libopcodes_ppc_vle.so libopcodes_aarch64.so capstone_ppc64.so capstone_aarch64.so capstone_x86.so capstone_x64.so

#------------------------------------------------------------------------------
# capstone
#------------------------------------------------------------------------------
capstone_ppc64.so: capstone.cpp
	g++ $(CPPFLAGS) -DPOWERPC_64 -lcapstone -shared capstone.cpp -o capstone_ppc64.so

capstone_aarch64.so: capstone.cpp
	g++ $(CPPFLAGS) -DAARCH64 -lcapstone -shared capstone.cpp -o capstone_aarch64.so

capstone_x86.so: capstone.cpp
	g++ $(CPPFLAGS) -DX86 -lcapstone -shared capstone.cpp -o capstone_x86.so

capstone_x64.so: capstone.cpp
	g++ $(CPPFLAGS) -DX64 -lcapstone -shared capstone.cpp -o capstone_x64.so

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
libopcodes.o: libopcodes.cpp
	g++ -c $(CPPFLAGS) libopcodes.cpp -o libopcodes.o

libopcodes_ppc.so: libopcodes_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC -shared -o libopcodes_ppc.so libopcodes_ppc.cpp libopcodes.o

libopcodes_ppc64.so: libopcodes_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC64 -shared -o libopcodes_ppc64.so libopcodes_ppc.cpp libopcodes.o

libopcodes_ppc_403.so: libopcodes_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_403 -shared -o libopcodes_ppc_403.so libopcodes_ppc.cpp libopcodes.o

libopcodes_ppc_403gc.so: libopcodes_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_403GC -shared -o libopcodes_ppc_403gc.so libopcodes_ppc.cpp libopcodes.o

libopcodes_ppc_405.so: libopcodes_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_405 -shared -o libopcodes_ppc_405.so libopcodes_ppc.cpp libopcodes.o

libopcodes_ppc_505.so: libopcodes_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_505 -shared -o libopcodes_ppc_505.so libopcodes_ppc.cpp libopcodes.o

libopcodes_ppc_601.so: libopcodes_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_601 -shared -o libopcodes_ppc_601.so libopcodes_ppc.cpp libopcodes.o

libopcodes_ppc_602.so: libopcodes_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_602 -shared -o libopcodes_ppc_602.so libopcodes_ppc.cpp libopcodes.o

libopcodes_ppc_603.so: libopcodes_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_603 -shared -o libopcodes_ppc_603.so libopcodes_ppc.cpp libopcodes.o

libopcodes_ppc_ec603e.so: libopcodes_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_EC603E -shared -o libopcodes_ppc_ec603e.so libopcodes_ppc.cpp libopcodes.o

libopcodes_ppc_604.so: libopcodes_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_604 -shared -o libopcodes_ppc_604.so libopcodes_ppc.cpp libopcodes.o

libopcodes_ppc_620.so: libopcodes_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_620 -shared -o libopcodes_ppc_620.so libopcodes_ppc.cpp libopcodes.o

libopcodes_ppc_630.so: libopcodes_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_630 -shared -o libopcodes_ppc_630.so libopcodes_ppc.cpp libopcodes.o

libopcodes_ppc_750.so: libopcodes_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_750 -shared -o libopcodes_ppc_750.so libopcodes_ppc.cpp libopcodes.o

libopcodes_ppc_860.so: libopcodes_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_860 -shared -o libopcodes_ppc_860.so libopcodes_ppc.cpp libopcodes.o

libopcodes_ppc_a35.so: libopcodes_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_A35 -shared -o libopcodes_ppc_a35.so libopcodes_ppc.cpp libopcodes.o

libopcodes_ppc_rs64ii.so: libopcodes_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_RS64II -shared -o libopcodes_ppc_rs64ii.so libopcodes_ppc.cpp libopcodes.o

libopcodes_ppc_rs64iii.so: libopcodes_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_RS64III -shared -o libopcodes_ppc_rs64iii.so libopcodes_ppc.cpp libopcodes.o

libopcodes_ppc_7400.so: libopcodes_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_7400 -shared -o libopcodes_ppc_7400.so libopcodes_ppc.cpp libopcodes.o

libopcodes_ppc_e500.so: libopcodes_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_E500 -shared -o libopcodes_ppc_e500.so libopcodes_ppc.cpp libopcodes.o

libopcodes_ppc_e500mc.so: libopcodes_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_E500MC -shared -o libopcodes_ppc_e500mc.so libopcodes_ppc.cpp libopcodes.o

libopcodes_ppc_e500mc64.so: libopcodes_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_E500MC64 -shared -o libopcodes_ppc_e500mc64.so libopcodes_ppc.cpp libopcodes.o

libopcodes_ppc_e5500.so: libopcodes_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_E5500 -shared -o libopcodes_ppc_e5500.so libopcodes_ppc.cpp libopcodes.o

libopcodes_ppc_e6500.so: libopcodes_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_E6500 -shared -o libopcodes_ppc_e6500.so libopcodes_ppc.cpp libopcodes.o

libopcodes_ppc_titan.so: libopcodes_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_TITAN -shared -o libopcodes_ppc_titan.so libopcodes_ppc.cpp libopcodes.o

libopcodes_ppc_vle.so: libopcodes_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_VLE -shared -o libopcodes_ppc_vle.so libopcodes_ppc.cpp libopcodes.o

libopcodes_aarch64.so: libopcodes_aarch64.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -shared -o libopcodes_aarch64.so libopcodes_aarch64.cpp libopcodes.o

clean:
	rm -rf *.so *.o *.dSYM
