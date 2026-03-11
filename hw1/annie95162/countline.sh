#!/bin/bash

if [ $# -lt 1 ]; then
    echo "missing file name"
    exit 0
fi

if [ $# -gt 1 ]; then
    echo "only one argument is allowed"
    exit 0
fi

fname=$1

if [ ! -e "$fname" ]; then
    echo "$fname not found"
    exit 0
fi

lines=$(wc -l < "$fname")
lines=$(echo $lines)
echo "$lines lines in $fname"