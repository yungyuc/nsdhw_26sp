#!/usr/bin/env bash

for f in "$@"; do
    count=$(wc -l < "$f")
    echo "$count $f"
done