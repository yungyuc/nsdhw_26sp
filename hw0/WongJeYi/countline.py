#!/usr/bin/env python3
import sys

# Count lines and print the result
if len(sys.argv) > 1:
    try:
        with open(sys.argv[1], 'r') as f:
            print(sum(1 for _ in f))
    except FileNotFoundError:
        sys.exit(1)
