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

	// create objects
	ffly_objp obj0 = ffly_obj_alloc(&err);
	ffly_objp obj1 = ffly_obj_alloc(&err);
	ffly_obj_prepare(obj0);
	ffly_obj_prepare(obj1);

	// give objects a physical body
	obj0->phy_body = ffly_physical_body(&obj0->x, &obj0->y, &obj0->z);
	obj1->phy_body = ffly_physical_body(&obj1->x, &obj1->y, &obj1->z);

	ffly_phy_bodyp body0 = ffly_get_phy_body(obj0->phy_body);
	ffly_phy_bodyp body1 = ffly_get_phy_body(obj1->phy_body);

	ffly_uni_attach_body(&uni, body1);

	ff_byte_t *texture = (ff_byte_t*)__ffly_mem_alloc(20*20*4);
	ffly_mem_set(texture, 0xff, 20*20*4);

	ffly_bzero(&body0->shape, sizeof(ffly_polygon));
	ffly_obj_vertex(body0, -10, -10, 0);
	ffly_obj_vertex(body0, 10, -10, 0);
	ffly_obj_vertex(body0, 10, 10, 0);
	body0->texture = texture;
	body0->xl = 20;
	body0->yl = 20;
	body0->zl = 1;

	obj0->x = 0;
	obj0->y = 50;
	obj0->z = 0;

	ffly_bzero(&body1->shape, sizeof(ffly_polygon));
	ffly_obj_vertex(body1, -10, -10, 0);
	ffly_obj_vertex(body1, 10, -10, 0);
	ffly_obj_vertex(body1, 10, 10, 0);
	body1->texture = texture;
	body1->xl = 20;
	body1->yl = 20;
	body1->zl = 1;

	obj1->x = 50;
	obj1->y = 50;
	obj1->z = 0;

	// set velocity of object to ...
	ffly_set_velocity(obj0->phy_body, 0.5);
	ffly_set_velocity(obj1->phy_body, 0.5);

	// set direction to a1 or right
	ffly_set_direction(obj0->phy_body, _ff_dir_a1);
	ffly_set_direction(obj1->phy_body, _ff_dir_a1);

	ffly_set_angle(obj0->phy_body, 0.1);
	ffly_set_angle(obj1->phy_body, 0.1);

	ffly_set_mass(obj0->phy_body, 10);

	ff_uint_t delta = 0, start = 0;
	while(1) {
		delta = clock-start;
		start = clock;
		char c;
		if (read(fd, &c, 1) < 0) goto _sk;
		if (c == '!')
			goto _end;
	_sk:
		if (obj0->x == 50)
			ffly_set_direction(obj0->phy_body, _ff_dir_a3);
		else if (!obj0->x)
			ffly_set_direction(obj0->phy_body, _ff_dir_a1);
		ffly_obj_handle(&uni, delta, obj0);
		ffly_printf("\e[2J-------------- x: %u:%u, y: %u:%u ------------- memusage: %u, clock: %u\n",
			obj0->x, obj1->x, obj0->y, obj1->y, ffly_mem_alloc_bc-ffly_mem_free_bc, clock);
		ffly_camera_handle(&camera);
		ffly_camera_draw(&camera, ffly_frame(__frame_buff__), WIDTH, HEIGHT, 0, 0);
		ffly_pipe_wr8l(0x0, pipe);
		ffly_pipe_write(ffly_frame(__frame_buff__), (WIDTH*HEIGHT)*4, pipe);
		ffly_nanosleep(0, 100000000);
		ffly_clock_tick();
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
