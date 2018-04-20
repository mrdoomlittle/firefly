../ffc -i test.ff -o test.s
../as/as -f ffef -i test.s -o test.o
../bond/bond -s "test.o" -d a.out
../exec/exec a.out
