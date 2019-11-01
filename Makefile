CPPFLAGS = 
CPPFLAGS += -I/usr/libopcodesal/include -std=c++11 -O0 -g
BFDFLAGS = -lbfd -liberty -lopcodes -lz 

all: libopcodes.o libopcodes_ppc.so libopcodes_ppc64.so libopcodes_403.so libopcodes_403gc.so libopcodes_405.so libopcodes_505.so libopcodes_601.so libopcodes_602.so libopcodes_603.so libopcodes_ec603e.so libopcodes_604.so libopcodes_620.so libopcodes_630.so libopcodes_750.so libopcodes_860.so libopcodes_a35.so libopcodes_rs64ii.so libopcodes_rs64iii.so libopcodes_7400.so libopcodes_e500.so libopcodes_e500mc.so libopcodes_e500mc64.so libopcodes_e5500.so libopcodes_e6500.so libopcodes_titan.so libopcodes_vle.so

libopcodes.o: libopcodes.cpp
	g++ -c $(CPPFLAGS) libopcodes.cpp -o libopcodes.o

libopcodes_ppc.so: libopcodes_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC -shared -o libopcodes_ppc.so libopcodes_ppc.cpp libopcodes.o

libopcodes_ppc64.so: libopcodes_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC64 -shared -o libopcodes_ppc64.so libopcodes_ppc.cpp libopcodes.o

libopcodes_403.so: libopcodes_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_403 -shared -o libopcodes_ppc_403.so libopcodes_ppc.cpp libopcodes.o

libopcodes_403gc.so: libopcodes_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_403GC -shared -o libopcodes_ppc_403gc.so libopcodes_ppc.cpp libopcodes.o

libopcodes_405.so: libopcodes_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_405 -shared -o libopcodes_ppc_405.so libopcodes_ppc.cpp libopcodes.o

libopcodes_505.so: libopcodes_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_505 -shared -o libopcodes_ppc_505.so libopcodes_ppc.cpp libopcodes.o

libopcodes_601.so: libopcodes_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_601 -shared -o libopcodes_ppc_601.so libopcodes_ppc.cpp libopcodes.o

libopcodes_602.so: libopcodes_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_602 -shared -o libopcodes_ppc_602.so libopcodes_ppc.cpp libopcodes.o

libopcodes_603.so: libopcodes_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_603 -shared -o libopcodes_ppc_603.so libopcodes_ppc.cpp libopcodes.o

libopcodes_ec603e.so: libopcodes_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_EC603E -shared -o libopcodes_ppc_ec603e.so libopcodes_ppc.cpp libopcodes.o

libopcodes_604.so: libopcodes_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_604 -shared -o libopcodes_ppc_604.so libopcodes_ppc.cpp libopcodes.o

libopcodes_620.so: libopcodes_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_620 -shared -o libopcodes_ppc_620.so libopcodes_ppc.cpp libopcodes.o

libopcodes_630.so: libopcodes_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_630 -shared -o libopcodes_ppc_630.so libopcodes_ppc.cpp libopcodes.o

libopcodes_750.so: libopcodes_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_750 -shared -o libopcodes_ppc_750.so libopcodes_ppc.cpp libopcodes.o

libopcodes_860.so: libopcodes_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_860 -shared -o libopcodes_ppc_860.so libopcodes_ppc.cpp libopcodes.o

libopcodes_a35.so: libopcodes_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_A35 -shared -o libopcodes_ppc_a35.so libopcodes_ppc.cpp libopcodes.o

libopcodes_rs64ii.so: libopcodes_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_RS64II -shared -o libopcodes_ppc_rs64ii.so libopcodes_ppc.cpp libopcodes.o

libopcodes_rs64iii.so: libopcodes_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_RS64III -shared -o libopcodes_ppc_rs64iii.so libopcodes_ppc.cpp libopcodes.o

libopcodes_7400.so: libopcodes_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_7400 -shared -o libopcodes_ppc_7400.so libopcodes_ppc.cpp libopcodes.o

libopcodes_e500.so: libopcodes_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_E500 -shared -o libopcodes_ppc_e500.so libopcodes_ppc.cpp libopcodes.o

libopcodes_e500mc.so: libopcodes_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_E500MC -shared -o libopcodes_ppc_e500mc.so libopcodes_ppc.cpp libopcodes.o

libopcodes_e500mc64.so: libopcodes_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_E500MC64 -shared -o libopcodes_ppc_e500mc64.so libopcodes_ppc.cpp libopcodes.o

libopcodes_e5500.so: libopcodes_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_E5500 -shared -o libopcodes_ppc_e5500.so libopcodes_ppc.cpp libopcodes.o

libopcodes_e6500.so: libopcodes_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_E6500 -shared -o libopcodes_ppc_e6500.so libopcodes_ppc.cpp libopcodes.o

libopcodes_titan.so: libopcodes_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_TITAN -shared -o libopcodes_ppc_titan.so libopcodes_ppc.cpp libopcodes.o

libopcodes_vle.so: libopcodes_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_VLE -shared -o libopcodes_ppc_vle.so libopcodes_ppc.cpp libopcodes.o

clean:
	rm -rf *.so *.o *.dSYM
