#!/usr/bin/bash
array=( 1 2 3 4 5 )
echo ${array[array[0]]}
A=1
array=(A A A)
echo ${array[A]}

for i in ${array[@]}:
