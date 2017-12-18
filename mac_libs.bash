#!/bin/bash
#cd termio; make; cd ../;
cd intlen; make arc64 mdlint_inc_dir=${ffly_root_dir}/mdlint/inc; cd ../;
#cd nibbles; make MDLINT_INC=${ffly_root_dir}/mdlint/inc; cd ../;
cd getdigit; make arc64 mdlint_inc_dir=${ffly_root_dir}/mdlint/inc intlen_inc_dir=${ffly_root_dir}/intlen/inc intlen_lib_dir=${ffly_root_dir}/intlen/lib; cd ../;
cd to_string; make mdlint_inc_dir=${ffly_root_dir}/mdlint/inc getdigit_inc_dir=${ffly_root_dir}/getdigit/inc getdigit_lib_dir=${ffly_root_dir}/getdigit/lib intlen_inc_dir=${ffly_root_dir}/intlen/inc intlen_lib_dir=${ffly_root_dir}/intlen/lib; cd ../;

stc_defines=-D__ffly_engine
if [ $ffly_debug_enabled -eq 1 ]; then
	sc_defines="$stc_defines -D__ffly_debug_enabled"
fi

if [ $ffly_mal_track -eq 1 ]; then
	sc_defines="$stc_defines -D__ffly_mal_track"
fi

cd str_cmb; make mdlint_inc_dir=${ffly_root_dir}/mdlint/inc arc=${ffly_arc} defines="$stc_defines" inc_flags=-I${ffly_root_dir}/inc; cd ../;

#cd tagged_memory; make LIB_PATH=${ffly_root_dir}/; cd ../;
cd emu2d; make ARC=${ffly_arc} MDLINT_INC=${ffly_root_dir}/mdlint/inc; cd ../;
cd emu3d; make ARC=${ffly_arc} MDLINT_INC=${ffly_root_dir}/mdlint/inc; cd ../;

if ! [ -d ${ffly_root_dir}/inc/mdl ]; then
	mkdir ${ffly_root_dir}/inc/mdl
fi

cp ${ffly_root_dir}/mdlint/inc/*.h* ${ffly_root_dir}/inc/mdl 2>/dev/null
cp ${ffly_root_dir}/getdigit/inc/mdl/*.h* ${ffly_root_dir}/inc/mdl 2>/dev/null
cp ${ffly_root_dir}/intlen/inc/mdl/*.h* ${ffly_root_dir}/inc/mdl 2>/dev/null
cp ${ffly_root_dir}/to_string/inc/mdl/*.h* ${ffly_root_dir}/inc/mdl 2>/dev/null
cp ${ffly_root_dir}/str_cmb/inc/mdl/*.h ${ffly_root_dir}/inc/mdl 2>/dev/null
cp ${ffly_root_dir}/serializer/inc/*.h* ${ffly_root_dir}/inc 2>/dev/null
