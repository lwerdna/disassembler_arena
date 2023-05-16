#!/usr/bin/env python3
#
# given a shared object, address, and bytes
# resolve and call that object's disassemble function
#
# EXAMPLE:
#  $ ./call_so.py llvm_aarch64_all.so DEADBEEF 1f 20 03 d5
#  nop

import sys
import struct

from ctypes import *

if __name__ == '__main__':
	sopath = sys.argv[1]

	addr = int(sys.argv[2], 16)

	data = ''
	data_ints = list(map(lambda x: int(x,16), sys.argv[3:]))
	data_strs = list(map(lambda x: struct.pack('B', x), data_ints))

	hex_strs = list(map(lambda x: '%02X'%x, data_ints))
	data_pretty = ' '.join(hex_strs)
	data_pretty_ = ' '.join(reversed(hex_strs))

	data_bytes = b''.join(data_strs)
	data_bytes_ = b''.join(reversed(data_strs))

	dll = CDLL(sopath)
	cbuf = create_string_buffer(256)

	pfunc = dll.disassemble
	pfunc.restype = c_int
	pfunc.argtypes = [c_uint64, c_char_p, c_uint, c_void_p]
	print('sopath:', sopath)
	print('dll:', dll)
	#print('addr: 0x%X' % addr)
	#print('data_bytes:', data_bytes)
	#print('len(data):', len(data_bytes))
	rv = dll.disassemble(addr, data_bytes, len(data_bytes), byref(cbuf))
	if rv != 0:
		print("ERROR: %d" % rv);
	else:
		tmp = cbuf.value.decode('utf-8')
		print(tmp, end='')
