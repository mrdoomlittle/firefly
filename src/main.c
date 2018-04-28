# include "ffint.h"
# include "types.h"
# include "uni.h"
# include "system/io.h"
# include "obj.h"
# include "ui/camera.h"
# include "dep/mem_set.h"
# include "system/nanosleep.h"
# include "linux/unistd.h"
# include "linux/fcntl.h"
# include "linux/time.h"
# include "linux/input-event-codes.h"
# include "linux/input.h"
# define __ffly_debug_enabled
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "dep/mem_set.h"
# include "obj_manager.h"
# include "polygon.h"
# include "dep/bzero.h"
# include "maths/abs.h"
# include "maths/min.h"
# include "maths/max.h"

# define WIDTH 400
# define HEIGHT 400

# include "maths/dot.h"
# include "maths/barycentric.h"
# include "maths/pi.h"
# include "cache.h"
# include "graphics/frame_buff.h"
# include "physics/light.h"
# include "maths/round.h"
# include "system/pipe.h"
# include "obj_pool.h"
ffly_frame_buffp fb;
ff_err_t ffmain(int __argc, char const *__argv[]) {
//	ffly_cache_prepare(20);
	ff_err_t err;
	fb = ffly_frame_buff_creat(WIDTH, HEIGHT, 4, &err);
	ff_uint_t pipe = ffly_pipe(8, FF_PIPE_SHMM, 0, &err);
	ffly_mem_set(ffly_frame(fb), 0, (WIDTH*HEIGHT)*4);

	ffly_pipe_connect(pipe);
/*
	ff_uint_t radiance = (ff_uint_t)(100.0*0.8);
	ff_uint_t x, y = 200-radiance;
	while(y != 200+radiance) {
		x = 200-radiance;
		while(x != 200+radiance) {
			ff_u8_t *p = frame+((x+(y*WIDTH))*4);
			*p = 255;
			*(p+1) = 255;
			*(p+2) = 255;
			*(p+3) = 255;
			x++;
		}
		y++;
	}
	ffly_light_emit(frame, WIDTH, HEIGHT, 200, 200, 0, 100);
	ffly_pipe_connect(pipe);
	ffly_pipe_write(frame, (WIDTH*HEIGHT)*4, pipe);

	__ffly_mem_free(frame);
	ffly_pipe_close(pipe);
	return 0;
*/
	struct ffly_obj_pool pool;
	ffly_obj_pool_init(&pool, 8);

	struct ffly_obj_man obj_man;
	ffly_obj_man_prepare(&obj_man);

	struct ffly_camera camera;
	ffly_camera_init(&camera, 100, 100);
	ffly_mem_set(camera.pixels, 0, 100*100*4);
	struct ffly_uni uni;
	ffly_uni_build(&uni, _ffly_uni_128, _ffly_uni_128, _ffly_uni_64, 3, _ffly_lotsize_8);

	ffly_set_camerax(camera, 0);
	ffly_set_cameray(camera, 0);

	ffly_camera_bind(&camera, &uni);

	ff_id_t obj0 = ffly_obj_man_add(&obj_man, ffly_obj_alloc(&err));
	ff_id_t obj1 = ffly_obj_man_add(&obj_man, ffly_obj_alloc(&err));

	ffly_objp obj = ffly_obj_man_get(&obj_man, obj0);
	ffly_obj_pool_add(&pool, obj);
	obj->angle = 0.0;
	obj->light = 100;
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
	obj->x = 20;
	obj->y = 20;
	obj->z = 0;

	ffly_uni_attach_obj(&uni, obj);

/*
	obj = ffly_obj_man_get(&obj_man, obj1);
	ffly_bzero(&obj->shape, sizeof(ffly_polygon));
	ffly_obj_vertex(obj, -8, -8, 0);
	ffly_obj_vertex(obj, 8, -8, 0);
	ffly_obj_vertex(obj, 8, 8, 0);
	obj->texture = texture;
	obj->xl = 60;
	obj->yl = 60;
	obj->zl = 1;
	obj->x = 10;
	obj->y = 10;
	obj->z = 0;

	ffly_uni_attach_obj(&uni, obj);
*/

	int fd;
	if ((fd = open("input", O_RDONLY|O_NONBLOCK|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR)) == -1) {
		ffly_printf("failed to open.\n");
		return -1;
	} 

	ff_u64_t update = 200000, i = 0;

	float r = 0.0;
	ff_i8_t dir = -1;
	ff_uint_t cam_x = 0, cam_y = 0, x = ffly_obj_man_get(&obj_man, obj0)->x, y = ffly_obj_man_get(&obj_man, obj0)->y;
	while(1) {
		char c;
		if (read(fd, &c, 1) < 0) goto _sk;
		
		/*
		if (event.type == EV_KEY) {
			switch(event.code) {
				case KEY_SPACE:
				break;
				case KEY_ESC:
					goto _exit;
				break;
				case KEY_W: {
					if (y > 0) y--;
					break;
				}
				case KEY_S: {
					y++;
					break;
				}
				case KEY_A: {
					if (x > 0) x--;
					break;
				}
				case KEY_D: {
					x++;
					break;
				}
				case KEY_KP8: {
					if (cam_y > 0) cam_y--;
					break;
				}
				case KEY_KP2: {
					cam_y++;
					break;
				}
				case KEY_KP6: {
					cam_x++;
					break;
				}
				case KEY_KP4: {
					if (cam_x > 0) cam_x--;
					break;
				}
				
			}
		}*/
		
		switch(c) {
			case '!':
				goto _exit;
			case 'w':
				if (cam_y>0) cam_y--;
			break;
			case 's':
				cam_y++;
			break;
			case 'a':
				if (cam_x>0) cam_x--;
			break;
			case 'd':
				cam_x++;
			break;
		}
		ffly_set_camerax(camera, cam_x);
		ffly_set_cameray(camera, cam_y);
	_sk:

		if (x == 0)
			dir = 1;
		if (x == 20)
			dir = -1;
		x+=dir;

		if (y == 0)
			dir = 1;
		if (y == 20)
			dir = -1;
		y+=dir;
		ffly_uni_obj_move(&uni, ffly_obj_man_get(&obj_man, obj0), x, y, 0); 
		if (i > update) {
			/*
 			if (r >= 360) r = 0.0;
			ffly_obj_rotate(obj, r);
			r+= 1;
			*/
			ffly_obj_pool_handle(&pool);
			ffly_camera_handle(&camera);
			ffly_printf("\e[2J-------------- x: %u:%u, y: %u:%u ------------- memusage: %u\n", x, cam_x, y, cam_y, ffly_mem_alloc_bc-ffly_mem_free_bc);
			ffly_camera_draw(&camera, ffly_frame(fb), WIDTH, HEIGHT, 0, 0);
			ffly_pipe_wr8l(0x0, pipe);
			ffly_pipe_write(ffly_frame(fb), (WIDTH*HEIGHT)*4, pipe);

			i = 0;
		} else
			i++;
	}

_exit:
	ffly_obj_pool_de_init(&pool);
	ffly_pipe_wr8l(0x1, pipe);
	ffly_chunk_cleanup();
	ffly_obj_cleanup();
	ffly_lot_cleanup();
	ffly_uni_free(&uni);
	close(fd);
	ffly_frame_buff_del(fb);
	ffly_pipe_close(pipe);
//	ffly_cache_free();
}
