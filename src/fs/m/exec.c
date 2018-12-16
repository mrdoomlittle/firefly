# include "m.h"
# include "../../system/io.h"
# include "../../string.h"
// tobe renamed

/*
	used for node init/setup like loading dir sub nodes into hash table.
	why not just use structs?
		because its less flexible and its not a real file system.
*/
ff_u8_t static *cc = NULL;
void loadin_node(struct mfs_node*, struct mfs_engrave*);
struct mfs_node* mfs_node_new(ff_u32_t);
void op0(void) {
/*
	load node
*/
	ff_u32_t nn, ca;
	nn = *(ff_u32_t*)cc;
	ca = *(ff_u32_t*)(cc+4);

	struct mfs_engrave eg;
	cr_read(&eg, sizeof(struct mfs_engrave), ca);
	loadin_node(mfs_node_new(nn), &eg);
	ffly_printf("op-0;\n");
}

struct mfs_node *mfs_getnode(ff_u32_t);
void op1(void) {	
	struct mfs_node *dir, *sub;
	dir = mfs_getnode(*(ff_u32_t*)cc);
	sub = mfs_getnode(*(ff_u32_t*)(cc+4));

	if (!(dir->ntf&MFS_HHT)) {
		dir->h = mfs_hash_new();
		dir->ntf |= MFS_HHT;
	}

	mfs_hash_put(dir->h, sub->name, strlen(sub->name), sub->nn);
	ffly_printf("op-1;\n");

}

void(*op[])(void) = {
	op0,
	op1
};

ff_u8_t static ops[] = {
	8,
	8
};

# include "../../hexdump.h"
void mfs_exec(ff_u8_t *__code, ff_uint_t __n) {
	ffly_printf("exec code dump.\n");
	ffly_hexdump(__code, __n);
	ff_u8_t *end;
	cc = __code;
	end = cc+__n;
	ff_u8_t on;
	ffly_printf("mfs-exec. %u\n", __n);
	while(cc != end) {
		on = *(cc++);
		ffly_printf("op no: %u\n", on);
		op[on]();
		cc+=ops[on];
	}
}
