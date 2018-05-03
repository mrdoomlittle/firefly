# include "ffint.h"
# include "types.h"
# include "graphics.h"
# include "system/pipe.h"
# include "linux/unistd.h"
# include "linux/fcntl.h"
# include "linux/time.h"
# include "linux/input-event-codes.h"
# include "linux/input.h"
# include "dep/mem_set.h"
# include "ui/camera.h"
# include "obj.h"
# include "physics/body.h"
# include "physics/direction.h"
# include "system/nanosleep.h"
# include "graphics/frame_buff.h"
# define __ffly_debug_enabled
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "dep/bzero.h"
# include "physics/clock.h"
# define WIDTH 400
# define HEIGHT 400

/*
	only for testing
*/

ff_uint_t pipe;
ff_err_t ffmain(int __argc, char const *__argv[]) {
	ff_err_t err;

	int fd;
	/*
		control
	*/
	fd = open("input", O_RDONLY|O_NONBLOCK|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);

	if (fd == -1)
		return -1;

	ff_set_frame_size(WIDTH, HEIGHT);
	ff_graphics_init();

	pipe = ffly_pipe(8, FF_PIPE_SHMM, 0, &err);

	// clear frame buffer
	ffly_mem_set(ffly_frame(__frame_buff__), 0, (WIDTH*HEIGHT)*4);

	ffly_pipe_connect(pipe);

	struct ffly_camera camera;
	ffly_camera_init(&camera, 100, 100);

	/*
		clear camera memory
	*/
	ffly_mem_set(camera.pixels, 0, 100*100*4);

	struct ffly_uni uni;

	/*
		build universe
	*/
	ffly_uni_build(&uni, _ffly_uni_128, _ffly_uni_128, _ffly_uni_64, 3, _ffly_lotsize_8);

	// bind camera to universe
	ffly_camera_bind(&camera, &uni);

	// create object
	ffly_objp obj = ffly_obj_alloc(&err);
	ffly_obj_prepare(obj);

	// give object a physical body
	obj->phy_body = ffly_physical_body(&obj->puppet);

	ff_byte_t *texture = (ff_byte_t*)__ffly_mem_alloc(20*20*4);
	ffly_mem_set(texture, 0xff, 20*20*4);

	ffly_bzero(&obj->shape, sizeof(ffly_polygon));
	ffly_obj_vertex(obj, -10, -10, 0);
	ffly_obj_vertex(obj, 10, -10, 0);
	ffly_obj_vertex(obj, 10, 10, 0);
	obj->texture = texture;
	obj->xl = 20;
	obj->yl = 20;
	obj->zl = 1;
	obj->x = 0;
	obj->y = 50;
	obj->z = 0;

	// set velocity of object to 1
	ffly_set_velocity(obj->phy_body, 0.5);

	// set direction to a1 or right
	ffly_set_direction(obj->phy_body, _ff_dir_a1);

	ffly_set_angle(obj->phy_body, -0.1);

	while(1) {
		char c;
		if (read(fd, &c, 1) < 0) goto _sk;
		if (c == '!')
			goto _end;
	_sk:
		if (obj->x == 50)
			ffly_set_direction(obj->phy_body, _ff_dir_a3);
		else if (!obj->x)
			ffly_set_direction(obj->phy_body, _ff_dir_a1);
		ffly_obj_handle(&uni, obj);
		ffly_printf("\e[2J-------------- x: %u, y: %u ------------- memusage: %u\n",
			obj->x, obj->y, ffly_mem_alloc_bc-ffly_mem_free_bc);
		ffly_camera_handle(&camera);
		ffly_camera_draw(&camera, ffly_frame(__frame_buff__), WIDTH, HEIGHT, 0, 0);
		ffly_pipe_wr8l(0x0, pipe);
		ffly_pipe_write(ffly_frame(__frame_buff__), (WIDTH*HEIGHT)*4, pipe);
		ffly_nanosleep(0, 100000000);
		//ffly_clock_tick();
	}	
_end:
	__ffly_mem_free(texture);
	ffly_pipe_wr8l(0x1, pipe);
	close(fd);
	ffly_pipe_close(pipe);
	ff_graphics_de_init();
	ffly_chunk_cleanup();
	ffly_obj_cleanup();
	ffly_lot_cleanup();
	ffly_uni_free(&uni);
	ffly_camera_de_init(&camera);
}
