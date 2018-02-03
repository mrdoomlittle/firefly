# include "io.h"
# include "vec.h"
# include "map.h"
# include "task_pool.h"
# include "nanosleep.h"
# include "../types/err_t.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "thread.h"
# include "config.h"
void* thr(void *__arg_p) {
	ffly_printf("HI.\n");
}

# include "map.h"
ffly_err_t ffmain(int __argc, char const *__argv[]) {
	struct ffly_map map;
	ffly_map_init(&map, _ffly_map_127);
	mdl_u64_t key = 0;
	ffly_map_put(&map, (mdl_u8_t const*)&key, 8, (void*)0);
	key = 1;
	ffly_map_put(&map, (mdl_u8_t const*)&key, 8, (void*)1);
	key = 2;
	ffly_map_put(&map, (mdl_u8_t const*)&key, 8, (void*)2);
	key = 3;
	ffly_map_put(&map, (mdl_u8_t const*)&key, 8, (void*)3);
	key = 4;
	ffly_map_put(&map, (mdl_u8_t const*)&key, 8, (void*)4);

	key = 3;
	ffly_map_del(&map, ffly_map_fetch(&map, (mdl_u8_t const*)&key, 8));

	ffly_err_t err;
	key = 0;
	ffly_printf("%p\n", ffly_map_get(&map, (mdl_u8_t const*)&key, 8, &err));
	key = 1;
	ffly_printf("%p\n", ffly_map_get(&map, (mdl_u8_t const*)&key, 8, &err));
	key = 2;
	ffly_printf("%p\n", ffly_map_get(&map, (mdl_u8_t const*)&key, 8, &err));
	key = 3;
	ffly_printf("%p\n", ffly_map_get(&map, (mdl_u8_t const*)&key, 8, &err));
	if (_err(err)) {
		ffly_printf("failure.\n");
	}
	key = 4;
	ffly_printf("%p\n", ffly_map_get(&map, (mdl_u8_t const*)&key, 8, &err));

	ffly_map_de_init(&map);

//	ffly_tid_t tid;
//	ffly_thread_create(&tid, &thr, NULL);
//	ffly_nanosleep(1, 0);
//	ffly_errmsg("testing");
//	ffly_printf("%s\n", __ffly_sysconf__.root_dir);	
/*
	ffly_task_pool_init(&__ffly_task_pool__, 2);
	

	ffly_nanosleep(1, 0);
	ffly_task_pool_cleanup(&__ffly_task_pool__);
*/
/*
    ffly_err_t err;
    ffly_vecp vec = ffly_vec(sizeof(mdl_u64_t), VEC_AUTO_RESIZE|VEC_NONCONTINUOUS|VEC_BLK_CHAIN, &err);

    mdl_u64_t *list[0xff];
    mdl_u8_t i = 0;
    while(i != 0xff) {
        mdl_u64_t *p;
        ffly_vec_push_back(vec, (void**)&p);
        *p = i;
        list[i] = p;
        ffly_printf("push: %u\n", i);
        i++;
    }

    i = 254;
    while(i != 200) {
        ffly_vec_del(vec, list[i]);
        i--;
    }
    mdl_u64_t *p = (mdl_u64_t*)ffly_vec_rend(vec);
    ffly_printf("%lu\n", *p);
*/
/*
    mdl_u64_t *itr = ffly_vec_begin(vec);
    while(itr != NULL) {
        ffly_printf("%lu\n", *itr);
        ffly_vec_itr(vec, (void**)&itr, VEC_ITR_FD, 1);
    }
*/
//    ffly_vec_destroy(vec);
}
