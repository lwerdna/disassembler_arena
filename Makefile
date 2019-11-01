CPPFLAGS = 
CPPFLAGS += -I/usr/local/include -std=c++11 -O0 -g
BFDFLAGS = -lbfd -liberty -lopcodes -lz 

all: libopcodes.o loc_ppc.so loc_ppc64.so loc_403.so loc_403gc.so loc_405.so loc_505.so loc_601.so loc_602.so loc_603.so loc_ec603e.so loc_604.so loc_620.so loc_630.so loc_750.so loc_860.so loc_a35.so loc_rs64ii.so loc_rs64iii.so loc_7400.so loc_e500.so loc_e500mc.so loc_e500mc64.so loc_e5500.so loc_e6500.so loc_titan.so loc_vle.so

libopcodes.o: libopcodes.cpp
	g++ -c $(CPPFLAGS) libopcodes.cpp -o libopcodes.o

loc_ppc.so: loc_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC -shared -o loc_ppc.so loc_ppc.cpp libopcodes.o

loc_ppc64.so: loc_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC64 -shared -o loc_ppc64.so loc_ppc.cpp libopcodes.o

loc_403.so: loc_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_403 -shared -o loc_ppc_403.so loc_ppc.cpp libopcodes.o

loc_403gc.so: loc_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_403GC -shared -o loc_ppc_403gc.so loc_ppc.cpp libopcodes.o

loc_405.so: loc_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_405 -shared -o loc_ppc_405.so loc_ppc.cpp libopcodes.o

loc_505.so: loc_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_505 -shared -o loc_ppc_505.so loc_ppc.cpp libopcodes.o

loc_601.so: loc_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_601 -shared -o loc_ppc_601.so loc_ppc.cpp libopcodes.o

loc_602.so: loc_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_602 -shared -o loc_ppc_602.so loc_ppc.cpp libopcodes.o

loc_603.so: loc_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_603 -shared -o loc_ppc_603.so loc_ppc.cpp libopcodes.o

loc_ec603e.so: loc_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_EC603E -shared -o loc_ppc_ec603e.so loc_ppc.cpp libopcodes.o

loc_604.so: loc_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_604 -shared -o loc_ppc_604.so loc_ppc.cpp libopcodes.o

loc_620.so: loc_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_620 -shared -o loc_ppc_620.so loc_ppc.cpp libopcodes.o

loc_630.so: loc_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_630 -shared -o loc_ppc_630.so loc_ppc.cpp libopcodes.o

loc_750.so: loc_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_750 -shared -o loc_ppc_750.so loc_ppc.cpp libopcodes.o

loc_860.so: loc_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_860 -shared -o loc_ppc_860.so loc_ppc.cpp libopcodes.o

loc_a35.so: loc_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_A35 -shared -o loc_ppc_a35.so loc_ppc.cpp libopcodes.o

loc_rs64ii.so: loc_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_RS64II -shared -o loc_ppc_rs64ii.so loc_ppc.cpp libopcodes.o

loc_rs64iii.so: loc_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_RS64III -shared -o loc_ppc_rs64iii.so loc_ppc.cpp libopcodes.o

loc_7400.so: loc_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_7400 -shared -o loc_ppc_7400.so loc_ppc.cpp libopcodes.o

loc_e500.so: loc_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_E500 -shared -o loc_ppc_e500.so loc_ppc.cpp libopcodes.o

loc_e500mc.so: loc_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_E500MC -shared -o loc_ppc_e500mc.so loc_ppc.cpp libopcodes.o

loc_e500mc64.so: loc_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_E500MC64 -shared -o loc_ppc_e500mc64.so loc_ppc.cpp libopcodes.o

loc_e5500.so: loc_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_E5500 -shared -o loc_ppc_e5500.so loc_ppc.cpp libopcodes.o

loc_e6500.so: loc_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_E6500 -shared -o loc_ppc_e6500.so loc_ppc.cpp libopcodes.o

loc_titan.so: loc_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_TITAN -shared -o loc_ppc_titan.so loc_ppc.cpp libopcodes.o

loc_vle.so: loc_ppc.cpp libopcodes.o
	g++ $(CPPFLAGS) $(BFDFLAGS) -DPPC_VLE -shared -o loc_ppc_vle.so loc_ppc.cpp libopcodes.o

clean:
	rm -rf *.so *.o *.dSYM
