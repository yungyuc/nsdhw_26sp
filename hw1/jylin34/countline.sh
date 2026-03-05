#!/bin/bash 

if [[ ${#} < 1 ]] ; then
    echo -e "missing file name"
elif [[ ${#} > 1 ]] ; then
    echo -e "only one argument is allowed"
else
    fname=${fname:-${1}}
    if [[ -f "${fname}" ]] ; then
        lines=$(wc -l < "${fname}" | xargs)
        echo -e "${lines} lines in ${fname}"
    else
        echo -e "${fname} not found"
    fi
fi
