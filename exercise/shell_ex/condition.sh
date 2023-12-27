#!/usr/bin/bash
a=19
b=20
if [[ a -lt b ]]; then
    echo "a < b"
else
    echo "a > b"
fi

if [[ !(true && false) ]]; then
    echo "true"
else
    echo "false"
fi

if [ $a -eq $b ]; then
    echo "a = b"
elif [[ a -lt b ]]; then
    echo "a < b"
else
    echo "a > b"
fi
