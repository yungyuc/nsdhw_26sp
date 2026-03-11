#!/bin/bash
if [ -n "$1" ]; then
    cat "$1" | wc -l | tr -d ' '
fi