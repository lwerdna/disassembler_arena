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

	rv = dll.disassemble(addr, data, len(data), ctypes.byref(cbuf))
	if rv != 0:
		tmp = '(error)'
	else:
		tmp = cbuf.value.decode('utf-8')
		tmp = tmp.strip()
		tmp = tmp.replace('\t', ' ')
	return tmp

if __name__ == '__main__':
	data = ''

	data_ints = list(map(lambda x: int(x,16), sys.argv[1:]))
	data_strs = list(map(lambda x: struct.pack('B', x), data_ints))

	hex_strs = list(map(lambda x: '%02X'%x, data_ints))
	data_pretty = ' '.join(hex_strs)
	data_pretty_ = ' '.join(reversed(hex_strs))

	data_bytes = b''.join(data_strs)
	data_bytes_ = b''.join(reversed(data_strs))

	libs = list(filter(lambda fname: re.match(r'.*\.so$', fname), os.listdir('.')))

	col1_width = 32
	col2_width = 40
	col3_width = 40

	print('%s%s%s' % (' '*col1_width, data_pretty.ljust(col2_width), data_pretty_.ljust(col3_width)))
	print('%s%s%s' % (' '*col1_width, ('-'*len(data_pretty)).ljust(col2_width), ('-'*len(data_pretty)).ljust(col3_width)))

	addresses = [0, 0xAAAAAAA0, 0xAAAAAAA4, 0xAAAAAAA8, 0xAAAAAAAC, 0xAAAAAAB0]
	for addr in addresses:
		for lib in sorted(libs):
			print(lib.ljust(col1_width), end='', flush=True)
			print('%X: %s' % (addr, disasm(lib, data_bytes, addr).rstrip().ljust(col2_width)), end='')
			print('%X: %s' % (addr, disasm(lib, data_bytes_, addr).rstrip()))
		if len(addresses)>1: print('')
