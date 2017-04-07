#!/bin/bash
lookup_str=$1
to_find=$2

lu_str_len=
tf_str_len=

if [ "$(command -v expr)" = "" ]; then
	if ! [ -f bin/len ]; then
		gcc -std=c11 -o bin/len len.c
	fi

	lu_str_len=$(bin/len "$lookup_str")
	tf_str_len=$(bin/len "$to_find")
else
	lu_str_len=$(expr length "$lookup_str")
	tf_str_len=$(expr length "$to_find")
fi

tf_point=0
match_c=0
lu_chr=' '
tf_chr=' '
nxt_chr=' '
for ((o = 0; o != $lu_str_len; o++))
do
	if [ $tf_point -eq $tf_str_len ] && [ $match_c -ne $tf_str_len ]; then
		tf_point=0
		match_c=0
		for ((i = $o; i != $lu_str_len; i++))
		do
			lu_chr=${lookup_str:$i:1}
			if [ "$lu_chr" == ' ' ]; then
				o=$((i + 1))
				break
			fi
		done
	fi

	lu_chr=${lookup_str:$o:1}
	if [ "$lu_chr" = ' ' ]; then
		tf_point=0
		match_c=0
		continue
	fi

	tf_chr=${to_find:$tf_point:1}

	if [ "$lu_chr" = "$tf_chr" ]; then
		match_c=$(($match_c + 1))
	fi

	if [ $o -ne $(($lu_str_len - 1)) ]; then
		nxt_chr=${lookup_str:$(($o + 1)):1}
	fi

	#echo "$match_c = $tf_str_len | $nxt_chr "
	if [ $match_c = $tf_str_len ] && ([ "$nxt_chr" = ' ' ] || [ $o -eq $(($lu_str_len - 1)) ]); then
		echo 0
		exit
	fi

	if [ $match_c = $tf_str_len ]; then
		tf_point=0
		match_c=0
		continue
	fi

	tf_point=$(($tf_point + 1))
done
echo 1

