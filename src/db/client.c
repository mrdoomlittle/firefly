# include "client.h"
# include "../system/io.h"
# include "../system/thread.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
void* ff_db_serve(void*);
# define MAX 20

ff_db_clp static client[MAX];
ff_db_clp static *fresh = client;
ff_db_clp static *devoid[MAX];
ff_db_clp static **next = devoid;

struct arg_s {
	void *arg_p;
	mdl_uint_t id;
};

void static*
prox(void *__arg_p) {
	struct arg_s *arg = (struct arg_s*)__arg_p;
	ff_db_serve(arg->arg_p);
	ff_db_client_destory(arg->id);
	__ffly_mem_free(__arg_p);
	return NULL;
}

mdl_uint_t
ff_db_client(FF_SOCKET *__sock, void *__arg_p) {
	ff_db_clp *p;
	if (next>devoid)
		p = *(--next);
	else {
		if (fresh>=client+MAX) {
			ffly_printf("max clients.\n");
			return 0;
		}
		p = fresh++;
	}
	*p = (ff_db_clp)__ffly_mem_alloc(sizeof(struct ff_db_cl));
	ff_db_clp cl = *p;
	mdl_uint_t id = p-client;

	cl->conn.ctr.sock = __sock;
	struct arg_s *arg = (struct arg_s*)__ffly_mem_alloc(sizeof(struct arg_s));
	arg->arg_p = __arg_p;
	arg->id = id; 
	ffly_thread_create(&cl->thread, prox, arg);
	return id;
}

void ff_db_client_destory(mdl_uint_t __id) {
	ff_db_clp *p = client+__id;
	__ffly_mem_free(*p);
	if (p == fresh-1)
		fresh--;
	else {
		*(next++) = p;
	}
}
