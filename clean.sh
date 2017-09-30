cd nibbles; make clean; cd ../;
cd termio; make clean; cd ../;
cd intlen; make clean; cd ../;
cd getdigit; make clean; cd ../;
cd to_string; make clean; cd ../;
cd str_cmb; make clean; cd ../;
cd tagged_memory; make clean; cd ../;

rm -f src/*.o src/maths/*.o src/data/*.o src/system/*.o src/audio/*.o src/ui/*.o src/gui/*.o src/memory/*.o src/memory/*.co src/graphics/*.o src/graphics/*.clo src/networking/*.o
rm -rf inc/* lib/*
