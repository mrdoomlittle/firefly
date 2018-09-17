# include "../clay.h"
# include "parser.h"
# include "hash.h"
# include "memalloc.h"
# include "../ffly_def.h"
# include "../stdio.h"
# define solidify(__clay, __n) out[(__n)->type](__clay, __n)

/*
	rename
*/

void static out_val(ffly_clayp, clay_nodep);
void static out_trove(ffly_clayp, clay_nodep);
static void(*out[])(ffly_clayp, clay_nodep) = {
	out_val,
	out_trove
};

# define tsz(__t) (1<<(((__t)-_clay_8)+3))

static struct clay_hash *hash;
void
out_val(ffly_clayp __clay, clay_nodep __n) {
	struct clay_val *val;

	val = (struct clay_val*)clay_mem_alloc(sizeof(struct clay_val));
	clay_hash_put(hash, __n->name, __n->nlen-1, val);
	val->type = __n->val_id;
	if (__n->val_id == _clay_str) {
		val->text = __n->p;
	} else {
		val->text = clay_mem_alloc(tsz(__n->val_id));
		ff_u64_t iv;
		iv = *(ff_u64_t*)__n->val;
		switch(__n->val_id) {
			case _clay_8:
				*(ff_u8_t*)val->text = iv; 
			break;
			case _clay_16:
				*(ff_u16_t*)val->text = iv;
			break;
			case _clay_32:
				*(ff_u32_t*)val->text = iv;
			break;
			case _clay_64:
				*(ff_u64_t*)val->text = iv;
			break;
		}
		printf("int type: %u, %u, %u\n", __n->val_id, iv, tsz(__n->val_id));
	}
}

void
out_trove(ffly_clayp __clay, clay_nodep __n) {
	struct clay_trove *t;

	t = (struct clay_trove*)clay_mem_alloc(sizeof(struct clay_trove));
	clay_hash_put(hash, __n->name, __n->nlen-1, t);
	struct clay_hash *pres;
	pres = hash;
	hash = &t->table;
	clay_hash_init(hash);
	clay_nodep n;
	n = __n->link;
	while(n != NULL) {
		solidify(__clay, n);
		n = n->link;
	}
	hash = pres;
}

void clay_solidify(ffly_clayp __clay, clay_nodep __top) {
	clay_nodep n;
	n = __top;
	hash = &__clay->table;
	clay_hash_init(hash);
	while(n != NULL) {
		printf("node: %u, %s\n", n->type, n->name);
		solidify(__clay, n);
		n = n->next;
	}
}
