#!/bin/bash
for i in {1..20}
do
    ./ex2_fix.sh; ./ex2_fix.sh&
done
