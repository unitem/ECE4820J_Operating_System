#!/usr/bin/bash
myschool="sjtu"
myname="xwq"
echo sjtu_xwq
echo "sjtu_xwq"
: '
echo "sjtu"_"xwq"
echo $myschool"_"$myname
echo "$myschool"_"$myname"
echo "${myschool}_${myname}"
echo "sjtu""_""xwq"
echo ${#myschool}
echo ${#myname[0]}
'
myid="sjtu_xwq:github"
echo ${myid:4:5}
a=5
b=6
echo ${myid:$a:$b}
echo `expr index "$myid" x`
a=`expr index "$myid" _x`
echo $a
