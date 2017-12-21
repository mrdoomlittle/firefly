make all

no_errors=0
clear
echo "------------ test:0 ------------"
./mem_cpy

if [ $? -eq -1 ]; then
	no_errors=$(($no_errors+1))
fi

echo "\n------------ test:1 ------------"
./mem_dupe

if [ $? -eq -1 ]; then
	no_errors=$(($no_errors+1))
fi

echo "\n------------ test:2 ------------"
./mem_cmp

if [ $? -eq -1 ]; then
	no_errors=$(($no_errors+1))
fi

echo "\n------------ test:3 ------------"
./find

if [ $? -eq -1 ]; then
	no_errors=$(($no_errors+1))
fi

echo "\n------------ test:4 ------------"
./vec

if [ $? -eq -1 ]; then
	no_errors=$(($no_errors+1))
fi

echo "\n------------ test:5 ------------"
./set

if [ $? -eq -1 ]; then
	no_errors=$(($no_errors+1))
fi

echo "error/s $no_errors"
