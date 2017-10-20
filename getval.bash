#!/bin/bash
i=0
str=$1
val=""
val_part=0
while [[ i -lt ${#str} ]]; do
	c=${str:$i:1}

	if [ $val_part -eq 1 ]; then
		val=$val$c
	else
		if [ "$c" = "=" ]; then
			val_part=1
		fi
	fi
	i=$(($i+1))
done
echo -ne "$val"
