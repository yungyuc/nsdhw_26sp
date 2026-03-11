#!/bin/sh
""":"
if [ -z "$PYTHON_BIN" ]; then
	PYTHON_BIN=python3
fi

exec "$PYTHON_BIN" "$0" "$@"
" """

import sys
import os.path
if len(sys.argv) < 2:
	sys.stdout.write('missing file name\n')
elif len(sys.argv) > 2:
	sys.stdout.write('only one argument is allowed\n')
else:
	fname = sys.argv[1]
	if os.path.exists(fname):
		with open(fname) as fobj:
			count = fobj.read().count('\n')
		sys.stdout.write('{} lines in {}\n'.format(count, fname))
	else:
		sys.stdout.write('{} not found\n'.format(fname))