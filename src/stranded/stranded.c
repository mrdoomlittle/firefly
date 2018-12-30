# include "stranded.h"

#define WIDTH 448
#define HEIGHT 448
# include "../duct.h"
# include "../engine/loop.h"
# include "../engine/entry.h"
# include "../engine/view.h"
# include "../engine/init.h"
# include "../event.h"
# include "../system/pipe.h"
# include "../graphics/chamber.h"
# include "../carriage.h"
# include "../context.h"
# include "../pixel.h"
# include "../texture.h"
# include "../model.h"
# include "../hs.h"
# include "../hs/exec.h"
# include "../obj.h"
# include "../physics/body.h"
# include "../physics/direction.h"
# include "../physics/clock.h"
# include "../graphics/fill.h"
# include "../graphics/colour.h"
# include "../ui/camera.h"
# include "../memory/plate.h"
# include "../system/sched.h"
# include "../storage/reservoir.h"
# include "../gravity.h"
# include "../malloc.h"
ff_dcp static dc;
static struct ffly_camera cam;
static struct ffly_uni uni;

static ff_uint_t delta, start;
static ff_int_t old_x, old_y;
static ffly_objp obj0, obj1;
static ffly_modelp m0, m1;
ff_u8_t static *tex;
ff_i8_t static _init(void) {
//	ffly_scheduler_init(SCHED_CORRODE);
	ffly_reservoir_init(&__ffly_reservoir__, 0, "test.resv");

	__ffly__chamber__ = ffly_chamber_new();

	dc = ff_duct(0, FF_PIPE_CREAT);
	duct(dc, open);
	duct(dc, listen);

	_queue_loadfuncs(&__ctx(event_queue));
	_queue_init(&__ctx(event_queue), sizeof(ff_eventp));


	ffly_camera_init(&cam, WIDTH, HEIGHT);
	ffly_uni_build(&uni, _ffly_uni_1024, _ffly_uni_1024, 4, 4, _ffly_lotsize_8);

	ffly_gravity_init(_ffly_uni_1024, _ffly_uni_1024, 4);

	ffly_camera_bind(&cam, &uni);

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

	body1->xl = 20;
	body1->yl = 20;
	body1->zl = 1;

	obj1->x = 40;
	obj1->y = 100;
	obj1->z = 0;

	tex = (ff_u8_t*)malloc(8);
	setpix(tex, 255, 0, 0, 255);
	setpix(tex+4, 0, 255, 0, 255);
	struct ffly_mpvec tx0[] = {
		{tex, 1},
		{tex+4, 1}
	};

	struct ffly_mpvec tx1[] = {
		{tex, 1},
		{tex+4, 1}
	};

	ffly_model_tex(m0, tx0);
	ffly_model_tex(m1, tx1);

	ffly_uni_attach_body(&uni, body0);
	ffly_uni_attach_body(&uni, body1);

	// set velocity of object to ...
	ffly_set_velocity(obj0->phy_body, 1);
	ffly_set_velocity(obj1->phy_body, 0);

	// set direction to a1 ka right
	ffly_set_direction(obj0->phy_body, _ff_dir_a2);
	ffly_set_direction(obj1->phy_body, _ff_dir_a2);

	ffly_set_angular_velocity(obj0->phy_body, 0.0);
	ffly_set_angular_velocity(obj1->phy_body, 0.0);

	ffly_set_mass(obj0->phy_body, 0);
	ffly_set_mass(obj1->phy_body, 0);

	delta = 0;
	start = phy_clock;
}

ff_i8_t static _de_init(void) {
	ffly_queue_de_init(&__ctx(event_queue));
	duct(dc, close);
	ff_event_cleanup();
	ffly_carriage_cleanup();

	ffly_model_dismantle(m0);
	ffly_model_dismantle(m1);
	ffly_chunk_cleanup();
	ffly_obj_cleanup();
	ffly_lot_cleanup();
	ffly_uni_free(&uni);
	ffly_body_cleanup();
	ffly_camera_de_init(&cam);
	ffly_plate_cleanup();
	ffly_gravity_cleanup();
	ffly_reservoir_de_init(&__ffly_reservoir__);
	free(tex);
}

ff_i8_t static _tick(void) {
	ffly_pixfill(WIDTH*HEIGHT, ffly_colour(255, 255, 255, 255), 0);
	delta = phy_clock-start;
	start = phy_clock;

	old_x = obj0->x;
	old_y = obj0->y;
	if (obj0->y >= 128) {
		ffly_set_direction(obj0->phy_body, _ff_dir_a0);
	} else if (!obj0->y)
		ffly_set_direction(obj0->phy_body, _ff_dir_a2);
	ffly_uni_update(&uni, delta);
	ffly_obj_handle(&uni, delta, obj0);
	ffly_obj_handle(&uni, delta, obj1);
	ffly_phy_clock_tick();
	ffly_camera_draw(&cam, WIDTH, HEIGHT, 0, 0);	
	ffly_chamber_run(0);
	if (!ff_duct_serve(dc))
		return -1;
	return 0;
}

void ffe_entry(void) {
	ffe_iflgs ^= FF_E_GR;
	ffe_init = _init;
	ffe_de_init = _de_init;
	ffe_tick = _tick;


	setframesize(WIDTH, HEIGHT);
}
