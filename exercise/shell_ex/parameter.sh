#!/usr/bin/bash
echo $0
echo $1
echo $2
echo $3
echo $#
echo $*
echo "$*"
echo $@
echo "$@"

declare -A site
site["google"]="www.google.com"
site["runoob"]="www.runoob.com"
site["taobao"]="www.taobao.com"

echo "数组的键为: ${!site[*]}"
echo "数组的键为: ${!site[@]}"
