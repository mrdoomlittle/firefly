#!/bin/bash
#cd termio; make; cd ../;
cd intlen; make arc64 mdlint_inc_dir=${ffly_root_dir}/mdlint/inc; cd ../;
#cd nibbles; make MDLINT_INC=${ffly_root_dir}/mdlint/inc; cd ../;
cd getdigit; make arc64 mdlint_inc_dir=${ffly_root_dir}/mdlint/inc intlen_inc_dir=${ffly_root_dir}/intlen/inc intlen_lib_dir=${ffly_root_dir}/intlen/lib; cd ../;
cd to_string; make mdlint_inc_dir=${ffly_root_dir}/mdlint/inc getdigit_inc_dir=${ffly_root_dir}/getdigit/inc getdigit_lib_dir=${ffly_root_dir}/getdigit/lib intlen_inc_dir=${ffly_root_dir}/intlen/inc intlen_lib_dir=${ffly_root_dir}/intlen/lib; cd ../;

sc_defines=-D__FFLY_ENGINE
if [ $ffly_debug_enabled -eq 1 ]; then
	sc_defines="$sc_defines -D__DEBUG_ENABLED"
fi

if [ $ffly_with_mem_tracker -eq 1 ]; then
	sc_defines="$sc_defines -D__WITH_MEM_TRACKER"
fi

cd str_cmb; make MDLINT_INC=${ffly_root_dir}/mdlint/inc ARC=${ffly_arc} DEFINES="$sc_defines" I_FLAGS=-I${ffly_root_dir}/inc; cd ../;

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
