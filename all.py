#!/usr/bin/env python3

import os
import re
import sys
import struct
import ctypes

def disasm(sopath, data, addr=0):
	# initialize disassembler, if necessary
	dll = ctypes.CDLL(sopath)
	cbuf = ctypes.create_string_buffer(256)

	dll.disassemble(addr, data, len(data), ctypes.byref(cbuf))
	return cbuf.value.decode('utf-8')
	return tmp

if __name__ == '__main__':
	data = ''

	data_ints = list(map(lambda x: int(x,16), sys.argv[1:]))
	data_strs = list(map(lambda x: struct.pack('B', x), data_ints))
	data_pretty = ' '.join(map(lambda x: '%02X'%x, data_ints))
	data_bytes = b''.join(data_strs)

	libs = filter(lambda fname: re.match(r'.*\.so$', fname), os.listdir('.'))

	print('%s' % (data_pretty))
	for lib in sorted(libs):
		print(lib.ljust(32)+' ', end='')

		instxt = disasm(lib, data_bytes)
		print('%s: %s' % (data_pretty, instxt))
