make mem_ncpy
make mem_dupe
make mem_cmp
make find


clear
echo "------------ test:0 ------------"
./mem_ncpy

echo "\n------------ test:1 ------------"
./mem_dupe

echo "\n------------ test:2 ------------"
./mem_cmp

echo "\n------------ test:3 ------------"
./find
