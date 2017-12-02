#!/bin/bash
. $ffly_root_dir/config.bash
if [ ! -d $ffly_root_dir/inc/firefly ]; then
	mkdir $ffly_root_dir/inc/firefly
fi

i=0
while [[ i -lt ${#dirs[@]} ]]; do
	dir=$ffly_root_dir/inc/firefly/${dirs[i]}
	echo "checking if '$dir' exists."
	if [ ! -d $dir ]; then
		echo "created '$dir'"
		mkdir $dir
	else
		echo "alrady exists."
	fi
	i=$((i+1))
done
