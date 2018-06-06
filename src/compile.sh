#!/bin/sh
ffly_cc=gcc
cc_flags="-D__ffly_debug -fno-builtin -D__ffly_no_task_pool -D__ffly_use_allocr -D__fflib -D__ffly_source"

ffly_objs=$(
	dst_dir=$PWD/dep
	root_dir=dep;
	. ./dep/compile.sh;
	echo "$ffly_objs"
)

ffly_objs="$ffly_objs $(
	dst_dir=$PWD/network
	root_dir=network
	. ./network/compile.sh
	echo "$ffly_objs"
)"

ffly_objs="$ffly_objs $(
	dst_dir=$PWD/system
	root_dir=system
	. ./system/compile.sh
	echo "$ffly_objs"
)"

ffly_objs="$ffly_objs $(
	dst_dir=$PWD/maths
	root_dir=maths
	. ./maths/compile.sh
	echo "$ffly_objs"
)"

ffly_objs="$ffly_objs $(
	dst_dir=$PWD/memory
	root_dir=memory
	. ./memory/compile.sh
	echo "$ffly_objs"
)"

ffly_objs="$ffly_objs $(
	dst_dir=$PWD/graphics
	root_dir=graphics
	. ./graphics/compile.sh
	echo "$ffly_objs"
)"

ffly_objs="$ffly_objs $(
	dst_dir=$PWD/physics
	root_dir=physics
	. ./physics/compile.sh
	echo "$ffly_objs"
)"

ffly_objs="$ffly_objs $(
	dst_dir=$PWD/data
	root_dir=data
	. ./data/compile.sh
	echo "$ffly_objs"
)"

