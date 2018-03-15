# include "../db.h"
# include "../system/io.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../dep/mem_cpy.h"
# include "../dep/mem_cmp.h"
// needs testing
/*
	connected users will need a key inorder to do anything.
*/
struct node {
	mdl_u8_t *key;
	struct node *next;
};

mdl_u8_t static rand = 0;
/*
	TODO: make random gen.
*/
void ff_db_keygen(mdl_u8_t *__key) {
	mdl_u8_t *p = __key;
	while(p != __key+KEY_SIZE)
		*(p++) = rand++;
}

mdl_uint_t static keysum(mdl_u8_t *__key) {
	mdl_u8_t *p = __key;
	mdl_uint_t ret = 0;
	while(p != __key+KEY_SIZE) ret+= *(p++);
	return ret; 
}

typedef struct node* nodep;
void ff_db_add_key(ff_dbdp __d, mdl_u8_t *__key) {
	nodep n = (nodep)__ffly_mem_alloc(sizeof(struct node));
	n->key = (mdl_u8_t*)__ffly_mem_alloc(KEY_SIZE);
	ffly_mem_cpy(n->key, __key, KEY_SIZE);
	n->next = NULL;
 
	void **p = __d->list+(keysum(__key)&0xff);
	if (!*p)
		*p = (void*)n;
	else {
		nodep end = (nodep)*p;
		while(end->next != NULL)
			end = end->next;
		end->next = n;
	}	   
}

void ff_db_rm_key(ff_dbdp __d, mdl_u8_t *__key) { 
	mdl_uint_t sum = keysum(__key);
	nodep beg = (nodep)*(__d->list+(sum&0xff));
	nodep p = beg;
	nodep prev = NULL;
	while(p != NULL) {
		if (!ffly_mem_cmp(__key, p->key, KEY_SIZE)) {
			if (p == beg)
				*(__d->list+(sum&0xff)) = p->next; 
			else
				prev->next = p->next; 
 
			__ffly_mem_free(p->key);
			__ffly_mem_free(p);
			break; 
		}
		prev = p;
		p = p->next;
	}  
}

mdl_u8_t ff_db_valid_key(ff_dbdp __d, mdl_u8_t *__key) {
	nodep p = *(__d->list+(keysum(__key)&0xff));
	while(p != NULL) {
		if (!ffly_mem_cmp(__key, p->key, KEY_SIZE))
			return 0;
		p = p->next;
	}
	return 1;
}

/*
# define TEST 200

int main() {
	ffly_io_init();
	struct ff_dbd daemon;
	daemon.list = (void**)__ffly_mem_alloc((KEY_SIZE*0xff)*sizeof(void*));
	mdl_u16_t key = 0;
	while(key != TEST) {
		ff_db_add_key(&daemon, (mdl_u8_t*)&key);
		key++;
	}

	key = 0;
	while(key != 50) {
		ff_db_rm_key(&daemon, (mdl_u8_t*)&key);
		key++;
	}
	
	key = 0;
	while(key != TEST) {
		ffly_printf("%u, valid: %s\n", key, !ff_db_valid_key(&daemon, (mdl_u8_t*)&key)?"yes":"no");
		key++;
	}
	__ffly_mem_free(daemon.list);
	ffly_io_closeup();
}*/


