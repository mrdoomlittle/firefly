#!/bin/bash
ROOT_DIR=${ROOT_DIR}
. $ROOT_DIR/config.bash
if [ ! -d $ROOT_DIR/inc/firefly ]; then
	mkdir $ROOT_DIR/inc/firefly
fi

i=0
while [[ i -lt ${#dirs[@]} ]]; do
	dir=$ROOT_DIR/inc/firefly/${dirs[i]}
	echo "checking if '$dir' exists."
	if [ ! -d $dir ]; then
		echo "created '$dir'"
		mkdir $dir
	else
		echo "alrady exists."
	fi
	i=$((i+1))
done
