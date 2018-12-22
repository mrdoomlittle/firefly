# define __ffly_debug_enabled
# include "types.h"
# include "dep/mem_cpy.h"
# include "system/thread.h"
# include "stdio.h"
# include "malloc.h"
# include "thread.h"
# include "mutex.h"
# include "system/nanosleep.h"
# include "ctl.h"
# include "string.h"
/*
	only for testing allocr need more woking on
*/
ffly_potp pot;
void *p = NULL;

void *thr(void *__arg_p) {
/*
	free(malloc(200));
	free(p);
	ffly_ctl(ffly_malc, _ar_setpot, (ff_u64_t)pot);
	//free(malloc(200));
	
	ffly_ctl(ffly_malc, _ar_unset, 0);
*/
ffly_printf("hello.\n");
}


# include "system/errno.h"
# include "system/nanosleep.h"
# include "linux/signal.h"
# include "linux/types.h"
# include "linux/sched.h"
# include "linux/unistd.h"
# include "linux/wait.h"
static ff_i8_t run = -1;
void sig(int __no) {
	run = 0;
}
/*
void restore();
__asm__("restore:mov $15,%rax\n\t"
		"syscall");
*/

ff_i8_t test(void *__arg_p) {
	ffly_printf("....\n");
	//ffly_nanosleep(1, 0);
	return -1;
}
# include "signal.h"
# include "maths/round.h"
# include "maths/floor.h"
# include "maths/ceil.h"
# include "maths/sin.h"
# include "maths/cos.h"
# include "system/errno.h"
# include "linux/limits.h"
# include "system/sched.h"
# include "linux/time.h"
# include "system/servant.h"
# include "system/task_pool.h"
# include "stores.h"
# include "system/tls.h"
# include "linux/prctl.h"
# include "system/queue.h"
void showfsreg() {
	void *p = NULL;
	if (arch_prctl(ARCH_GET_FS, &p) == -1) {
		ffly_printf("error, %s\n", strerror(errno));	
	}
	ffly_printf("fs: %p\n", p);
}
ff_uint_t tls_test;
# include "linux/mman.h"
# include "system/thread.h"
# include "clock.h"
# include "pallet.h"
void *tls;
void* th(void *__arg) {

}
# include "cache.h"
# include "resource.h"
# include "location.h"
# include "system/config.h"
# include "storage/reservoir.h"
# include "version.h"
# include "env.h"
# include "stdio.h"
# include "line.h"
# include "linux/termios.h"
# include "linux/time.h"
# include "linux/signal.h"
# include "signal.h"
# include "pulse.h"
# include "clock.h"
# include "brick.h"
# include "graphics/image.h"
# include "storage/cistern.h"
# include "gravity.h"
# include "uni.h"
# include "carriage.h"
# include "tools.h"
# include "bron/driver.h"
# include "bron/context.h"
# include "bron/tri.h"
# include "bron/tex.h"
# include "bron/dc.h"
# include "bron/prim.h"
# include "bron/pixel.h"
# include "bron/frame_buff.h"
# include "bron/objbuf.h"
# include "pallet.h"
# include "graphics/pipe.h"
# include "graphics/fill.h"
# include "graphics/copy.h"
# include "graphics/draw.h"
# include "graphics/frame_buff.h"
# include "graphics/mutate.h"
# include "slurry/connection.h"
# include "db/connect.h"
# include "system/util/hash.h"
void frame_read_rgb(ffly_frame_buffp __fb, void *__dst, ff_uint_t __width, ff_uint_t __height, ff_uint_t __x, ff_uint_t __y) {
	ff_u8_t buf[__width*__height*4];
	ffly_mem_set(buf, 0, __width*__height*4);
	ffly_fb_read(__fb, buf, __width, __height, __x, __y);
	ffly_rgba_to_rgb(buf, __dst, __width*__height);
	
}
# include "tc.h"
# include "nought/types.h"
# include "bron/tex.h"
# include "system/vec.h"
# include "system/pool.h"
ff_err_t ffmain(int __argc, char const *__argv[]) {
	struct timespec ts0, ts1, ts2, ts3;

	ts2.tv_sec = 0;
	ts3.tv_sec = 0;
_again:
	ffly_printf("%u, %u\n", ts2.tv_sec, ts3.tv_sec);
	clock_gettime(CLOCK_MONOTONIC, &ts0);
 
	clock_gettime(CLOCK_MONOTONIC, &ts1);

	goto _again;
//	ffly_vecp p;
//	ff_err_t err;
//	p = ffly_vec(10, VEC_AUTO_RESIZE|VEC_AS, &err);
//	void *pp;

//	ffly_vec_push_back(p, &pp);
//	ffly_vec_destroy(p);
//	ffly_mapp p;
//	p = ffly_map(_ffly_map_255);

//	ffly_map_put(p, "hello", 5, NULL);

//	ffly_map_destroy(p);
//	__asm__("subq $16, %rsp");
/*
ffly_printf("%u\n", __argc);
#define N 12
	ff_u32_t b[N];

	ff_uint_t i;
	i = 0;
	for(;i != N;i++)
		b[i] = ffly_brick_new(_ff_brick_256, NULL, NULL, NULL, 0);
	
	for(;i != N;i++)
		ffly_brick_rid(b[i]);
*/
/*
	struct ffly_cistern ctn;
	ffly_cistern_init(&ctn, "test.cis");

# define N 24
//	while(1) {
		void *p[N];
		ff_uint_t i;

		i = 0;
		while(i != N) {
			p[i++] = ffly_cistern_alloc(&ctn, 256);
			ffly_cistern_write(&ctn, p[i-1], &i, (i-1)*sizeof(ff_uint_t), sizeof(ff_uint_t));
		}

		i = 0;
		while(i != N) {
			ffly_cistern_free(&ctn, p[i++]);
		}
//	}
*/
/*
	void *r;
	r = ffly_cistern_alloc(&ctn, 256);
	char buf[1024];
	ffly_cistern_write(&ctn, r, buf, 0, 256);
	ffly_cistern_de_init(&ctn);
*/
/*
	struct sigaction sa;
	memset(&sa, 0, sizeof(struct sigaction));
	sa.sa_handler = sig;
	sigaction(SIGINT, &sa, NULL);   
 
	struct tc_spec now;
_again:
	ff_tc_gettime(&now);
	ff_u64_t h, m, s;
	h = now.sec/3600;
	m = now.sec/60;
	s = now.sec;

	ffly_printf("time: %u-hour : %u-min %u-sec : %u\n", h, m, s, now.nsec);
	ffly_fdrain(ffly_out);
	ffly_nanosleep(1, 0);
	if (run == -1) {
		goto _again;
	}
*/
/*
	ff_stores_connect("127.0.0.1", 21299, "firefly");

	ff_stores_login("mrdoomlittle", "none");
	char const *motd;
	motd = (char const*)ff_stores_get(0x00);
	if (motd != NULL) {
		ffly_printf("motd: %s\n", motd);

		__ffly_mem_free(motd);
	} else {
		ffly_printf("error, null motd.\n");
	}
	ff_stores_logout();
	ff_stores_disconnect();
*/
/*
	ff_db_ctrp ctr;
	ff_err_t err;
	ctr = ff_db_ctr(ffly_hash("firefly", 7), "127.0.0.1", 21299, &err);
	ff_db_ctr_login(ctr, "mrdoomlittle", ffly_hash("none", 4));
	ff_db_ctr_logout(ctr);
	ff_db_ctr_shutdown(ctr);
	ff_db_ctr_destroy(ctr);
*/
//	s_test();	
/*
	ffly_bron_driver(_bron_dd_nought, &BRON_CONTEXT->driver);
	BRON_CONTEXT->stack = 0;

	struct ffly_bron_tex tex;
	tex.inn[0] = 255;
	tex.inn[1] = 255;
	tex.inn[2] = 255;
	tex.inn[3] = 255;
	struct ffly_bron_tri2 tri;
	tri.v0.x = -10;
	tri.v0.y = 0;
	tri.v1.x = 10;
	tri.v1.y = 4;
	tri.v2.x = 10;
	tri.v2.y = 10;

	ff_u16_t fb;
	ffly_bron_setctx(ffly_g_ctx_new());
	fb = ffly_bron_fb_new(76, 76);
	ffly_bron_fb_set(fb);

	ffly_bron_start();

	ff_u8_t dfc[4] = {20, 21, 22, 23};
	ffly_bron_pixfill(10*76, dfc);
	ffly_bron_tri2(&tri, ffly_g_tex(&tex));
	ff_u8_t dst[76*76*4];
	ffly_mem_set(dst, 0, 76*76*4);
	BRON_CONTEXT->driver.frame(dst, 0, 0, 76, 76);

	ffly_bron_finish();
	ffly_bron_fb_destroy(fb);
	ffly_bron_done();

	ffly_printf("dst: %p\n", dst);
	ff_uint_t x, y;
	y = 0;
	while(y != 76) {
		x = 0;
		while(x != 76) {
			ff_u32_t v;
			v = *(ff_u32_t*)(dst+((x+(y*76))*4));
			ff_u8_t r,g,b,a;
			r = v&0xff;
			g = v>>8&0xff;
			b = v>>16&0xff;
			a = v>>24&0xff;
			char c;
			//ffly_printf("%u.%u.%u.%u\n", r, g, b, a);
			if (r == 20 && g == 21 && b == 22 && a == 23) {
				c = '@';
			} else {
				if (v == 21) {
					c = '!';
				} else if (v>0) {
					c = '#';
				} else {
					c = ' ';
				}
			}
			ffly_printf("%c", c);
			x++;
		}
		ffly_printf("\n");
		y++;
	}
*/
/*
# define WIDTH 512
# define HEIGHT 512
# define NV 5
	struct bron_vertex2 v[2][NV] = {
	{
		{0, 0},
		{256-44, 44},
		{256, 256},
		{44, 256-44},
		{0, 0}
	},
	{
		{511, 0},
		{256+44, 44},
		{256, 256},
		{511-44, 256-44},
		{511, 0}
	}
	};

	bron_dd(_bron_dd_nought, &BRON_CONTEXT->driver);
	BRON_CONTEXT->stack = 0;

	ff_err_t err;
	__frame_buff__ = ffly_frame_buff_creat(WIDTH, HEIGHT, 4, &err);
	int out;
	out = open("test.ppm", O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
    char buf[128];
	ff_uint_t size;
	size = ffly_sprintf(buf, "P6\n%u %u\n255\n", WIDTH, HEIGHT);
	write(out, buf, size-1);
	ff_u8_t row[WIDTH*3];

	ff_u8_t src[WIDTH*HEIGHT*4];
	ffly_mem_set(src, 255, WIDTH*HEIGHT*4);

	ffly_grp_prepare(&__ffly_grp__, 100);
	ff_u16_t fb;
	bron_setctx(bron_ctx_new());
	fb = bron_fb_new(WIDTH, HEIGHT);
	bron_fb_set(fb);

	bron_start();



	ff_u16_t ob0, ob1;
	ob0 = ffly_bron_objbuf_new(NV*sizeof(struct bron_vertex2));
	ob1 = ffly_bron_objbuf_new(NV*sizeof(struct bron_vertex2));
	ffly_bron_objbuf_map(ob0);
	ffly_bron_objbuf_map(ob1);
	ffly_bron_objbuf_write(ob0, 0, NV*sizeof(struct bron_vertex2), &v[0]);
	ffly_bron_objbuf_write(ob1, 0, NV*sizeof(struct bron_vertex2), &v[1]);

	ffly_colour_t ca = {122, 34, 4, 255};
	ffly_colour_t cb = {41, 65, 104, 255};
	ffly_pixfill(WIDTH*HEIGHT, ca, 0);	
	ffly_grp_unload(&__ffly_grp__);
	//BRON_CONTEXT->driver.sb(NOUGHT_BLEND);
	ffly_pixfill(WIDTH*256, cb, 0);
	ffly_grp_unload(&__ffly_grp__);

	BRON_CONTEXT->driver.draw(ob0, NV);
	BRON_CONTEXT->driver.draw(ob1, NV);
	ffly_bron_objbuf_unmap(ob0);
	ffly_bron_objbuf_unmap(ob1);
	ffly_bron_objbuf_destroy(ob0);
	ffly_bron_objbuf_destroy(ob1);
	ffly_fb_copy(__frame_buff__);
	bron_finish();
	bron_fb_destroy(fb);
	bron_done();
	ffly_fb_yank(__frame_buff__);
	ff_uint_t y;
	y = 0;
	while(y != HEIGHT) {
		ffly_mem_set(row, 0, WIDTH*3);
		frame_read_rgb(__frame_buff__, row, WIDTH, 1, 0, y);
		write(out, row, WIDTH*3);
		y++;
	}	

	ffly_frame_buff_del(__frame_buff__);
	close(out);
//	ffly_tile_cleanup();
*/
/*
	ff_uint_t i, n;

	i = 0;
	ffly_carriage_add(_ff_carr0);
	ffly_carriage_add(_ff_carr0);
	ffly_carriage_add(_ff_carr0);
	ffly_carriage_add(_ff_carr0);
	while(i != 26) {
		ffly_carriage_put(_ff_carr0, (void*)0);
		n  = 0;
		while(n++ != 26) {
		if (ffly_carriage_turn(_ff_carr0, 0) == -1) continue;
		ffly_carriage_done(_ff_carr0, 0);
		}
		n = 0;
		while(n++ != 26) {
		if (ffly_carriage_turn(_ff_carr0, 1) == -1) continue;
		ffly_carriage_done(_ff_carr0, 1);
		}
		ffly_printf("%d\n", ffly_carriage_turn(_ff_carr0, 2));

		n = 0;
		while(n++ != 26) {
		if (ffly_carriage_turn(_ff_carr0, 2) == -1) continue;
		ffly_carriage_done(_ff_carr0, 2);
		}

		n = 0;
		while(n++ != 26) {
		if (ffly_carriage_turn(_ff_carr0, 3) == -1) continue;
		ffly_carriage_done(_ff_carr0, 3);
		}
		ffly_printf("%d\n", ffly_carriage_turn(_ff_carr0, 2));
		ffly_carriage_wait(_ff_carr0);
		ffly_printf("%d\n", ffly_carriage_turn(_ff_carr0, 2));
		ffly_carriage_reset(_ff_carr0);
	i++;
}
*/
//	ffly_imagep	im;

//	im = ffly_im_creat(100, 100);
//	ffly_im_save(im, "test.p", _ffly_im_p);	

//	ffly_im_destroy(im);
/*
	ffly_scheduler_init(SCHED_CORRODE);
	ffly_reservoir_init(&__ffly_reservoir__, RESV_CORRODE, "test.resv");
	
	ffly_gravity_init(_ffly_uni_256, _ffly_uni_256, 4);
	ffly_gravity_add(200.0, 2, 2, 0);
	ffly_gravity_add(200.0, 100, 100, 0);


	ffly_printf("%u, %u\n", (ff_u64_t)ffly_gravity_at(2, 2, 0), (ff_u64_t)ffly_gravity_at(100, 100, 0));
	ffly_gravity_cleanup();
*/
/*
	ff_uint_t const c = 256;
	ff_uint_t n = 0;
	void *list[c+1];
	ff_u8_t buf[58];

	void *r;
# define NUM 21299
	*(ff_u64_t*)buf = NUM;
//	voist
	while(n != 1) {
		ff_uint_t i;


		i = 0;
		while(i != c) {
			*(ff_u64_t*)buf = i*n;
			list[i++] = r = ffly_reservoir_alloc(&__ffly_reservoir__, 24);
			ffly_reservoir_write(&__ffly_reservoir__, r, buf, 0, 8);
		}

		ffly_nanosleep(5, 0);
		i = 0;
		while(i != c) {
			r = list[i];
			ffly_reservoir_read(&__ffly_reservoir__, r, buf, 0, 8);
			if (*(ff_u64_t*)buf != i*n) {
				ffly_printf("error, got: %u, need: %u\n", *(ff_u64_t*)buf, i*n);
			} else {
				ffly_printf("all good.\n");
			}
			i++;
			ffly_reservoir_free(&__ffly_reservoir__, r);
		}

		n++;
	}
	*/
/*
	ff_uint_t i;
	ff_u64_t start;
	i = 0;
	while(i != 10) {
		start = clock;
		ffly_nanosleep(0, 500000000);
		ffly_printf("delta: %u\n", clock-start);
		i++;
	}
*/
//	struct timespec begin;
//	clock_gettime(CLOCK_MONOTONIC, &begin);

//	ff_u64_t end;
//	end = clock+1000;
//	while(clock<end);// {
//		ffly_printf("%u, %u\n", clock, end);
//	}
//	struct timespec now;
//	clock_gettime(CLOCK_MONOTONIC, &now);
//	ffly_printf("sec: %u, nsec: %lu\n", now.tv_sec-begin.tv_sec, now.tv_nsec-begin.tv_nsec);
//	ff_u8_t i = 0;
//	ff_tid_t t;
//	while(i != 9) {
//		ffly_thread_create(&t, th, NULL);
//		i++;
//	}
//	ffly_nanosleep(1, 0);
/*
	ff_uint_t i = 0;
	ff_u64_t start;
	while(i++ != 100) {
		start = ffly_clock_get();
		ffly_nanosleep(0, 200000000);
		ffly_printf("%lu\n", ffly_clock_get()-start);
	}
*/
/*
	ff_uint_t n;


	n = 0;
	while(n != 26) {
		ff_uint_t const c = 21;
		ff_u32_t b_list[c];
		ff_uint_t i;

		i = 0;
		while(i != c) {
			b_list[i++] = ffly_brick_new(_ff_brick_64, NULL, NULL, NULL, 0);
		}

		i = 0;
		while(i != c-2) {
			ffly_brick_rid(b_list[i++]);
		}

		n++;
	}

	ffly_printf("---------\n");
	ffly_bricks_show();
	ffly_brick_cleanup();
*/
/*
	struct termios term, old;
	tcgetattr(ffly_in->fd, &term);
	old = term;
	term.c_lflag &= ~(ICANON|ECHO);

	tcsetattr(ffly_in->fd, &term);

	ffly_fdrain(ffly_out);
	char c;
	ff_uint_t n, i;
	while(1) {
		ffly_l_show(ffly_out);
		char buf[26];
		n = read(ffly_in->fd, buf, sizeof(buf));
		i = 0;
		while(i != n) {
			c = *(buf+i);
			if (c == '\n') goto _out;
			if (c == 27) {
				i++;
				i++;
				if (*(buf+2) == 'D') {
					ffly_l_backward;
				} else if (*(buf+2) == 'C') {
					ffly_l_forward;
				}
			} else if (c>=32 && c<=126) {
				ffly_l_put(c);
			} else if (c == 127) {
				ffly_l_del();
			}
			i++;
		}
	}
_out:
	tcsetattr(ffly_in->fd, &old);
*/
/*
	ffly_scheduler_init(0);	
	ff_uint_t const c = 20;
	ff_u32_t s[c];

	ff_uint_t i = 0;
	while(i != c) {
		s[i] = ffly_schedule(NULL, NULL, 0);
		i++;
	}

	i = 0;
	while(i != c) {
		if (i != 12) 
			ffly_sched_rm(s[i]);
		i++;
	}

	i = 0;
	while(i != c) {
		s[i] = ffly_schedule(NULL, NULL, 0);
		i++;
	}

	ffly_scheduler_de_init();
*/
/*
	ffly_set_cache_dir("../cache");
	ffly_cache_prepare(20);

	ff_rid_t r;
	ffly_resource_creat(&r);
	ffly_resource_move("test.res");
*/
//	struct ffly_pallet pallet;
//	ffly_pallet_init(&pallet, 20, 20, _ffly_tile_64);
/*
	ff_uint_t i = 0;
	while(i++ != 300) {
		ffly_sched_clock_tick(1);
		ffly_scheduler_tick();
 		ffly_nanosleep(0, 1000000);
		ffly_clock_tick();
		ffly_printf("clock: %u\n", clock);
		ffly_resource_load_bin(r, "test.res");
	}

    ffly_scheduler_de_init();
	ffly_cache_cleanup();
	*/
//	ffly_pallet_de_init(&pallet);
//	ffly_resource_cleanup();
	//__ffly_mem_alloc(10);
/*
struct ffly_queue queue;
	ffly_queue_init(&queue, 1);
	ff_u8_t a;
	ffly_queue_push(&queue,&a);
	ffly_queue_push(&queue,&a);
	ffly_queue_push(&queue,&a);
	ffly_queue_push(&queue,&a);
	ffly_queue_pop(&queue,&a);
	ffly_queue_pop(&queue,&a);
	ffly_queue_pop(&queue,&a);
	ffly_queue_pop(&queue,&a);
	ffly_queue_de_init(&queue);
*/
//tls = p;
//	ff_tid_t id;
//	ffly_thread_create(&id, th, NULL);
//	ffly_thread_wait(id);
//ls_test = ffly_tls_alloc(sizeof(ff_uint_t));


//	struct ffly_task_pool pool;
//	ffly_task_pool_init(&pool, 4);

//	ff_u8_t i = 0;

//	while(i++ != 20) {
//		ffly_task_pool_add(&pool, test, NULL);
//	}
//	ffly_nanosleep(1, 0);

//	ffly_task_pool_cleanup(&pool);
//	p = malloc(200);
//	ffly_ctl(ffly_malc, _ar_getpot, (ff_u64_t)&pot);
//	ffly_tid_t id;
//	ffly_thread_create(&id, thr, NULL);
//	ffly_thread_wait(id);
/*
	struct sigaction sa;
	memset(&sa, 0, sizeof(struct sigaction));
	sa.sa_handler = sig;
	sigaction(SIGINT, &sa, NULL);	
	while(run<0);
	ffly_printf("bye.\n");
*/
/*
	double val = 1;
	double cos = 0, sin = 0; 
	cos = ffly_cos(val);
	sin = ffly_sin(val);

	ff_u64_t adr;
	__asm__("movq %%rsp, %0\n\t" : "=m"(adr));
	adr = (adr+(16-1))&-16;
	__asm__("movq %0, %%rsp\n\t" : : "m"(adr));
	ffly_printf("cos: %f, sin: %f\n", cos, sin);
*/
/*
	__linux_pid_t pid = fork();
	if (pid == 0) {
		ffly_nanosleep(2, 0);
		exit(0);
	}

	wait4(pid, NULL, __WALL, NULL);
	ffly_printf("report: %s\n", strerror(errno));
*/
/*
	char buf[PATH_MAX];
	getcwd(buf, PATH_MAX);
	ffly_printf("dir: %s\n", buf);
*/
/*
	ffly_schedule(test, NULL, 1000);
	ff_uint_t i = 0;

	while(i++ != 10000) {
		ffly_sched_clock_tick(1);
		ffly_scheduler_tick();
		ffly_nanosleep(0, 1000000);
	}

	ffly_scheduler_de_init();
*/
/*
	if (sysconf_db_loaded == -1) {
		ffly_printf("config not loaded.\n");
		goto _fault;
	}

	if (_err(ff_stores_connect(__ffly_sysconf__.db.ip_addr,
								__ffly_sysconf__.db.port,
								__ffly_sysconf__.db.enckey)))
	{
		ff_location_list();
		goto _fault;		
	}
	
	if (_err(ff_stores_login(__ffly_sysconf__.db.user, __ffly_sysconf__.db.passwd))) {
		ff_location_list();
		goto _fault;
	}
	char *motd = (char*)ff_stores_get(_ff_stores_motd);
	ffly_printf("motd: %s\n", motd);
	__ffly_mem_free(motd);
	ff_stores_logout();
	ff_stores_disconnect();
*/
/*
	// might break
	ffly_scheduler_init(SCHED_CORRODE);
	
	struct ffly_reservoir res;
	ffly_reservoir_init(&res, RESV_CORRODE, "test.resv");

	void *r0, *r1, *r2;
	r0 = ffly_reservoir_alloc(&res, 1024);
	r1 = ffly_reservoir_alloc(&res, 1024);
	r2 = ffly_reservoir_alloc(&res, 1024);

	char buf[1024];
	ffly_reservoir_write(&res, r0, buf, 0, 1024);
	ffly_reservoir_write(&res, r1, buf, 0, 1024);
	ffly_reservoir_write(&res, r2, buf, 0, 1024);

	ffly_nanosleep(2, 0);

//	ffly_reservoir_de_init(&res);
//	ffly_scheduler_de_init();

	ffly_nanosleep(2, 0);
*/
}
