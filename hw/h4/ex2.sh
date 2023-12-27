#!/bin/bash

# Specify the file name
FILE="numbers.txt"

# Check if the file exists
if [[ ! -f $FILE ]]; then
    # If the file doesn't exist, create it with the number 1
    echo "1" > $FILE
else
    # Read the last number in the file
    LAST_NUMBER=$(tail -n 1 $FILE)
    
    # Increment the last number
    NEXT_NUMBER=$((LAST_NUMBER + 1))
    
    # Append the result to the file
    echo $NEXT_NUMBER >> $FILE
fi
