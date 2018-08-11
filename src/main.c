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
# define __ffly_debug
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "dep/bzero.h"
# include "physics/clock.h"
# include "gravity.h"
# include "duct.h"
# include "graphics/pipe.h"
# include "memory/plate.h"
# include "system/sched.h"
# include "storage/reservoir.h"
# include "physics/contact.h"
# include "linux/time.h"
# include "model.h"
# include "hs.h"
# include "hs/exec.h"
# include "texture.h"
# include "pixel.h"
# define WIDTH 448
# define HEIGHT 448
/*
	only for testing
*/
/*
char const *contact_str(ff_u8_t __loc) {
	switch(__loc) {
		case _ffly_contact_c0: return "top";
		case _ffly_contact_c1: return "right";
		case _ffly_contact_c2: return "bottom";
		case _ffly_contact_c3: return "left";
	}
	return "unknown";
}
*/

ff_i8_t contact(ff_u8_t __loca, ff_u8_t __locb, ffly_phy_bodyp __a, ffly_phy_bodyp __b) {
//	if (__loc == _ffly_contact_c2) {
//	//	ffly_set_direction(__a->id, _ff_dir_a0);
//	}
	ffly_printf("############################# body contact, nodes{%u & %u ^ %u}\n", __loca, __loca+1, __locb);
}

