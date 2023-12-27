#!/usr/bin/bash
var=http://www.aaa.com/123.htm
echo ${var:4:3}
echo ${var#*//}
echo ${var##*/}
echo ${var##*m}
echo ${var:4}
echo ${var:0-1}
echo ${var:0-4:3}
echo `expr length "$var"`