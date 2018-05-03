#!/bin/bash
. $ffly_root_dir/config.bash
USE_CP=1
i=0
while [[ i -lt ${#dirs[@]} ]]; do
	dst_dir=$ffly_root_dir/inc/firefly/${dirs[i]}
	src_dir=$ffly_root_dir/src/${dirs[i]}
	for e in ${header_ext[@]}; do
		if [ $USE_CP -eq 1 ]; then
			cp $src_dir/*.$e $dst_dir 2> /dev/null
		else
			files=($(find $src_dir -maxdepth 1 -name "*.$e" | sed -e 's/^\.\///' | tr '\n' ' '))
			for f in ${files[@]}; do
				cp $f $dst_dir
			done
		fi
	done
	i=$(($i+1))
done