ff_err_t ffmain(int __argc, char const *__argv[]) {
	ffly_scheduler_init(SCHED_CORRODE);
	ffly_reservoir_init(&__ffly_reservoir__, RESV_CORRODE, "test.resv");

	ff_err_t err;
	ffly_grp_prepare(&__ffly_grp__, 20);

	int fd;
	/*
		control
	*/
	fd = open("input", O_RDONLY|O_NONBLOCK|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);

	if (fd == -1)
		return -1;

	ff_set_frame_size(WIDTH, HEIGHT);
	ff_graphics_init();

	ff_duct_open(FF_PIPE_CREAT);

	// clear frame buffer
	ffly_mem_set(ffly_frame(__frame_buff__), 0, (WIDTH*HEIGHT)*4);

	ff_duct_listen();

	struct ffly_camera camera;
	ffly_camera_init(&camera, 256, 256);

	/*
		clear camera memory
	*/
	ffly_mem_set(camera.pixels, 0, 256*256*4);

	struct ffly_uni uni;

	/*
		build universe
	*/
	ffly_uni_build(&uni, _ffly_uni_256, _ffly_uni_256, 4, 4, _ffly_lotsize_8);

	if (_err(ffly_gravity_init(_ffly_uni_256, _ffly_uni_256, 4))) {
		ffly_printf("gravity error.\n");
		return;
	}
/*
	ff_uint_t x, y = 0;
	while(y != 70) {
		x = 0;
		while(x != 70) {
			ffly_gravity_add(0.001*(x*y), x, y, 0);
			x++;
		}
		y++;
	}
*/
	// bind camera to universe
	ffly_camera_bind(&camera, &uni);

	// create objects
	ffly_objp obj0, obj1;

	// load and run hull script
	struct ff_hs h;
	ff_hs_init(&h);
	ff_hs_load(&h, "main.ffhs");
	ff_hs_objp o;
	o = ff_hs_build(&h);
	void *fins[] = {
		&obj0,
		&obj1
	};

	ff_hs_run(o, fins);
	ff_hs_clean(&h);

//	ffly_objp obj0 = ffly_obj_alloc(&err);
//	ffly_objp obj1 = ffly_obj_alloc(&err);
//	ffly_obj_prepare(obj0);
//	ffly_obj_prepare(obj1);

	// give objects a physical body
//	obj0->phy_body = ffly_physical_body(&obj0->x, &obj0->y, &obj0->z);
//	obj1->phy_body = ffly_physical_body(&obj1->x, &obj1->y, &obj1->z);

	ffly_phy_bodyp body0 = ffly_get_phy_body(obj0->phy_body);
	ffly_phy_bodyp body1 = ffly_get_phy_body(obj1->phy_body);

	ffly_pnode(body0, 0, 0, 0);
	ffly_pnode(body0, 20, 0, 0);
	ffly_pnode(body0, 0, 20, 0);
	ffly_pnode(body0, 20, 20, 0);

	ffly_pnode(body1, 0, 0, 0);
	ffly_pnode(body1, 20, 0, 0);
	ffly_pnode(body1, 0, 20, 0);
	ffly_pnode(body1, 20, 20, 0);
	ffly_texturep tex;
	tex = ffly_texture_creat(8);
	ff_byte_t *texture = ffly_texture_innards(tex);

	// red
	setpix(texture, 255, 0, 0, 255);
	setpix(texture+4, 0, 255, 0, 255);

	ffly_modelp m0, m1;

	m0 = ffly_model_new(2);
	m1 = ffly_model_new(2);

	body0->model = m0;
	ffly_body_vertex(body0, -10, -10, 0);
	ffly_body_vertex(body0, 10, -10, 0);
	ffly_body_vertex(body0, 10, 10, 0);
	ffly_model_nextpoly(m0);
	ffly_body_vertex(body0, -10, -10, 0);
	ffly_body_vertex(body0, -10, 10, 0);
	ffly_body_vertex(body0, 10, 10, 0);

	m0->texture = texture;
	body0->xl = 20;
	body0->yl = 20;
	body0->zl = 1;

	obj0->x = 40;
	obj0->y = 0;
	obj0->z = 0;

	body1->model = m1;
	ffly_body_vertex(body1, -10, -10, 0);
	ffly_body_vertex(body1, 10, -10, 0);
	ffly_body_vertex(body1, 10, 10, 0);
	ffly_model_nextpoly(m1);
	ffly_body_vertex(body1, -10, -10, 0);
	ffly_body_vertex(body1, -10, 10, 0);
	ffly_body_vertex(body1, 10, 10, 0);

	m1->texture = texture;
	body1->xl = 20;
	body1->yl = 20;
	body1->zl = 1;

	obj1->x = 40;
	obj1->y = 40;
	obj1->z = 0;

	/*
		map polys to colour
	*/
	ff_u32_t tx0[] = {
		0, 1
	};

	ff_u32_t tx1[] = {
		0, 1
	};

	ffly_model_tex(m0, 2, tx0);
	ffly_model_tex(m1, 2, tx1);
	ffly_uni_attach_body(&uni, body0);
	ffly_uni_attach_body(&uni, body1);

	// set velocity of object to ...
	ffly_set_velocity(obj0->phy_body, 1);
	ffly_set_velocity(obj1->phy_body, 0);

	// set direction to a1 or right
	ffly_set_direction(obj0->phy_body, _ff_dir_a2);
	ffly_set_direction(obj1->phy_body, _ff_dir_a2);

	ffly_set_angular_velocity(obj0->phy_body, 0.0);
	ffly_set_angular_velocity(obj1->phy_body, 0.0);

	ffly_set_mass(obj0->phy_body, 910000);
	ffly_set_mass(obj1->phy_body, 0);

	ffly_phy_contact(body0, body1, contact);

	ff_uint_t delta = 0, start = phy_clock;
	ff_int_t old_x, old_y;
	ffly_queue_init(&ffly_event_queue, sizeof(ff_eventp));
	struct timespec t0, t1;
	clock_gettime(CLOCK_MONOTONIC, &t0);

	ff_uint_t rps = 0;
	while(1) {
		clock_gettime(CLOCK_MONOTONIC, &t1);
		if (t1.tv_sec-t0.tv_sec >=1) {
			clock_gettime(CLOCK_MONOTONIC, &t0);
			ffly_printf("##### %u-RPS.\n", rps);
			rps = 0;
		}

		delta = phy_clock-start;
		start = phy_clock;
		char c;
		if (read(fd, &c, 1) < 0) goto _sk;
		if (c == '!')
			goto _end;
	_sk:
		old_x = obj0->x;
		old_y = obj0->y;
		if (obj0->y >= 100) {
			ffly_set_direction(obj0->phy_body, _ff_dir_a0);		
		} else if (!obj0->y)
			ffly_set_direction(obj0->phy_body, _ff_dir_a2);
		ffly_uni_update(&uni, delta);
		ffly_obj_handle(&uni, delta, obj0);
		ffly_obj_handle(&uni, delta, obj1);
		ffly_printf("-------------- x: %u{%d}:%u, y: %u{%d}:%u ------------- memusage: %u, clock pulse: %u\n",
			obj0->x, (ff_int_t)obj0->x-old_x, obj1->x,
			obj0->y, (ff_int_t)obj0->y-old_y, obj1->y, ffly_mem_alloc_bc-ffly_mem_free_bc, phy_clock);
		ffly_camera_handle(&camera);
		ffly_camera_draw(&camera, ffly_frame(__frame_buff__), WIDTH, HEIGHT, 0, 0);
		ffly_grp_unload(&__ffly_grp__);
		if (!ff_duct_serve())
			goto _end;
		ffly_nanosleep(0, 100000000);//800000000);
		ffly_phy_clock_tick();
		ff_eventp event;
		while(!ff_event_poll(&event)) {
			ffly_printf("got event.\n");
			ff_event_del(event);
		}
		rps++;
	}	
_end:
	ffly_sentinel_cleanup();
	ffly_model_dismantle(m0);
	ffly_model_dismantle(m1);
	ffly_queue_de_init(&ffly_event_queue);
	ff_event_cleanup();
	ffly_texture_bulldoze(tex);
	close(fd);
	ff_duct_close();
	ff_graphics_de_init();
	ffly_chunk_cleanup();
	ffly_obj_cleanup();
	ffly_lot_cleanup();
	ffly_uni_free(&uni);
	ffly_body_cleanup();
	ffly_gravity_cleanup();
	ffly_camera_de_init(&camera);
	ffly_grp_cleanup(&__ffly_grp__);
	ffly_plate_cleanup();
//	ffly_terrain_cleanup();
}
