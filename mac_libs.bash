#!/bin/bash
ROOT_DIR=${ROOT_DIR}
#cd termio; make; cd ../;
cd intlen; make ARC64 MDLINT_INC=${ROOT_DIR}/mdlint/inc; cd ../;
#cd nibbles; make MDLINT_INC=${ROOT_DIR}/mdlint/inc; cd ../;
cd getdigit; make ARC64 MDLINT_INC=${ROOT_DIR}/mdlint/inc INTLEN_INC=${ROOT_DIR}/intlen/inc; cd ../;
cd to_string; make MDLINT_INC=${ROOT_DIR}/mdlint/inc GETDIGIT_INC=${ROOT_DIR}/getdigit/inc INTLEN_INC=${ROOT_DIR}/intlen/inc; cd ../;

SC_DEFINES=-D__FFLY_ENGINE
if [ $(bash find.bash "$FFLY_ARGS" "--debug-enabled") = "0" ]; then
	SC_DEFINES="$SC_DEFINES -D__DEBUG_ENABLED"
fi

if [ $(bash find.bash "$FFLY_ARGS" "--with-mem-tracker") = "0" ]; then
	SC_DEFINES="$SC_DEFINES -D__WITH_MEM_TRACKER"
fi

cd str_cmb; make MDLINT_INC=${ROOT_DIR}/mdlint/inc ARC=${ARC} DEFINES="$SC_DEFINES" I_FLAGS=-I${ROOT_DIR}/inc; cd ../;

#cd tagged_memory; make LIB_PATH=${ROOT_DIR}/; cd ../;
cd emu2d; make ARC=${ARC} MDLINT_INC=${ROOT_DIR}/mdlint/inc; cd ../;
cd emu3d; make ARC=${ARC} MDLINT_INC=${ROOT_DIR}/mdlint/inc; cd ../;

if ! [ -d ${ROOT_DIR}/inc/mdl ]; then
	mkdir ${ROOT_DIR}/inc/mdl
fi

cp ${ROOT_DIR}/mdlint/inc/*.h* ${ROOT_DIR}/inc/mdl 2>/dev/null
cp ${ROOT_DIR}/getdigit/inc/mdl/*.h* ${ROOT_DIR}/inc/mdl 2>/dev/null
cp ${ROOT_DIR}/intlen/inc/mdl/*.h* ${ROOT_DIR}/inc/mdl 2>/dev/null
cp ${ROOT_DIR}/to_string/inc/mdl/*.h* ${ROOT_DIR}/inc/mdl 2>/dev/null
cp ${ROOT_DIR}/str_cmb/inc/mdl/*.h ${ROOT_DIR}/inc/mdl 2>/dev/null
cp ${ROOT_DIR}/serializer/inc/*.h* ${ROOT_DIR}/inc 2>/dev/null
