#!/usr/bin/env python

import sys, time, string, os

code_pre = '#include <stdio.h>\nint main() {\n'
code_mid = '\tprintf("TYPE: %d\\n", sizeof(TYPE));\n'
code_post = '\treturn 0;\n}'

if len(sys.argv) > 1:
	binfilename = 'pyctypes_' + str(time.time())
	cfilename = binfilename + '.c'
	out = open(cfilename, 'w')
	out.write(code_pre)
	for type in sys.argv[1:]:
		out.write(code_mid.replace('TYPE', type))
	out.write(code_post)
	out.close()
	os.system('cc -o ' + binfilename + ' ' + cfilename)
	os.system('./' + binfilename)
	os.unlink(cfilename)
	os.unlink(binfilename)
