#!/bin/bash

#
# This script is to turn countline.py into shell script.
#

if [ $# -ne 1 ] ; then
    echo -e "missing file name"
elif [ $# -gt 1 ] ; then
    echo -e "only one argument is allowed"
else
    if [ -f $1 ] ; then
        echo -e "$(wc -l < "$1") lines in "$1""
    else
        echo -e "$1 not found"
    fi
fi