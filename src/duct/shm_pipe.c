# include "../duct.h"
# include "../system/pipe.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
struct context {
	ff_u8_t pipe;
	ff_u8_t flags;
};

void static
_open(struct context *__ct) {
	ff_err_t err;
	__ct->pipe = ffly_pipe(512, __ct->flags, 0, &err);
}

void static
_close(struct context *__ct) {
	ffly_pipe_close(__ct->pipe);
}

void static
_read(struct context *__ct, void *__buf, ff_uint_t __n) {
	ffly_pipe_read(__buf, __n, __ct->pipe);	
}

void static
_write(struct context *__ct, void *__buf, ff_uint_t __n) {
	ffly_pipe_write(__buf, __n, __ct->pipe);
}

void static
_listen(struct context *__ct) {
	ffly_pipe_listen(__ct->pipe);
}

void static
_connect(struct context *__ct) {
	ffly_pipe_connect(__ct->pipe);
}

void duct_shm_pipe(ff_dcp __c, ff_u8_t __flags) {
	struct context *ct;
	ct = __ffly_mem_alloc(sizeof(struct context));
	ct->flags = __flags;
	__c->arg = (long long)ct;
	__c->open = (void(*)(long long))_open;
	__c->close = (void(*)(long long))_close;
	__c->read = (void(*)(long long, void*, ff_uint_t))_read;
	__c->write = (void(*)(long long, void*, ff_uint_t))_write;
	__c->listen = (void(*)(long long))_listen;
	__c->connect = (void(*)(long long))_connect;
}
