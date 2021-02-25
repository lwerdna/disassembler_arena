#!/usr/bin/env python3

import os
import re
import sys
import struct

from subprocess import Popen, PIPE

from ctypes import *
def disasm_old(sopath, data, addr=0):
	# initialize disassembler, if necessary
	dll = CDLL(sopath)
	cbuf = create_string_buffer(256)

	pfunc = dll.disassemble
	pfunc.restype = c_int
	pfunc.argtypes = [c_uint64, c_char_p, c_uint, c_void_p]
	rv = dll.disassemble(addr, data, len(data), byref(cbuf))
	if rv != 0:
		tmp = '(error)'
	else:
		tmp = cbuf.value.decode('utf-8')
		tmp = tmp.strip()
		tmp = tmp.replace('\t', ' ')
	return tmp

def disasm(sopath, data, addr=0):
	cmdline = ['call_so', sopath, hex(addr)]
	for byte in data:
		cmdline.append(hex(byte))
	process = Popen(cmdline, stdout=PIPE, stderr=PIPE)
	(stdout, stderr) = process.communicate()
	stdout = stdout.decode("utf-8")
	stderr = stderr.decode("utf-8")
	process.wait()

	result = stderr
	if stdout and not stdout.isspace():
		result = stdout	

	result = result.strip()
	result = re.sub(r'\s+', ' ', result)
	return result

if __name__ == '__main__':
	if not sys.argv[1:]:
		print('call all disassembly .so\'s')
		print('usage:')
		print('\t%s <address> <bytes>' % sys.argv[0])
		print('examples:')
		print('\t%s 80000000 1f 20 03 d5' % sys.argv[0])
		sys.exit(-1)

	addr = int(sys.argv[1], 16)

	data = ''
	data_ints = list(map(lambda x: int(x,16), sys.argv[2:]))
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

	for lib in sorted(libs):
		print(lib.ljust(col1_width), end='', flush=True)
		print('%X: %s' % (addr, disasm(lib, data_bytes, addr).rstrip().ljust(col2_width)), end='')
		print('%X: %s' % (addr, disasm(lib, data_bytes_, addr).rstrip()))
