#!/bin/bash
python_cmd=${PYTHON_BIN:-python3}
if ! command -v "$python_cmd" >/dev/null 2>&1; then
    echo "Error: Specified Python binary '$python_cmd' not found." >&2
   exit 1
fi
_PY_CODE=$(cat << 'PYEOF'
import sys
import fileinput
count = 0
for _ in fileinput.input():
    count += 1
sys.stdout.write(str(count) + '\n')
PYEOF
)
exec "$python_cmd" -c "$_PY_CODE" "$@"
