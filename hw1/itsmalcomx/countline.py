#!/usr/bin/env bash
''':'
if [ -n "$PYTHON_BIN" ]; then
    if ! command -v "$PYTHON_BIN" &>/dev/null; then
        echo "Error: '$PYTHON_BIN' not found" >&2
        exit 1
    fi
    exec "$PYTHON_BIN" "$0" "$@"
else
    exec python3 "$0" "$@"
fi
':'''

import sys

for fname in sys.argv[1:]:
    with open(fname) as f:
        count = sum(1 for _ in f)
    print(count, fname)