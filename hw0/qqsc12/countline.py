#!/usr/bin/env python3
import fileinput
lines = [line for line in fileinput.input()]
print(len(lines))
