make all

no_errors=0
clear
echo "------------ mem_cpy ------------"
./mem_cpy

if [ $? -eq -1 ]; then
	no_errors=$(($no_errors+1))
fi

echo "\n------------ mem_dupe ------------"
./mem_dupe

if [ $? -eq -1 ]; then
	no_errors=$(($no_errors+1))
fi

echo "\n------------ mem_cmp ------------"
./mem_cmp

if [ $? -eq -1 ]; then
	no_errors=$(($no_errors+1))
fi

echo "\n------------ find ------------"
./find

if [ $? -eq -1 ]; then
	no_errors=$(($no_errors+1))
fi

echo "\n------------ vec:0 ------------"
./vec.0

if [ $? -eq -1 ]; then
	no_errors=$(($no_errors+1))
fi

echo "\n------------ vec:1 ------------"
./vec.1

if [ $? -eq -1 ]; then
	no_errors=$(($no_errors+1))
fi

echo "\n------------ set ------------"
./set

if [ $? -eq -1 ]; then
	no_errors=$(($no_errors+1))
fi

echo "error/s $no_errors"
