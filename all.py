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
	tmp = cbuf.value.decode('utf-8')
	tmp = tmp.replace('\t', ' ')
	return tmp

if __name__ == '__main__':
	data = ''

	data_ints = list(map(lambda x: int(x,16), sys.argv[1:]))
	data_strs = list(map(lambda x: struct.pack('B', x), data_ints))

	hex_strs = list(map(lambda x: '%02X'%x, data_ints))
	data_pretty = ''.join(hex_strs)
	data_pretty_ = ''.join(reversed(hex_strs))

	data_bytes = b''.join(data_strs)
	data_bytes_ = b''.join(reversed(data_strs))

	libs = filter(lambda fname: re.match(r'.*\.so$', fname), os.listdir('.'))

	print('%s' % (data_pretty))
	for lib in sorted(libs):
		print(lib.ljust(32)+' ', end='', flush=True)

		print(('%s: %s' % (data_pretty, disasm(lib, data_bytes).rstrip())).ljust(40), end='')
		print(('%s: %s' % (data_pretty_, disasm(lib, data_bytes_).rstrip())))
