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
	arch = (sys.argv[1:] and sys.argv[1]) or 'all'

	if arch in ['aarch64', 'arm64', 'all']:
		# 1f 20 03 d5 is little-endian instruction word 0xd503201f is nop in aarch64
		assert disasm('libopcodes_aarch64.so', b'\x1f\x20\x03\xd5') == 'nop'
		assert disasm('libopcodes_aarch64_ilp32.so', b'\x1f\x20\x03\xd5') == 'nop'
		assert disasm('capstone_aarch64.so', b'\x1f\x20\x03\xd5') == 'nop'
		assert disasm('llvm_aarch64_all.so', b'\x1f\x20\x03\xd5') == 'nop'

		# this is stz2g v8.5 memory tagging feature, earlier versions should fail
		# D9FFF7FF "stz2g.."
		result = disasm('llvm_aarch64_all.so', b'\xff\xf7\xff\xd9')
		assert result == 'stz2g sp, [sp], #-0x10', f'got wrong result: {result}'

	if arch in ['thumb', 'thumb2', 'all']:
		# 00 bf is little-endian instruction word 0xbf00 is 16-bit nop in thumb
		assert disasm('binja_thumb2.so', b'\x00\xbf') == 'nop'
		assert disasm('capstone_arm_thumb.so', b'\x00\xbf') == 'nop'
		assert disasm('llvm_thumb.so', b'\x00\xbf') == 'nop'

		# AF F3 00 80 is little-endian (applied to every byte PAIR!) instruction word 0xf3af8000
		assert disasm('binja_thumb2.so', b'\xaf\xf3\x00\x80') == 'nop.w'
		assert disasm('capstone_arm_thumb.so', b'\xaf\xf3\x00\x80') == 'nop.w'
		assert disasm('llvm_thumb.so', b'\xaf\xf3\x00\x80') == 'nop.w'

	if arch in ['arm', 'all']:
		assert disasm('binja_armv7.so', b'\x00\xf0\x20\xe3') == 'nop'
		assert disasm('capstone_arm_v8.so', b'\x00\xf0\x20\xe3') == 'nop'
		assert disasm('llvm_arm_v8_all.so', b'\x00\xf0\x20\xe3') == 'nop'
		assert disasm('libopcodes_arm_unknown.so', b'\x00\xf0\x20\xe3') == 'nop {0}'

	print('passed')
