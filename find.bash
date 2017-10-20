#!/bin/bash
lookup_str=$1
to_find_str=$2

lu_str_len=
tf_str_len=
if [ "$(command -v expr)" = "" ]; then
	if ! [ -f bin/len ]; then
		gcc -o bin/len len.c
	fi

	lu_str_len=$(bin/len "$lookup_str")
	tf_str_len=$(bin/len "$to_find_str")
else
	lu_str_len=$(expr length "$lookup_str")
	tf_str_len=$(expr length "$to_find_str")
fi

for ((i = 0; i != $lu_str_len; i++)); do
	lu_chr=${lookup_str:$i:1}
	plen=0
	part=""
	while true; do
		if [ "$lu_chr" = ' ' ] || [ $i -ge $lu_str_len ]; then
			if [ $i -gt $lu_str_len ]; then
				echo -ne "1"
				exit
			fi
			break
		fi
		part=$part$lu_chr
		plen=$(($plen+1))
		i=$(($i+1))
		lu_chr=${lookup_str:$i:1}
	done

#	echo "$part"
#	echo "$plen"
	loc=0
	while [[ $loc -lt $plen ]]; do
		a_chr=${part:$loc:1}
		b_chr=${to_find_str:$loc:1}

		if [ "$b_chr" == '*' ]; then
			if [[ "$a_chr" = [a-zA-Z] ]]; then
				break
			fi

			dif=$(($plen-$loc))

			echo "${part:((loc+1)):dif}"
			exit
		fi

		if ! [ "$a_chr" = "$b_chr" ]; then
			break
		fi

		loc=$(($loc+1))
	done

	if [ $loc -eq $tf_str_len ]; then
		echo -ne "0"
		exit
	fi
done
echo -ne "1"
