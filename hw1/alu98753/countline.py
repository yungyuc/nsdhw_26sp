#!/bin/sh
""":"
PYTHON_CMD=${PYTHON_BIN:-python3}
exec "$PYTHON_CMD" "$0" "$@"
" """

import sys

if len(sys.argv) < 2:
    sys.stdout.write('missing file name\n')

fname = sys.argv[1]
# Read in binary mode to avoid issues with different newline conventions
with open(fname, 'rb') as f:
    count = f.read().count(b'\n')
sys.stdout.write('{} {}\n'.format(count, fname))
