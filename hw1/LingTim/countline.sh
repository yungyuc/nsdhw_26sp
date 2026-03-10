#!/bin/bash

if [ "$#" -eq 0 ]; then
    echo "missing file name"
elif [ "$#" -gt 1 ]; then
    echo "only one argument is allowed"
else
    fname="$1"
    if [ -e "$fname" ]; then
        lines=$(wc -l < "$fname"
        echo "$lines lines in $fname"
    else
        echo "$fname not found"
    fi
fi
