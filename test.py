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
	tmp = cbuf.value.decode('utf-8').strip()
	tmp = tmp.replace('\t', ' ')
	return tmp

if __name__ == '__main__':
	# d5 03 20 1f is big-endian instruction word 0xd503201f is nop
	assert disasm('libopcodes_aarch64.so', b'\xd5\x03\x20\x1f') == 'nop'
	assert disasm('libopcodes_aarch64_ilp32.so', b'\xd5\x03\x20\x1f') == 'nop'
	assert disasm('capstone_aarch64.so', b'\xd5\x03\x20\x1f') == 'nop'
	assert disasm('llvm_armv8_all.so', b'\xd5\x03\x20\x1f') == 'nop'

	# this is stz2g v8.5 memory tagging feature, earlier versions should fail
	# D9FFF7FF "stz2g.."
	assert disasm('llvm_armv8_all.so', b'\xd9\xff\xf7\xff') == 'stz2g sp, [sp], #-16'

	print('passed')
