#!/bin/bash

FILE="numbers.txt"
LOCKFILE="${FILE}.lock"

(
    # Wait for lock on the lock file (file descriptor 200). 
    # This ensures that only one instance of the script can modify numbers.txt at a time.
    flock -e 200
    
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

) 200>$LOCKFILE