dst_dir=$PWD
root_dir=.
$ffly_cc $cc_flags -c -o $dst_dir/gui/window.o $root_dir/gui/window.c
$ffly_cc $cc_flags -c -o $dst_dir/copy.o $root_dir/copy.c
$ffly_cc $cc_flags -c -o $dst_dir/clock.o $root_dir/clock.c
$ffly_cc $cc_flags -c -o $dst_dir/tile.o $root_dir/tile.c
$ffly_cc $cc_flags -c -o $dst_dir/pallet.o $root_dir/pallet.c
$ffly_cc $cc_flags -c -o $dst_dir/resource.o $root_dir/resource.c
$ffly_cc $cc_flags -c -o $dst_dir/cache.o $root_dir/cache.c
$ffly_cc $cc_flags -c -o $dst_dir/event.o $root_dir/event.c
$ffly_cc $cc_flags -c -o $dst_dir/db.o $root_dir/db.c
$ffly_cc $cc_flags -c -o $dst_dir/db/connect.o $root_dir/db/connect.c
$ffly_cc $cc_flags -c -o $dst_dir/stores.o $root_dir/stores.c
$ffly_cc $cc_flags -c -o $dst_dir/compactor.o $root_dir/compactor.c
$ffly_cc $cc_flags -c -o $dst_dir/terrain.o $root_dir/terrain.c
$ffly_cc $cc_flags -c -o $dst_dir/duct.o $root_dir/duct.c
$ffly_cc $cc_flags -c -o $dst_dir/gravity.o $root_dir/gravity.c
$ffly_cc $cc_flags -c -o $dst_dir/graphics.o $root_dir/graphics.c
$ffly_cc $cc_flags -c -o $dst_dir/lot.o $root_dir/lot.c
$ffly_cc $cc_flags -c -o $dst_dir/chunk_manager.o $root_dir/chunk_manager.c
$ffly_cc $cc_flags -c -o $dst_dir/uni.o $root_dir/uni.c
$ffly_cc $cc_flags -c -o $dst_dir/chunk.o $root_dir/chunk.c
$ffly_cc $cc_flags -c -o $dst_dir/obj.o $root_dir/obj.c
$ffly_cc $cc_flags -c -o $dst_dir/obj_pool.o $root_dir/obj_pool.c
$ffly_cc $cc_flags -c -o $dst_dir/obj_manager.o $root_dir/obj_manager.c
$ffly_cc $cc_flags -c -o $dst_dir/polygon.o $root_dir/polygon.c
$ffly_cc $cc_flags -D__ffly_testing -c -o $dst_dir/ui/camera.o $root_dir/ui/camera.c
$ffly_cc $cc_flags -c -o $dst_dir/rat.o $root_dir/rat.c
$ffly_cc $cc_flags -c -o $dst_dir/hatch.o $root_dir/hatch.c
$ffly_cc $cc_flags -c -o $dst_dir/signal.o $root_dir/signal.c
$ffly_cc $cc_flags -c -o $dst_dir/crypto/frazzle.o $root_dir/crypto/frazzle.c
$ffly_cc $cc_flags -c -o $dst_dir/pellet.o $root_dir/pellet.c
$ffly_cc $cc_flags -c -o $dst_dir/rdm.o $root_dir/rdm.c
$ffly_cc $cc_flags -c -o $dst_dir/depart.o $root_dir/depart.c
$ffly_cc $cc_flags -c -o $dst_dir/opt.o $root_dir/opt.c
$ffly_cc $cc_flags -c -o $dst_dir/init.o $root_dir/init.c
$ffly_cc $cc_flags -c -o $dst_dir/inet_addr.o $root_dir/inet_addr.c
$ffly_cc $cc_flags -c -o $dst_dir/in.o $root_dir/in.c
$ffly_cc $cc_flags -c -o $dst_dir/crypto.o $root_dir/crypto.c
$ffly_cc $cc_flags -c -o $dst_dir/net.o $root_dir/net.c
$ffly_cc $cc_flags -c -o $dst_dir/rand.o $root_dir/rand.c
$ffly_cc $cc_flags -c -o $dst_dir/lib.o $root_dir/lib.c
$ffly_cc $cc_flags -c -o $dst_dir/mode.o $root_dir/mode.c
$ffly_cc $cc_flags -c -o $dst_dir/config.o $root_dir/config.c
$ffly_cc $cc_flags -c -o $dst_dir/ffly_system.o $root_dir/ffly_system.c
$ffly_cc $cc_flags -c -o $dst_dir/firefly.o $root_dir/firefly.c
$ffly_cc $cc_flags -c -o $dst_dir/put_bit.o $root_dir/put_bit.c
$ffly_cc $cc_flags -c -o $dst_dir/get_bit.o $root_dir/get_bit.c
$ffly_cc $cc_flags -c -o $dst_dir/exec.o $root_dir/exec.c
$ffly_cc $cc_flags -c -o $dst_dir/resin/exec.o $root_dir/resin/exec.c
$ffly_cc $cc_flags -c -o $dst_dir/arcs.o $root_dir/arcs.c
$ffly_cc $cc_flags -c -o $dst_dir/resin.o $root_dir/resin.c
$ffly_cc $cc_flags -c -o $dst_dir/resin/mm.o $root_dir/resin/mm.c
$ffly_cc $cc_flags -c -o $dst_dir/mod/port.o $root_dir/mod/port.c
$ffly_cc $cc_flags -c -o $dst_dir/call.o $root_dir/call.c
$ffly_cc $cc_flags -c -o $dst_dir/mod.o $root_dir/mod.c
$ffly_cc $cc_flags -c -o $dst_dir/layer.o $root_dir/layer.c
$ffly_cc $cc_flags -c -o $dst_dir/gui/btn.o $root_dir/gui/btn.c
$ffly_cc $cc_flags -c -o $dst_dir/location.o $root_dir/location.c
$ffly_cc $cc_flags -c -o $dst_dir/storage/slab.o $root_dir/storage/slab.c
$ffly_cc $cc_flags -c -o $dst_dir/storage/reservoir.o $root_dir/storage/reservoir.c
$ffly_cc $cc_flags -c -o $dst_dir/piston.o $root_dir/piston.c
$ffly_cc $cc_flags -c -o $dst_dir/corrode.o $root_dir/corrode.c
$ffly_cc $cc_flags -c -o $dst_dir/bog.o $root_dir/bog.c
export ffly_objs="$dst_dir/event.o $ffly_objs $dst_dir/resin.o $dst_dir/exec.o \
$dst_dir/resin/exec.o $dst_dir/resin/mm.o $dst_dir/mode.o \
$dst_dir/firefly.o $dst_dir/ffly_system.o $dst_dir/config.o $dst_dir/put_bit.o  $dst_dir/get_bit.o \
$dst_dir/arcs.o $dst_dir/call.o $dst_dir/rand.o $dst_dir/lib.o $dst_dir/mod.o $dst_dir/mod/port.o \
$dst_dir/net.o $dst_dir/crypto.o $dst_dir/in.o $dst_dir/inet_addr.o $dst_dir/init.o \
$dst_dir/opt.o $dst_dir/depart.o $dst_dir/rdm.o $dst_dir/pellet.o $dst_dir/crypto/frazzle.o \
$dst_dir/signal.o $dst_dir/hatch.o $dst_dir/rat.o \
$dst_dir/lot.o $dst_dir/chunk_manager.o $dst_dir/uni.o $dst_dir/chunk.o \
$dst_dir/obj.o $dst_dir/obj_pool.o $dst_dir/obj_manager.o $dst_dir/polygon.o \
$dst_dir/ui/camera.o $dst_dir/graphics.o $dst_dir/gravity.o $dst_dir/duct.o \
$dst_dir/terrain.o $dst_dir/compactor.o $dst_dir/stores.o $dst_dir/db.o \
$dst_dir/db/connect.o $dst_dir/layer.o $dst_dir/gui/btn.o $dst_dir/tile.o \
$dst_dir/pallet.o $dst_dir/resource.o $dst_dir/cache.o $dst_dir/clock.o \
$dst_dir/copy.o $dst_dir/gui/window.o $dst_dir/location.o \
$dst_dir/storage/slab.o $dst_dir/storage/reservoir.o $dst_dir/piston.o $dst_dir/corrode.o $dst_dir/bog.o"
